// Project includes
#include "configdirector.h"
#include "../configmanagers/backupsettingsmanager/backupsettingsmanager.h"
#include "../configmanagers/installmetadatamanager/installmetadatamanager.h"
#include "../configmanagers/notificationsmanager/notificationsmanager.h"
#include "../configmanagers/usersettingsmanager/usersettingsmanager.h"
#include "../../config/configsettings/_settings.h"

// Qt includes
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QStandardPaths>

// Singleton instance
ConfigDirector& ConfigDirector::getInstance() {
    static ConfigDirector instance;
    return instance;
}

// Constructor to initialize managers and set up configuration state
ConfigDirector::ConfigDirector()
    : installMetadataManager(nullptr)
    , userSettingsManager(nullptr)
    , backupSettingsManager(nullptr)
{
    setupFilePaths();

    installMetadataManager = new InstallMetadataManager(appMetadataPath);
    userSettingsManager = new UserSettingsManager(userConfigPath);
    backupSettingsManager = new BackupSettingsManager(*userSettingsManager);

    if (isFirstRun()) {
        setupDefaults();
    }

    installMetadataManager->load();
    userSettingsManager->load();
}

// Returns the configured backup directory
QString ConfigDirector::getBackupDirectory() const {
    return backupSettingsManager->getBackupDirectory();
}

// Sets the backup directory
void ConfigDirector::setBackupDirectory(const QString& dir) {
    backupSettingsManager->setBackupDirectory(dir);
}

// Returns the prefix used for backup files
QString ConfigDirector::getBackupPrefix() const {
    return backupSettingsManager->getBackupPrefix();
}

// Sets the backup file prefix
void ConfigDirector::setBackupPrefix(const QString& prefix) {
    backupSettingsManager->setBackupPrefix(prefix);
}

// Returns the user's theme preference
UserThemePreference ConfigDirector::getThemePreference() const {
    const QJsonObject& settings = userSettingsManager->settings();
    if (settings.contains(ConfigKeys::k_THEME_PREFERENCE_KEY)) {
        const QString prefStr = settings.value(ConfigKeys::k_THEME_PREFERENCE_KEY).toString();
        return stringToUserThemePreference(prefStr);
    }
    return UserThemePreference::Auto;
}

// Sets the user's theme preference
void ConfigDirector::setThemePreference(UserThemePreference preference) {
    QJsonObject& settings = userSettingsManager->settings();
    settings[ConfigKeys::k_THEME_PREFERENCE_KEY] = userThemePreferenceToString(preference);
    userSettingsManager->save();
}

// Loads the install metadata from file
void ConfigDirector::loadInstallMetadata() {
    installMetadataManager->load();
}

// Saves the current install metadata to file
void ConfigDirector::saveInstallMetadata() {
    installMetadataManager->save();
}

// Returns the application install directory (public wrapper)
QString ConfigDirector::getAppInstallDirPublic() const {
    return getAppInstallDir();
}

// Returns the application install directory path
QString ConfigDirector::getAppInstallDir() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Returns the full path to a specific config file
QString ConfigDirector::getFilePath(const QString& fileName) const {
    return getAppInstallDir() + "/" + AppConfig::k_APPDATA_SETUP_FOLDER + "/" + fileName;
}

// Initializes file paths for metadata and user settings
void ConfigDirector::setupFilePaths() {
    const QString baseDir = getAppInstallDir() + "/" + AppConfig::k_APPDATA_SETUP_FOLDER;
    appMetadataPath = baseDir + "/" + AppConfig::k_APPDATA_SETUP_INFO_FILE;
    userConfigPath = baseDir + "/" + AppConfig::k_APPDATA_SETUP_USER_SETTINGS_FILE;
}

// Determines whether this is the first time the app is run
bool ConfigDirector::isFirstRun() const {
    return !QFile::exists(appMetadataPath) || !QFile::exists(userConfigPath);
}

// Sets up default settings and values on first run
void ConfigDirector::setupDefaults() {
    InstallMetadataManager::initializeDefaults();
    userSettingsManager->initializeDefaults();
    NotificationsManager::initializeDefaults();
}
