// Project includes
#include "configdirector.h"
#include "../../config/configsettings/_settings.h"
#include "../configmanagers/usersettingsmanager/usersettingsmanager.h"
#include "../configmanagers/installmetadatamanager/installmetadatamanager.h"
#include "../configmanagers/backupsettingsmanager/backupsettingsmanager.h"
#include "../../core/utils/file_utils/jsonmanager.h"

// Qt includes
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QSaveFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>

// Singleton access
ConfigDirector& ConfigDirector::getInstance() {
    // Return the singleton instance
    static ConfigDirector instance;
    return instance;
}

// Constructor
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

// Get the backup directory from settings
QString ConfigDirector::getBackupDirectory() const {
    return backupSettingsManager->getBackupDirectory();
}

// Set the backup directory in settings
void ConfigDirector::setBackupDirectory(const QString& dir) {
    backupSettingsManager->setBackupDirectory(dir);
}

// Get the backup prefix from settings
QString ConfigDirector::getBackupPrefix() const {
    return backupSettingsManager->getBackupPrefix();
}

// Set the backup prefix in settings
void ConfigDirector::setBackupPrefix(const QString& prefix) {
    backupSettingsManager->setBackupPrefix(prefix);
}

// Get the theme preference from settings
UserThemePreference ConfigDirector::getThemePreference() const {
    const QJsonObject& settings = userSettingsManager->settings();
    if (settings.contains(ConfigKeys::k_THEME_PREFERENCE_KEY)) {
        const QString prefStr = settings.value(ConfigKeys::k_THEME_PREFERENCE_KEY).toString();
        return stringToUserThemePreference(prefStr);
    }
    return UserThemePreference::Auto;
}

// Set the theme preference in settings
void ConfigDirector::setThemePreference(UserThemePreference preference) {
    QJsonObject& settings = userSettingsManager->settings();
    settings[ConfigKeys::k_THEME_PREFERENCE_KEY] = userThemePreferenceToString(preference);
    userSettingsManager->save();
}

// Load install metadata from storage
void ConfigDirector::loadInstallMetadata() {
    installMetadataManager->load();
}

// Save install metadata to storage
void ConfigDirector::saveInstallMetadata() {
    installMetadataManager->save();
}

// Get the publicly exposed app install directory
QString ConfigDirector::getAppInstallDirPublic() const {
    return getAppInstallDir();
}

// Get the internal app install directory
QString ConfigDirector::getAppInstallDir() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Get the full path to a file within the app's data directory
QString ConfigDirector::getFilePath(const QString& fileName) const {
    return getAppInstallDir() + "/" + AppConfig::k_APPDATA_SETUP_FOLDER + "/" + fileName;
}

// Set up file paths for metadata and user config
void ConfigDirector::setupFilePaths() {
    appMetadataPath = getFilePath(AppConfig::k_APPDATA_SETUP_INFO_FILE);
    userConfigPath = getFilePath(AppConfig::k_APPDATA_SETUP_USER_SETTINGS_FILE);
}

// Check if the application is running for the first time
bool ConfigDirector::isFirstRun() const {
    return !QFile::exists(appMetadataPath) || !QFile::exists(userConfigPath);
}

// Create default configuration files and values
void ConfigDirector::setupDefaults() {
    installMetadataManager->setupDefaults(getAppInstallDir());

    QJsonObject userConfig;
    userConfig[ConfigKeys::k_BACKUP_CONFIG_GROUP] = QJsonObject{
        {ConfigKeys::k_BACKUP_DIRECTORY_KEY, ConfigDefaults::k_BACKUP_DIRECTORY},
        {ConfigKeys::k_BACKUP_PREFIX_KEY, ConfigDefaults::k_BACKUP_PREFIX}
    };

    userConfig[ConfigKeys::k_THEME_PREFERENCE_KEY] = userThemePreferenceToString(UserThemePreference::Auto);

    JsonManager::saveJsonFile(userConfigPath, userConfig);
}
