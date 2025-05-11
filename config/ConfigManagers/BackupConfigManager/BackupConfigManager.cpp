// Project includes
#include "BackupConfigManager.h"
#include "BackupConfigConstants.h"
#include "../UserConfigManager/UserConfigManager.h"

// Constructor initializes reference to user config manager
BackupConfigManager::BackupConfigManager(UserConfigManager& configManager)
    : userConfigManager(configManager) {}

// Returns backup config group object
QJsonObject BackupConfigManager::getBackupSettings() const {
    const QJsonObject& settings = userConfigManager.settings();
    const QJsonValue groupVal = settings.value(ConfigKeys::BACKUP_CONFIG_GROUP);
    return groupVal.isObject() ? groupVal.toObject() : QJsonObject{};
}

// Returns stored or default backup directory
QString BackupConfigManager::getBackupDirectory() const {
    return getBackupSettings()
    .value(ConfigKeys::BACKUP_DIRECTORY_KEY)
        .toString(ConfigDefaults::BACKUP_DIRECTORY);
}

// Sets backup directory in settings
void BackupConfigManager::setBackupDirectory(const QString& dir) {
    updateBackupSetting(ConfigKeys::BACKUP_DIRECTORY_KEY, dir, ConfigDefaults::BACKUP_DIRECTORY);
}

// Returns stored or default backup prefix
QString BackupConfigManager::getBackupPrefix() const {
    return getBackupSettings()
    .value(ConfigKeys::BACKUP_PREFIX_KEY)
        .toString(ConfigDefaults::BACKUP_PREFIX);
}

// Sets backup prefix in settings
void BackupConfigManager::setBackupPrefix(const QString& prefix) {
    updateBackupSetting(ConfigKeys::BACKUP_PREFIX_KEY, prefix, ConfigDefaults::BACKUP_PREFIX);
}

// Updates a specific backup setting key with new value if different
void BackupConfigManager::updateBackupSetting(const QString& key, const QString& newValue, const QString& defaultValue) {
    QJsonObject& settings = userConfigManager.settings();
    QJsonObject backupSettings = settings.value(ConfigKeys::BACKUP_CONFIG_GROUP).toObject();

    const QString currentValue = backupSettings.value(key).toString(defaultValue);
    if (currentValue == newValue)
        return;

    backupSettings.insert(key, newValue);
    settings.insert(ConfigKeys::BACKUP_CONFIG_GROUP, backupSettings);
    userConfigManager.save();
}
