// Project includes
#include "ServiceDirector.h"
#include "../ServiceManagers/InstallServiceManager/InstallServiceManager.h"
#include "../ServiceManagers/UserServiceManager/UserServiceManager.h"
#include "../ServiceManagers/UserServiceManager/UserServiceConstants.h"
#include "../ServiceManagers/BackupServiceManager/BackupServiceManager.h"
#include "../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"
#include "../../services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.h"
#include "../../services/ServiceManagers/TemplateServiceManager/TemplateServiceManager.h"
#include "../../ui/welcomedialog/welcomedialog.h"

// Qt includes
#include <QFile>
#include <QDir>
#include <QTimer>

// C++ includes
#include <memory>

// Lifecycle
ServiceDirector& ServiceDirector::getInstance() {
    static ServiceDirector instance;
    return instance;
}

ServiceDirector::ServiceDirector() {
    QDir().mkpath(PathServiceManager::appConfigFolderPath());

    const QString appMetadataPath = PathServiceManager::appInitMetadataFilePath();
    const QString userServicePath = PathServiceManager::userSettingsFilePath();

    installServiceManager = std::make_unique<InstallServiceManager>(appMetadataPath);
    userServiceManager    = std::make_unique<UserServiceManager>(userServicePath);
    backupServiceManager  = std::make_unique<BackupServiceManager>(*userServiceManager);

    firstRun = !QFile::exists(appMetadataPath) || !QFile::exists(userServicePath);
    if (firstRun) {
        setupDefaults();
    }

    installServiceManager->load();
    userServiceManager->load();
}

bool ServiceDirector::isFirstRun() const {
    return firstRun;
}

// Defaults
void ServiceDirector::setupDefaults() {
    InstallServiceManager::initializeDefaults();
    userServiceManager->initializeDefaults();
    NotificationServiceManager::initializeDefaults();
    TemplateServiceManager::initializeDefaults();
}

// Welcome dialog
void ServiceDirector::maybeShowWelcomeDialog(QWidget* parent) {
    if (!firstRun)
        return;

    QTimer::singleShot(ServiceDirectorConstants::kImmediateMs, parent, [parent]() {
        WelcomeDialog welcomeDialog(parent);
        welcomeDialog.exec();
    });
}

// Theme
void ServiceDirector::applyTheme() {
    getThemeServiceManager()->applyTheme();
}

void ServiceDirector::installThemeEventFilter(QObject* target) {
    getThemeServiceManager()->installEventFilter(target);
}

ThemeServiceConstants::UserThemePreference ServiceDirector::getThemePreference() const {
    const auto& settings = userServiceManager->settings();
    const auto& key      = UserServiceKeys::ThemePreferenceKey;

    return settings.contains(key)
               ? ThemeServiceConstants::stringToUserThemePreference(settings.value(key).toString())
               : ThemeServiceConstants::UserThemePreference::Auto;
}

void ServiceDirector::setThemePreference(ThemeServiceConstants::UserThemePreference preference) {
    auto& settings = userServiceManager->settings();
    settings[UserServiceKeys::ThemePreferenceKey] =
        ThemeServiceConstants::userThemePreferenceToString(preference);
    userServiceManager->save();
}

// Install metadata
void ServiceDirector::loadInstallMetadata() {
    installServiceManager->load();
}

void ServiceDirector::saveInstallMetadata() {
    installServiceManager->save();
}

// Accessors
UserServiceManager* ServiceDirector::getUserServiceManager() {
    return userServiceManager.get();
}

BackupServiceManager* ServiceDirector::getBackupServiceManager() {
    return backupServiceManager.get();
}

InstallServiceManager* ServiceDirector::getInstallServiceManager() {
    return installServiceManager.get();
}

ThemeServiceManager* ServiceDirector::getThemeServiceManager() {
    return &ThemeServiceManager::instance();
}
