// Project includes
#include "../_constants.h"
#include "../usersettingsmanager/usersettingsmanager.h"
#include "backupsettingsmanager.h"

// Constructor
BackupSettingsManager::BackupSettingsManager(UserSettingsManager& settingsManager)
    : userSettingsManager(settingsManager)
{
}

// Get backup settings as QJsonObject
QJsonObject BackupSettingsManager::getBackupSettings() const {
    const QJsonObject& settings = userSettingsManager.settings();
    if (!settings.contains(ConfigKeys::k_BACKUP_CONFIG_GROUP) ||
        !settings[ConfigKeys::k_BACKUP_CONFIG_GROUP].isObject()) {
        return {};
    }
    return settings[ConfigKeys::k_BACKUP_CONFIG_GROUP].toObject();
}

// Get backup directory path
QString BackupSettingsManager::getBackupDirectory() const {
    const QJsonObject backupSettings = getBackupSettings();
    return backupSettings.value(ConfigKeys::k_BACKUP_DIRECTORY_KEY)
        .toString(ConfigDefaults::k_BACKUP_DIRECTORY);
}

// Set backup directory path
void BackupSettingsManager::setBackupDirectory(const QString& dir) {
    QJsonObject settings = userSettingsManager.settings();
    QJsonObject backupSettings = settings[ConfigKeys::k_BACKUP_CONFIG_GROUP].toObject();

    if (backupSettings.value(ConfigKeys::k_BACKUP_DIRECTORY_KEY).toString() == dir) {
        return;
    }

    backupSettings[ConfigKeys::k_BACKUP_DIRECTORY_KEY] = dir;
    settings[ConfigKeys::k_BACKUP_CONFIG_GROUP] = backupSettings;
    userSettingsManager.save();
}

// Get backup file prefix
QString BackupSettingsManager::getBackupPrefix() const {
    const QJsonObject backupSettings = getBackupSettings();
    return backupSettings.value(ConfigKeys::k_BACKUP_PREFIX_KEY)
        .toString(ConfigDefaults::k_BACKUP_PREFIX);
}

// Set backup file prefix
void BackupSettingsManager::setBackupPrefix(const QString& prefix) {
    QJsonObject settings = userSettingsManager.settings();
    QJsonObject backupSettings = settings[ConfigKeys::k_BACKUP_CONFIG_GROUP].toObject();

    if (backupSettings.value(ConfigKeys::k_BACKUP_PREFIX_KEY).toString() == prefix) {
        return;
    }

    backupSettings[ConfigKeys::k_BACKUP_PREFIX_KEY] = prefix;
    settings[ConfigKeys::k_BACKUP_CONFIG_GROUP] = backupSettings;
    userSettingsManager.save();
}
