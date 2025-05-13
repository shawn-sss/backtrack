// Project includes
#include "BackupServiceManager.h"
#include "BackupServiceConstants.h"
#include "../UserServiceManager/UserServiceManager.h"

// Constructor initializes reference to user service manager
BackupServiceManager::BackupServiceManager(UserServiceManager& serviceManager)
    : userServiceManager(serviceManager) {}

// Returns backup service group object
QJsonObject BackupServiceManager::getBackupSettings() const {
    const QJsonObject& settings = userServiceManager.settings();
    const QJsonValue groupVal = settings.value(ServiceKeys::BACKUP_SERVICE_GROUP);
    return groupVal.isObject() ? groupVal.toObject() : QJsonObject{};
}

// Returns stored or default backup directory
QString BackupServiceManager::getBackupDirectory() const {
    return getBackupSettings()
    .value(ServiceKeys::BACKUP_DIRECTORY_KEY)
        .toString(ServiceDefaults::BACKUP_DIRECTORY);
}

// Sets backup directory in settings
void BackupServiceManager::setBackupDirectory(const QString& dir) {
    updateBackupSetting(ServiceKeys::BACKUP_DIRECTORY_KEY, dir, ServiceDefaults::BACKUP_DIRECTORY);
}

// Returns stored or default backup prefix
QString BackupServiceManager::getBackupPrefix() const {
    return getBackupSettings()
    .value(ServiceKeys::BACKUP_PREFIX_KEY)
        .toString(ServiceDefaults::BACKUP_PREFIX);
}

// Sets backup prefix in settings
void BackupServiceManager::setBackupPrefix(const QString& prefix) {
    updateBackupSetting(ServiceKeys::BACKUP_PREFIX_KEY, prefix, ServiceDefaults::BACKUP_PREFIX);
}

// Updates a specific backup setting key with new value if different
void BackupServiceManager::updateBackupSetting(const QString& key, const QString& newValue, const QString& defaultValue) {
    QJsonObject& settings = userServiceManager.settings();
    QJsonObject backupSettings = settings.value(ServiceKeys::BACKUP_SERVICE_GROUP).toObject();

    const QString currentValue = backupSettings.value(key).toString(defaultValue);
    if (currentValue == newValue)
        return;

    backupSettings.insert(key, newValue);
    settings.insert(ServiceKeys::BACKUP_SERVICE_GROUP, backupSettings);
    userServiceManager.save();
}
