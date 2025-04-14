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
    appMetadata[InstallMetadataKeys::k_APP_AUTHOR] = AppInfo::k_APP_AUTHOR_NAME;
    appMetadata[InstallMetadataKeys::k_APP_NAME] = AppInfo::k_APP_NAME;
    appMetadata[InstallMetadataKeys::k_APP_VERSION] = AppInfo::k_APP_VERSION;
    appMetadata["install"] = installData;

    JsonManager::saveJsonFile(appMetadataPath, appMetadata);

    QJsonObject userConfig;
    userConfig[ConfigKeys::k_BACKUP_CONFIG_GROUP] = QJsonObject{
        {ConfigKeys::k_BACKUP_DIRECTORY_KEY, ConfigDefaults::k_BACKUP_DIRECTORY},
        {ConfigKeys::k_BACKUP_PREFIX_KEY, ConfigDefaults::k_BACKUP_PREFIX}
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
    if (!userSettings.contains(ConfigKeys::k_BACKUP_CONFIG_GROUP) ||
        !userSettings[ConfigKeys::k_BACKUP_CONFIG_GROUP].isObject()) {
        return ConfigDefaults::k_BACKUP_DIRECTORY;
    }

    return userSettings[ConfigKeys::k_BACKUP_CONFIG_GROUP]
        .toObject()
        .value(ConfigKeys::k_BACKUP_DIRECTORY_KEY)
        .toString(ConfigDefaults::k_BACKUP_DIRECTORY);
}

// Set backup directory
void ConfigManager::setBackupDirectory(const QString& dir) {
    QJsonObject backupSettings = userSettings[ConfigKeys::k_BACKUP_CONFIG_GROUP].toObject();

    if (backupSettings[ConfigKeys::k_BACKUP_DIRECTORY_KEY].toString() != dir) {
        backupSettings[ConfigKeys::k_BACKUP_DIRECTORY_KEY] = dir;
        userSettings[ConfigKeys::k_BACKUP_CONFIG_GROUP] = backupSettings;
        saveUserConfig();
    }
}

// Retrieve backup prefix
QString ConfigManager::getBackupPrefix() const {
    if (!userSettings.contains(ConfigKeys::k_BACKUP_CONFIG_GROUP) ||
        !userSettings[ConfigKeys::k_BACKUP_CONFIG_GROUP].isObject()) {
        return ConfigDefaults::k_BACKUP_PREFIX;
    }

    return userSettings[ConfigKeys::k_BACKUP_CONFIG_GROUP]
        .toObject()
        .value(ConfigKeys::k_BACKUP_PREFIX_KEY)
        .toString(ConfigDefaults::k_BACKUP_PREFIX);
}

// Set backup prefix
void ConfigManager::setBackupPrefix(const QString& prefix) {
    QJsonObject backupSettings = userSettings[ConfigKeys::k_BACKUP_CONFIG_GROUP].toObject();

    if (backupSettings[ConfigKeys::k_BACKUP_PREFIX_KEY].toString() != prefix) {
        backupSettings[ConfigKeys::k_BACKUP_PREFIX_KEY] = prefix;
        userSettings[ConfigKeys::k_BACKUP_CONFIG_GROUP] = backupSettings;
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
