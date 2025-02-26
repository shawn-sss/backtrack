#include "configmanager.h"
#include "../_constants.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonDocument>

// Singleton Instance
ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

// Constructor
ConfigManager::ConfigManager() {
    load();
}

// Config File Path Management
QString ConfigManager::getConfigFilePath() const {
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(configDir);
    return configDir + "/" + AppConfig::CONFIG_FILE_NAME;
}

QString ConfigManager::getConfigFilePathPublic() const {
    return getConfigFilePath();
}

// Configuration Loading
void ConfigManager::load() {
    QFile file(getConfigFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        settings[ConfigKeys::BACKUP_DIRECTORY_KEY] = DEFAULT_BACKUP_DIRECTORY;
        settings[ConfigKeys::BACKUP_PREFIX_KEY] = DEFAULT_BACKUP_PREFIX;
        save();
        return;
    }
    settings = QJsonDocument::fromJson(file.readAll()).object();
    file.close();
}

// Configuration Saving
void ConfigManager::save() {
    QFile file(getConfigFilePath());
    if (!file.open(QIODevice::WriteOnly)) return;
    file.write(QJsonDocument(settings).toJson());
    file.close();
}

// Backup Directory Management
QString ConfigManager::getBackupDirectory() const {
    return settings.value(ConfigKeys::BACKUP_DIRECTORY_KEY).toString(DEFAULT_BACKUP_DIRECTORY);
}

void ConfigManager::setBackupDirectory(const QString& dir) {
    settings[ConfigKeys::BACKUP_DIRECTORY_KEY] = dir;
    save();
}

// Backup Prefix Management
QString ConfigManager::getBackupPrefix() const {
    return settings.value(ConfigKeys::BACKUP_PREFIX_KEY).toString(DEFAULT_BACKUP_PREFIX);
}

void ConfigManager::setBackupPrefix(const QString& prefix) {
    settings[ConfigKeys::BACKUP_PREFIX_KEY] = prefix;
    save();
}
