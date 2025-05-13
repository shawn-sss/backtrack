// Project includes
#include "ServiceDirector.h"
#include "ServiceDirectorConstants.h"
#include "../ServiceManagers/BackupServiceManager/BackupServiceManager.h"
#include "../ServiceManagers/InstallServiceManager/InstallServiceManager.h"
#include "../ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"
#include "../ServiceManagers/UserServiceManager/UserServiceManager.h"
#include "../ServiceManagers/UserServiceManager/UserServiceConstants.h"
#include "../ServiceManagers/ThemeServiceManager/ThemeServiceConstants.h"

// Qt includes
#include <QFile>
#include <QJsonObject>
#include <QStandardPaths>

using namespace UserServiceKeys;
using namespace ThemeServiceConstants;

// Get singleton instance of ServiceDirector
ServiceDirector& ServiceDirector::getInstance() {
    static ServiceDirector instance;
    return instance;
}

// Constructor sets up service paths and managers
ServiceDirector::ServiceDirector() {
    setupFilePaths();

    installServiceManager = std::make_unique<InstallServiceManager>(appMetadataPath);
    userServiceManager = std::make_unique<UserServiceManager>(userServicePath);
    backupServiceManager = std::make_unique<BackupServiceManager>(*userServiceManager);

    if (isFirstRun()) {
        setupDefaults();
    }

    installServiceManager->load();
    userServiceManager->load();
}

// Returns backup directory path
QString ServiceDirector::getBackupDirectory() const {
    return backupServiceManager->getBackupDirectory();
}

// Sets backup directory path
void ServiceDirector::setBackupDirectory(const QString& dir) {
    backupServiceManager->setBackupDirectory(dir);
}

// Returns backup filename prefix
QString ServiceDirector::getBackupPrefix() const {
    return backupServiceManager->getBackupPrefix();
}

// Sets backup filename prefix
void ServiceDirector::setBackupPrefix(const QString& prefix) {
    backupServiceManager->setBackupPrefix(prefix);
}

// Returns user theme preference from service
UserThemePreference ServiceDirector::getThemePreference() const {
    const QJsonObject& settings = userServiceManager->settings();
    if (settings.contains(k_THEME_PREFERENCE_KEY)) {
        return stringToUserThemePreference(settings.value(k_THEME_PREFERENCE_KEY).toString());
    }
    return UserThemePreference::Auto;
}

// Updates theme preference in user service
void ServiceDirector::setThemePreference(UserThemePreference preference) {
    QJsonObject& settings = userServiceManager->settings();
    settings[k_THEME_PREFERENCE_KEY] = userThemePreferenceToString(preference);
    userServiceManager->save();
}

// Loads install metadata from service
void ServiceDirector::loadInstallMetadata() {
    installServiceManager->load();
}

// Saves install metadata to service
void ServiceDirector::saveInstallMetadata() {
    installServiceManager->save();
}

// Returns app installation directory path
QString ServiceDirector::getAppInstallDir() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Returns full file path within app service directory
QString ServiceDirector::getFilePath(const QString& fileName) const {
    return getAppInstallDir() + "/" + ServiceDirectorConstants::kAppDataFolder + "/" + fileName;
}

// Initializes file paths for metadata and user service
void ServiceDirector::setupFilePaths() {
    const QString baseDir = getAppInstallDir() + "/" + ServiceDirectorConstants::kAppDataFolder;
    appMetadataPath = baseDir + "/" + ServiceDirectorConstants::kMetadataFile;
    userServicePath = baseDir + "/" + ServiceDirectorConstants::kUserSettingsFile;
}

// Checks if this is the first run (missing service files)
bool ServiceDirector::isFirstRun() const {
    return !QFile::exists(appMetadataPath) || !QFile::exists(userServicePath);
}

// Sets default configurations on first run
void ServiceDirector::setupDefaults() {
    InstallServiceManager::initializeDefaults();
    userServiceManager->initializeDefaults();
    NotificationServiceManager::initializeDefaults();
}
