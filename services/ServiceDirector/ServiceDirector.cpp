// Project includes
#include "ServiceDirector.h"
#include "../ServiceManagers/BackupServiceManager/BackupServiceManager.h"
#include "../ServiceManagers/InstallServiceManager/InstallServiceManager.h"
#include "../ServiceManagers/UserServiceManager/UserServiceManager.h"
#include "../ServiceManagers/UserServiceManager/UserServiceConstants.h"
#include "../../services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"
#include "../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"
#include "../../services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.h"
#include "../../ui/welcomedialog/welcomedialog.h"

// Qt includes
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QTimer>

using namespace ThemeServiceConstants;

// Singleton accessor
ServiceDirector& ServiceDirector::getInstance() {
    static ServiceDirector instance;
    return instance;
}

// Constructor initializes services and detects first run
ServiceDirector::ServiceDirector() {
    QDir().mkpath(PathServiceManager::appConfigFolderPath());

    const QString appMetadataPath = PathServiceManager::appInitMetadataFilePath();
    const QString userServicePath = PathServiceManager::userSettingsFilePath();

    installServiceManager = std::make_unique<InstallServiceManager>(appMetadataPath);
    userServiceManager = std::make_unique<UserServiceManager>(userServicePath);
    backupServiceManager = std::make_unique<BackupServiceManager>(*userServiceManager);

    firstRun = !QFile::exists(appMetadataPath) || !QFile::exists(userServicePath);
    if (firstRun) {
        setupDefaults();
    }

    installServiceManager->load();
    userServiceManager->load();
}

// Sets up default configuration for services
void ServiceDirector::setupDefaults() {
    InstallServiceManager::initializeDefaults();
    userServiceManager->initializeDefaults();
    NotificationServiceManager::initializeDefaults();
}

// Returns true if this is the application's first run
bool ServiceDirector::isFirstRun() const {
    return firstRun;
}

// Shows welcome dialog on first run
void ServiceDirector::maybeShowWelcomeDialog(QWidget* parent) {
    if (!firstRun)
        return;

    QTimer::singleShot(0, parent, [parent]() {
        WelcomeDialog welcomeDialog(parent);
        welcomeDialog.exec();
    });
}

// Applies user theme settings
void ServiceDirector::applyTheme() {
    getThemeServiceManager()->applyTheme();
}

// Installs theme event filter on given QObject
void ServiceDirector::installThemeEventFilter(QObject* target) {
    getThemeServiceManager()->installEventFilter(target);
}

// Gets user's theme preference
UserThemePreference ServiceDirector::getThemePreference() const {
    const auto& settings = userServiceManager->settings();
    const auto& key = UserServiceKeys::k_THEME_PREFERENCE_KEY;

    return settings.contains(key)
               ? stringToUserThemePreference(settings.value(key).toString())
               : UserThemePreference::Auto;
}

// Sets user's theme preference
void ServiceDirector::setThemePreference(UserThemePreference preference) {
    auto& settings = userServiceManager->settings();
    settings[UserServiceKeys::k_THEME_PREFERENCE_KEY] = userThemePreferenceToString(preference);
    userServiceManager->save();
}

// Loads install service metadata
void ServiceDirector::loadInstallMetadata() {
    installServiceManager->load();
}

// Saves install service metadata
void ServiceDirector::saveInstallMetadata() {
    installServiceManager->save();
}

// Accessor for user service manager
UserServiceManager* ServiceDirector::getUserServiceManager() {
    return userServiceManager.get();
}

// Accessor for backup service manager
BackupServiceManager* ServiceDirector::getBackupServiceManager() {
    return backupServiceManager.get();
}

// Accessor for install service manager
InstallServiceManager* ServiceDirector::getInstallServiceManager() {
    return installServiceManager.get();
}

// Accessor for theme service manager
ThemeServiceManager* ServiceDirector::getThemeServiceManager() {
    return &ThemeServiceManager::instance();
}
