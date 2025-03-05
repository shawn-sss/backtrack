#include "configmanager.h"
#include "../_constants.h"

#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonDocument>

// Instance management
ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

// Constructor
ConfigManager::ConfigManager() {
    load();
}

// Config file path management
QString ConfigManager::getConfigFilePath() const {
    static const QString baseConfigDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    static const QString artifactsDir = baseConfigDir + "/" + AppConfig::RUNTIME_STORAGE_FOLDER;
    static const QString configFilePath = artifactsDir + "/" + AppConfig::CONFIG_FILE_NAME;
    QDir().mkpath(artifactsDir);
    return configFilePath;
}


// Returns public-facing config file path
QString ConfigManager::getConfigFilePathPublic() const {
    return getConfigFilePath();
}

// Loads configuration from file
void ConfigManager::load() {
    QFile file(getConfigFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        if (settings.isEmpty()) {
            settings[ConfigKeys::BACKUP_DIRECTORY_KEY] = DEFAULT_BACKUP_DIRECTORY;
            settings[ConfigKeys::BACKUP_PREFIX_KEY] = DEFAULT_BACKUP_PREFIX;
            save();
        }
        return;
    }

    const QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isNull() && doc.isObject()) {
        settings = doc.object();
    }
}

// Saves configuration to file
void ConfigManager::save() {
    QSaveFile file(getConfigFilePath());
    if (!file.open(QIODevice::WriteOnly)) return;

    QByteArray jsonData = QJsonDocument(settings).toJson(QJsonDocument::Compact);
    file.write(jsonData);
    file.commit();
}

// Gets current backup directory
QString ConfigManager::getBackupDirectory() const {
    return settings.value(ConfigKeys::BACKUP_DIRECTORY_KEY).toString(DEFAULT_BACKUP_DIRECTORY);
}

// Sets backup directory and saves if changed
void ConfigManager::setBackupDirectory(const QString& dir) {
    if (settings.value(ConfigKeys::BACKUP_DIRECTORY_KEY).toString() != dir) {
        settings[ConfigKeys::BACKUP_DIRECTORY_KEY] = dir;
        save();
    }
}

// Gets current backup prefix
QString ConfigManager::getBackupPrefix() const {
    return settings.value(ConfigKeys::BACKUP_PREFIX_KEY).toString(DEFAULT_BACKUP_PREFIX);
}

// Sets backup prefix and saves if changed
void ConfigManager::setBackupPrefix(const QString& prefix) {
    if (settings.value(ConfigKeys::BACKUP_PREFIX_KEY).toString() != prefix) {
        settings[ConfigKeys::BACKUP_PREFIX_KEY] = prefix;
        save();
    }
}
