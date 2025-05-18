#include "PathServiceManager.h"
#include "PathServiceConstants.h"

// Qt includes
#include <QStandardPaths>
#include <QDir>

// Joins two path components
static QString joinPath(const QString& base, const QString& sub) {
    return QDir(base).filePath(sub);
}

// Returns the application's root directory for local data
QString PathServiceManager::appDataRootDir() {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Returns the path to the application's configuration folder
QString PathServiceManager::appConfigFolderPath() {
    return joinPath(appDataRootDir(), APP_DATA_CONFIG_FOLDER_NAME);
}

// Returns the full path to the initial metadata file
QString PathServiceManager::appInitMetadataFilePath() {
    return joinPath(appConfigFolderPath(), APP_DATA_FILE_APP_INIT_METADATA);
}

// Returns the full path to the notifications log file
QString PathServiceManager::appNotificationsFilePath() {
    return joinPath(appConfigFolderPath(), APP_DATA_FILE_NOTIFICATIONS_LOG);
}

// Returns the full path to the user settings file
QString PathServiceManager::userSettingsFilePath() {
    return joinPath(appConfigFolderPath(), APP_DATA_FILE_USER_SETTINGS);
}

// Returns the root directory used for backups
QString PathServiceManager::backupDataRootDir() {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Returns the path to the main backup setup folder
QString PathServiceManager::backupSetupFolderPath() {
    return joinPath(backupDataRootDir(), BACKUP_LOCATION_SETUP_FOLDER_NAME);
}

// Returns the path to the backup configuration folder
QString PathServiceManager::backupConfigFolderPath() {
    return joinPath(backupSetupFolderPath(), BACKUP_LOCATION_CONFIG_FOLDER_NAME);
}

// Returns the path to the backup initialization metadata file
QString PathServiceManager::backupInitMetadataFilePath() {
    return joinPath(backupConfigFolderPath(), BACKUP_LOCATION_FILE_INIT_METADATA);
}

// Returns the path to the backup logs folder
QString PathServiceManager::backupLogsFolderPath() {
    return joinPath(backupConfigFolderPath(), BACKUP_LOCATION_LOG_FOLDER_NAME);
}

// Returns the full path to a specific backup log file
QString PathServiceManager::backupLogFilePath(const QString& backupName) {
    return backupLogsFolderPath() + "/" + backupName + "_" + App::Items::k_BACKUP_SETUP_CONFIG_LOGS_FILE;
}

// Returns the expected path to a named backup folder
QString PathServiceManager::expectedBackupFolderPath(const QString& backupName) {
    return joinPath(backupDataRootDir(), backupName);
}
