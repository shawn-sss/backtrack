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
    return !QFile::exists(getFilePath(AppConfig::APPDATA_SETUP_INFO_FILE)) ||
           !QFile::exists(getFilePath(AppConfig::APPDATA_SETUP_USER_SETTINGS_FILE));
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

    saveJsonFile(getFilePath(AppConfig::APPDATA_SETUP_INFO_FILE), appMetadata);

    QJsonObject userConfig;
    userConfig["backup_config"] = QJsonObject{
        {"location", ConfigDefaults::BACKUP_DIRECTORY},
        {"prefix", ConfigDefaults::BACKUP_PREFIX}
    };

    saveJsonFile(getFilePath(AppConfig::APPDATA_SETUP_USER_SETTINGS_FILE), userConfig);
}

// Load JSON from file
bool ConfigManager::loadJsonFile(const QString& path, QJsonObject& target) {
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
bool ConfigManager::saveJsonFile(const QString& path, const QJsonObject& data) {
    QDir().mkpath(QFileInfo(path).absolutePath());
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(QJsonDocument(data).toJson(QJsonDocument::Indented));
    return file.commit();
}

// Load install metadata
void ConfigManager::loadInstallMetadata() {
    QJsonObject rootObject;
    if (loadJsonFile(getFilePath(AppConfig::APPDATA_SETUP_INFO_FILE), rootObject)) {
        installMetadata = rootObject;
    }
}

// Save install metadata
void ConfigManager::saveInstallMetadata() {
    saveJsonFile(getFilePath(AppConfig::APPDATA_SETUP_INFO_FILE), installMetadata);
}

// Load user configuration
void ConfigManager::loadUserConfig() {
    QJsonObject rootObject;
    if (loadJsonFile(getFilePath(AppConfig::APPDATA_SETUP_USER_SETTINGS_FILE), rootObject)) {
        userSettings = rootObject;
    }
}

// Save user configuration
void ConfigManager::saveUserConfig() {
    saveJsonFile(getFilePath(AppConfig::APPDATA_SETUP_USER_SETTINGS_FILE), userSettings);
}

// Retrieve backup directory
QString ConfigManager::getBackupDirectory() const {
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
