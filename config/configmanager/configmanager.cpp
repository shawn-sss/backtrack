// Project includes same directory
#include "configmanager.h"

// Project includes different directory
#include "../_constants.h"

// Built-in Qt includes
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QSaveFile>
#include <QJsonDocument>
#include <QStandardPaths>

// Singleton instance
ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

// Constructor
ConfigManager::ConfigManager() {
    if (isFirstRun()) {
        setupDefaults();
    }
    loadInstallMetadata();
    loadUserConfig();
}

// Retrieve app install directory
QString ConfigManager::getAppInstallDir() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Public accessor for app install directory
QString ConfigManager::getAppInstallDirPublic() const {
    return getAppInstallDir();
}

// Construct full file path
QString ConfigManager::getFilePath(const QString& fileName) const {
    return getAppInstallDir() + "/" + AppConfig::RUNTIME_STORAGE_FOLDER + "/" + fileName;
}

// Check if first run
bool ConfigManager::isFirstRun() const {
    return !QFile::exists(getFilePath(AppConfig::INSTALL_METADATA_FILE_NAME)) ||
           !QFile::exists(getFilePath(AppConfig::USER_CONFIG_FILE_NAME));
}

// Initialize default settings
void ConfigManager::setupDefaults() {
    installMetadata = {
        {"app_name", AppInfo::APP_DISPLAY_TITLE},
        {"app_author", AppInfo::AUTHOR_NAME},
        {"app_version", AppInfo::APP_VERSION},
        {"install_dir", getAppInstallDir()},
        {"install_time", QDateTime::currentDateTimeUtc().toString(Qt::ISODate)}
    };

    userSettings = {
        {ConfigKeys::BACKUP_DIRECTORY_KEY, ConfigDefaults::BACKUP_DIRECTORY},
        {ConfigKeys::BACKUP_PREFIX_KEY, ConfigDefaults::BACKUP_PREFIX}
    };

    saveInstallMetadata();
    saveUserConfig();
}

// Load JSON from file
bool loadJsonFile(const QString& path, QJsonObject& target) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        return false;
    }
    target = doc.object();
    return true;
}

// Save JSON to file
bool saveJsonFile(const QString& path, const QJsonObject& data) {
    QDir().mkpath(QFileInfo(path).absolutePath());
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(QJsonDocument(data).toJson(QJsonDocument::Compact));
    return file.commit();
}

// Load install metadata
void ConfigManager::loadInstallMetadata() {
    loadJsonFile(getFilePath(AppConfig::INSTALL_METADATA_FILE_NAME), installMetadata);
}

// Save install metadata
void ConfigManager::saveInstallMetadata() {
    saveJsonFile(getFilePath(AppConfig::INSTALL_METADATA_FILE_NAME), installMetadata);
}

// Load user configuration
void ConfigManager::loadUserConfig() {
    loadJsonFile(getFilePath(AppConfig::USER_CONFIG_FILE_NAME), userSettings);
}

// Save user configuration
void ConfigManager::saveUserConfig() {
    saveJsonFile(getFilePath(AppConfig::USER_CONFIG_FILE_NAME), userSettings);
}

// Retrieve backup directory
QString ConfigManager::getBackupDirectory() const {
    return userSettings.value(ConfigKeys::BACKUP_DIRECTORY_KEY).toString(ConfigDefaults::BACKUP_DIRECTORY);
}

// Set backup directory
void ConfigManager::setBackupDirectory(const QString& dir) {
    if (userSettings.value(ConfigKeys::BACKUP_DIRECTORY_KEY).toString() != dir) {
        userSettings[ConfigKeys::BACKUP_DIRECTORY_KEY] = dir;
        saveUserConfig();
    }
}

// Retrieve backup prefix
QString ConfigManager::getBackupPrefix() const {
    return userSettings.value(ConfigKeys::BACKUP_PREFIX_KEY).toString(ConfigDefaults::BACKUP_PREFIX);
}

// Set backup prefix
void ConfigManager::setBackupPrefix(const QString& prefix) {
    if (userSettings.value(ConfigKeys::BACKUP_PREFIX_KEY).toString() != prefix) {
        userSettings[ConfigKeys::BACKUP_PREFIX_KEY] = prefix;
        saveUserConfig();
    }
}
