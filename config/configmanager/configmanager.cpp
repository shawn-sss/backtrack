// Project includes same directory
#include "configmanager.h"

// Project includes different directory
#include "../_constants.h"
#include "../../core/utils/file_utils/jsonmanager.h"

// Built-in Qt includes
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QSaveFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QDebug>

// Singleton instance
ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

// Constructor
ConfigManager::ConfigManager() {
    setupFilePaths();
    if (isFirstRun()) {
        setupDefaults();
    }
    loadInstallMetadata();
    loadUserConfig();
}

// Setup JSON file paths
void ConfigManager::setupFilePaths() {
    appMetadataPath = getFilePath(AppConfig::k_APPDATA_SETUP_INFO_FILE);
    userConfigPath = getFilePath(AppConfig::k_APPDATA_SETUP_USER_SETTINGS_FILE);
}

// Check if first run
bool ConfigManager::isFirstRun() const {
    return !QFile::exists(appMetadataPath) || !QFile::exists(userConfigPath);
}

// Initialize default settings
void ConfigManager::setupDefaults() {
    QJsonObject installData;
    installData["location"] = getAppInstallDir();
    installData["timestamp"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    QJsonObject appMetadata;
    appMetadata[InstallMetadataKeys::APP_NAME] = AppInfo::k_APP_DISPLAY_TITLE;
    appMetadata[InstallMetadataKeys::APP_AUTHOR] = AppInfo::k_AUTHOR_NAME;
    appMetadata[InstallMetadataKeys::APP_VERSION] = AppInfo::k_APP_VERSION;
    appMetadata["install"] = installData;

    JsonManager::saveJsonFile(appMetadataPath, appMetadata);

    QJsonObject userConfig;
    userConfig[ConfigKeys::BACKUP_CONFIG_GROUP] = QJsonObject{
        {ConfigKeys::BACKUP_DIRECTORY_KEY, ConfigDefaults::BACKUP_DIRECTORY},
        {ConfigKeys::BACKUP_PREFIX_KEY, ConfigDefaults::BACKUP_PREFIX}
    };

    JsonManager::saveJsonFile(userConfigPath, userConfig);
}

// Load install metadata
void ConfigManager::loadInstallMetadata() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(appMetadataPath, rootObject)) {
        if (rootObject.isEmpty()) {
            return;
        }
        installMetadata = rootObject;
    }
}

// Save install metadata
void ConfigManager::saveInstallMetadata() {
    JsonManager::saveJsonFile(appMetadataPath, installMetadata);
}

// Load user configuration
void ConfigManager::loadUserConfig() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(userConfigPath, rootObject)) {
        if (rootObject.isEmpty()) {
            return;
        }
        userSettings = rootObject;
    }
}

// Save user configuration
void ConfigManager::saveUserConfig() {
    JsonManager::saveJsonFile(userConfigPath, userSettings);
}

// Retrieve backup directory
QString ConfigManager::getBackupDirectory() const {
    if (!userSettings.contains(ConfigKeys::BACKUP_CONFIG_GROUP) ||
        !userSettings[ConfigKeys::BACKUP_CONFIG_GROUP].isObject()) {
        return ConfigDefaults::BACKUP_DIRECTORY;
    }

    return userSettings[ConfigKeys::BACKUP_CONFIG_GROUP]
        .toObject()
        .value(ConfigKeys::BACKUP_DIRECTORY_KEY)
        .toString(ConfigDefaults::BACKUP_DIRECTORY);
}

// Set backup directory
void ConfigManager::setBackupDirectory(const QString& dir) {
    QJsonObject backupSettings = userSettings[ConfigKeys::BACKUP_CONFIG_GROUP].toObject();

    if (backupSettings[ConfigKeys::BACKUP_DIRECTORY_KEY].toString() != dir) {
        backupSettings[ConfigKeys::BACKUP_DIRECTORY_KEY] = dir;
        userSettings[ConfigKeys::BACKUP_CONFIG_GROUP] = backupSettings;
        saveUserConfig();
    }
}

// Retrieve backup prefix
QString ConfigManager::getBackupPrefix() const {
    if (!userSettings.contains(ConfigKeys::BACKUP_CONFIG_GROUP) ||
        !userSettings[ConfigKeys::BACKUP_CONFIG_GROUP].isObject()) {
        return ConfigDefaults::BACKUP_PREFIX;
    }

    return userSettings[ConfigKeys::BACKUP_CONFIG_GROUP]
        .toObject()
        .value(ConfigKeys::BACKUP_PREFIX_KEY)
        .toString(ConfigDefaults::BACKUP_PREFIX);
}

// Set backup prefix
void ConfigManager::setBackupPrefix(const QString& prefix) {
    QJsonObject backupSettings = userSettings[ConfigKeys::BACKUP_CONFIG_GROUP].toObject();

    if (backupSettings[ConfigKeys::BACKUP_PREFIX_KEY].toString() != prefix) {
        backupSettings[ConfigKeys::BACKUP_PREFIX_KEY] = prefix;
        userSettings[ConfigKeys::BACKUP_CONFIG_GROUP] = backupSettings;
        saveUserConfig();
    }
}

// Public accessor for app install directory
QString ConfigManager::getAppInstallDirPublic() const {
    return getAppInstallDir();
}

// Retrieve app install directory
QString ConfigManager::getAppInstallDir() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Construct full file path
QString ConfigManager::getFilePath(const QString& fileName) const {
    return getAppInstallDir() + "/" + AppConfig::k_APPDATA_SETUP_FOLDER + "/" + fileName;
}
