#include "configmanager.h"

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
    return configDir + "/config.json";
}

QString ConfigManager::getConfigFilePathPublic() const {
    return getConfigFilePath();
}

// Configuration Loading
void ConfigManager::load() {
    QFile file(getConfigFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        settings["backup_directory"] = DEFAULT_BACKUP_DIRECTORY;
        settings["backup_prefix"] = DEFAULT_BACKUP_PREFIX;
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
    return settings.value("backup_directory").toString(DEFAULT_BACKUP_DIRECTORY);
}

void ConfigManager::setBackupDirectory(const QString& dir) {
    settings["backup_directory"] = dir;
    save();
}

// Backup Prefix Management
QString ConfigManager::getBackupPrefix() const {
    return settings.value("backup_prefix").toString(DEFAULT_BACKUP_PREFIX);
}

void ConfigManager::setBackupPrefix(const QString& prefix) {
    settings["backup_prefix"] = prefix;
    save();
}
