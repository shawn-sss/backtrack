// Project includes
#include "../../configsettings/_settings.h"
#include "../usersettingsmanager/usersettingsmanager.h"
#include "backupsettingsmanager.h"

// Constructor
BackupSettingsManager::BackupSettingsManager(UserSettingsManager& settingsManager)
    : userSettingsManager(settingsManager) {}

// Return the backup settings JSON object
QJsonObject BackupSettingsManager::getBackupSettings() const {
    const QJsonObject& settings = userSettingsManager.settings();
    if (!settings.contains(ConfigKeys::k_BACKUP_CONFIG_GROUP) ||
        !settings[ConfigKeys::k_BACKUP_CONFIG_GROUP].isObject()) {
        return {};
    }
    return settings[ConfigKeys::k_BACKUP_CONFIG_GROUP].toObject();
}

// Get the current backup directory from settings
QString BackupSettingsManager::getBackupDirectory() const {
    const QJsonObject backupSettings = getBackupSettings();
    return backupSettings.value(ConfigKeys::k_BACKUP_DIRECTORY_KEY)
        .toString(ConfigDefaults::k_BACKUP_DIRECTORY);
}

// Set the backup directory in user settings
void BackupSettingsManager::setBackupDirectory(const QString& dir) {
    QJsonObject& settings = userSettingsManager.settings();

    QJsonObject backupSettings = settings.value(ConfigKeys::k_BACKUP_CONFIG_GROUP).toObject();
    const QString currentDir = backupSettings.value(ConfigKeys::k_BACKUP_DIRECTORY_KEY)
                                   .toString(ConfigDefaults::k_BACKUP_DIRECTORY);

    if (currentDir == dir)
        return;

    backupSettings[ConfigKeys::k_BACKUP_DIRECTORY_KEY] = dir;
    settings[ConfigKeys::k_BACKUP_CONFIG_GROUP] = backupSettings;

    userSettingsManager.save();
}

// Get the current backup prefix from settings
QString BackupSettingsManager::getBackupPrefix() const {
    const QJsonObject backupSettings = getBackupSettings();
    return backupSettings.value(ConfigKeys::k_BACKUP_PREFIX_KEY)
        .toString(ConfigDefaults::k_BACKUP_PREFIX);
}

// Set the backup prefix in user settings
void BackupSettingsManager::setBackupPrefix(const QString& prefix) {
    QJsonObject& settings = userSettingsManager.settings();

    QJsonObject backupSettings = settings.value(ConfigKeys::k_BACKUP_CONFIG_GROUP).toObject();
    const QString currentPrefix = backupSettings.value(ConfigKeys::k_BACKUP_PREFIX_KEY)
                                      .toString(ConfigDefaults::k_BACKUP_PREFIX);

    if (currentPrefix == prefix)
        return;

    backupSettings[ConfigKeys::k_BACKUP_PREFIX_KEY] = prefix;
    settings[ConfigKeys::k_BACKUP_CONFIG_GROUP] = backupSettings;

    userSettingsManager.save();
}
