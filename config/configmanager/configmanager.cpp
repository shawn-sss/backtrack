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
#include <QJsonDocument>
#include <QStandardPaths>
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
    appMetadataPath = getFilePath(AppConfig::APPDATA_SETUP_INFO_FILE);
    userConfigPath = getFilePath(AppConfig::APPDATA_SETUP_USER_SETTINGS_FILE);
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
    return getAppInstallDir() + "/" + AppConfig::APPDATA_SETUP_FOLDER + "/" + fileName;
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
    appMetadata["app_name"] = AppInfo::APP_DISPLAY_TITLE;
    appMetadata["app_author"] = AppInfo::AUTHOR_NAME;
    appMetadata["app_version"] = AppInfo::APP_VERSION;
    appMetadata["install"] = installData;

    JsonManager::saveJsonFile(appMetadataPath, appMetadata);

    QJsonObject userConfig;
    userConfig["backup_config"] = QJsonObject{
        {"location", ConfigDefaults::BACKUP_DIRECTORY},
        {"prefix", ConfigDefaults::BACKUP_PREFIX}
    };

    JsonManager::saveJsonFile(userConfigPath, userConfig);
}

// Load install metadata
void ConfigManager::loadInstallMetadata() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(appMetadataPath, rootObject)) {
        if (rootObject.isEmpty()) {}
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
        if (rootObject.isEmpty()) {}
        userSettings = rootObject;
    }
}

// Save user configuration
void ConfigManager::saveUserConfig() {
    JsonManager::saveJsonFile(userConfigPath, userSettings);
}

// Retrieve backup directory
QString ConfigManager::getBackupDirectory() const {
    if (!userSettings.contains("backup_config") || !userSettings["backup_config"].isObject()) {
        return ConfigDefaults::BACKUP_DIRECTORY;
    }
    return userSettings["backup_config"].toObject().value("location").toString(ConfigDefaults::BACKUP_DIRECTORY);
}

// Set backup directory
void ConfigManager::setBackupDirectory(const QString& dir) {
    QJsonObject backupSettings = userSettings["backup_config"].toObject();

    if (backupSettings["location"].toString() != dir) {
        backupSettings["location"] = dir;
        userSettings["backup_config"] = backupSettings;
        saveUserConfig();
    }
}

// Retrieve backup prefix
QString ConfigManager::getBackupPrefix() const {
    if (!userSettings.contains("backup_config") || !userSettings["backup_config"].isObject()) {
        return ConfigDefaults::BACKUP_PREFIX;
    }
    return userSettings["backup_config"].toObject().value("prefix").toString(ConfigDefaults::BACKUP_PREFIX);
}

// Set backup prefix
void ConfigManager::setBackupPrefix(const QString& prefix) {
    QJsonObject backupSettings = userSettings["backup_config"].toObject();

    if (backupSettings["prefix"].toString() != prefix) {
        backupSettings["prefix"] = prefix;
        userSettings["backup_config"] = backupSettings;
        saveUserConfig();
    }
}
