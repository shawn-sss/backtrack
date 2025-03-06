#include "configmanager.h"
#include "../_constants.h"

#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QDateTime>

// Singleton instance management
ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

// Constructor - always loads both files
ConfigManager::ConfigManager() {
    if (isFirstRun()) {
        setupDefaults();
    }
    loadInstallMetadata();
    loadUserConfig();
}

// Helper - returns AppData/Local/MyDataBackupApp
QString ConfigManager::getAppInstallDir() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Paths for both files
QString ConfigManager::getInstallMetadataFilePath() const {
    return getAppInstallDir() + "/" + AppConfig::RUNTIME_STORAGE_FOLDER + "/" + AppConfig::INSTALL_METADATA_FILE_NAME;
}

QString ConfigManager::getUserConfigFilePath() const {
    return getAppInstallDir() + "/" + AppConfig::RUNTIME_STORAGE_FOLDER + "/" + AppConfig::USER_CONFIG_FILE_NAME;
}

// Public accessors for paths
QString ConfigManager::getInstallMetadataFilePathPublic() const {
    return getInstallMetadataFilePath();
}

QString ConfigManager::getUserConfigFilePathPublic() const {
    return getUserConfigFilePath();
}

// First-run detection (both files must exist)
bool ConfigManager::isFirstRun() const {
    QFile installFile(getInstallMetadataFilePath());
    QFile userFile(getUserConfigFilePath());
    return !installFile.exists() || !userFile.exists();
}

// Initializes both files on first run
void ConfigManager::setupDefaults() {
    installMetadata["app_name"] = AppInfo::APP_DISPLAY_TITLE;
    installMetadata["app_author"] = AppInfo::AUTHOR_NAME;
    installMetadata["app_version"] = AppInfo::APP_VERSION;
    installMetadata["install_dir"] = getAppInstallDir();
    installMetadata["install_time"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    userSettings[ConfigKeys::BACKUP_DIRECTORY_KEY] = ConfigDefaults::BACKUP_DIRECTORY;
    userSettings[ConfigKeys::BACKUP_PREFIX_KEY] = ConfigDefaults::BACKUP_PREFIX;

    saveInstallMetadata();
    saveUserConfig();
}

// Loads install metadata
void ConfigManager::loadInstallMetadata() {
    QFile file(getInstallMetadataFilePath());
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) {
            installMetadata = doc.object();
        }
    }
}

// Saves install metadata
void ConfigManager::saveInstallMetadata() {
    QDir().mkpath(getAppInstallDir() + "/" + AppConfig::RUNTIME_STORAGE_FOLDER);
    QSaveFile file(getInstallMetadataFilePath());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(installMetadata).toJson(QJsonDocument::Compact));
        file.commit();
    }
}

// Loads user config
void ConfigManager::loadUserConfig() {
    QFile file(getUserConfigFilePath());
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) {
            userSettings = doc.object();
        }
    }
}

// Saves user config
void ConfigManager::saveUserConfig() {
    QDir().mkpath(getAppInstallDir() + "/" + AppConfig::RUNTIME_STORAGE_FOLDER);
    QSaveFile file(getUserConfigFilePath());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(userSettings).toJson(QJsonDocument::Compact));
        file.commit();
    }
}

// Backup directory handling
QString ConfigManager::getBackupDirectory() const {
    return userSettings.value(ConfigKeys::BACKUP_DIRECTORY_KEY).toString(ConfigDefaults::BACKUP_DIRECTORY);
}

void ConfigManager::setBackupDirectory(const QString& dir) {
    if (userSettings.value(ConfigKeys::BACKUP_DIRECTORY_KEY).toString() != dir) {
        userSettings[ConfigKeys::BACKUP_DIRECTORY_KEY] = dir;
        saveUserConfig();
    }
}

// Backup prefix handling
QString ConfigManager::getBackupPrefix() const {
    return userSettings.value(ConfigKeys::BACKUP_PREFIX_KEY).toString(ConfigDefaults::BACKUP_PREFIX);
}

void ConfigManager::setBackupPrefix(const QString& prefix) {
    if (userSettings.value(ConfigKeys::BACKUP_PREFIX_KEY).toString() != prefix) {
        userSettings[ConfigKeys::BACKUP_PREFIX_KEY] = prefix;
        saveUserConfig();
    }
}
