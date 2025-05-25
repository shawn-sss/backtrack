// Project includes
#include "BackupServiceManager.h"
#include "BackupServiceConstants.h"
#include "../UserServiceManager/UserServiceManager.h"
#include "../PathServiceManager/PathServiceManager.h"

// Constructor
BackupServiceManager::BackupServiceManager(UserServiceManager& serviceManager)
    : userServiceManager(serviceManager) {}

// Returns the full backup settings group
QJsonObject BackupServiceManager::getBackupSettings() const {
    const QJsonObject& settings = userServiceManager.settings();
    const QJsonValue groupValue = settings.value(ServiceKeys::BACKUP_SERVICE_GROUP);
    return groupValue.isObject() ? groupValue.toObject() : QJsonObject{};
}

// Returns the configured backup directory
QString BackupServiceManager::getBackupDirectory() const {
    const QJsonObject backupSettings = getBackupSettings();
    return backupSettings.value(ServiceKeys::BACKUP_DIRECTORY_KEY)
        .toString(PathServiceManager::backupSetupFolderPath());
}

// Sets the backup directory
void BackupServiceManager::setBackupDirectory(const QString& dir) {
    updateBackupSetting(ServiceKeys::BACKUP_DIRECTORY_KEY, dir, PathServiceManager::backupSetupFolderPath());
}

// Returns the configured backup prefix
QString BackupServiceManager::getBackupPrefix() const {
    const QJsonObject backupSettings = getBackupSettings();
    return backupSettings.value(ServiceKeys::BACKUP_PREFIX_KEY)
        .toString(ServiceDefaults::BACKUP_PREFIX);
}

// Sets the backup prefix
void BackupServiceManager::setBackupPrefix(const QString& prefix) {
    updateBackupSetting(ServiceKeys::BACKUP_PREFIX_KEY, prefix, ServiceDefaults::BACKUP_PREFIX);
}

// Updates and persists a backup setting if changed
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
