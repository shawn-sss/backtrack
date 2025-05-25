// Project includes
#include "ServiceDirector.h"
#include "../ServiceManagers/BackupServiceManager/BackupServiceManager.h"
#include "../ServiceManagers/InstallServiceManager/InstallServiceManager.h"
#include "../ServiceManagers/UserServiceManager/UserServiceManager.h"
#include "../ServiceManagers/UserServiceManager/UserServiceConstants.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"
#include "../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"
#include "../../services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.h"

// Qt includes
#include <QFile>
#include <QDir>
#include <QJsonObject>

using namespace ThemeServiceConstants;

// Singleton accessor
ServiceDirector& ServiceDirector::getInstance() {
    static ServiceDirector instance;
    return instance;
}

// Constructor initializes services and loads configuration
ServiceDirector::ServiceDirector() {
    QDir().mkpath(PathServiceManager::appConfigFolderPath());

    const QString appMetadataPath = PathServiceManager::appInitMetadataFilePath();
    const QString userServicePath = PathServiceManager::userSettingsFilePath();

    installServiceManager = std::make_unique<InstallServiceManager>(appMetadataPath);
    userServiceManager = std::make_unique<UserServiceManager>(userServicePath);
    backupServiceManager = std::make_unique<BackupServiceManager>(*userServiceManager);

    if (!QFile::exists(appMetadataPath) || !QFile::exists(userServicePath)) {
        setupDefaults();
    }

    installServiceManager->load();
    userServiceManager->load();
}

// Sets up default configuration values
void ServiceDirector::setupDefaults() {
    InstallServiceManager::initializeDefaults();
    userServiceManager->initializeDefaults();
    NotificationServiceManager::initializeDefaults();
}

// User theme preference access
UserThemePreference ServiceDirector::getThemePreference() const {
    const auto& settings = userServiceManager->settings();
    const auto& key = UserServiceKeys::k_THEME_PREFERENCE_KEY;

    return settings.contains(key)
               ? stringToUserThemePreference(settings.value(key).toString())
               : UserThemePreference::Auto;
}

void ServiceDirector::setThemePreference(UserThemePreference preference) {
    auto& settings = userServiceManager->settings();
    settings[UserServiceKeys::k_THEME_PREFERENCE_KEY] = userThemePreferenceToString(preference);
    userServiceManager->save();
}

// Install metadata load/save
void ServiceDirector::loadInstallMetadata() {
    installServiceManager->load();
}

void ServiceDirector::saveInstallMetadata() {
    installServiceManager->save();
}

// Service manager accessors
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

// Theme handling methods
void ServiceDirector::applyTheme() {
    getThemeServiceManager()->applyTheme();
}

void ServiceDirector::installThemeEventFilter(QObject* target) {
    getThemeServiceManager()->installEventFilter(target);
}
