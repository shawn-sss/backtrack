// Project includes
#include "PathServiceManager.h"
#include "PathServiceConstants.h"

// Qt includes
#include <QDir>
#include <QStandardPaths>

// Joins two path segments into a single, correctly formatted file path
static QString joinPath(const QString& base, const QString& sub) {
    return QDir(base).filePath(sub);
}

// Sets the user-defined backup directory
void PathServiceManager::setBackupDirectory(const QString& path) {
    userBackupDir = path;
}

// Gets the user-defined backup directory
QString PathServiceManager::getBackupDirectory() {
    return userBackupDir;
}

// Gets the application's root directory for local data
QString PathServiceManager::appDataRootDir() {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Gets the path to the application's configuration folder
QString PathServiceManager::appConfigFolderPath() {
    return joinPath(appDataRootDir(), APP_DATA_CONFIG_FOLDER_NAME);
}

// Gets the full path to the initial metadata file
QString PathServiceManager::appInitMetadataFilePath() {
    return joinPath(appConfigFolderPath(), APP_DATA_FILE_APP_INIT_METADATA);
}

// Gets the full path to the notifications log file
QString PathServiceManager::appNotificationsFilePath() {
    return joinPath(appConfigFolderPath(), APP_DATA_FILE_NOTIFICATIONS_LOG);
}

// Gets the full path to the user settings file
QString PathServiceManager::userSettingsFilePath() {
    return joinPath(appConfigFolderPath(), APP_DATA_FILE_USER_SETTINGS);
}

// Gets the root directory used for backups
QString PathServiceManager::backupDataRootDir() {
    if (userBackupDir.isEmpty()) {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    }
    return userBackupDir;
}

// Gets the path to the main backup setup folder
QString PathServiceManager::backupSetupFolderPath() {
    return joinPath(backupDataRootDir(), BACKUP_LOCATION_SETUP_FOLDER_NAME);
}

// Gets the path to the backup configuration folder
QString PathServiceManager::backupConfigFolderPath() {
    const QString base = backupDataRootDir();
    const QString defaultRoot = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    if (base == defaultRoot) {
        return joinPath(joinPath(base, BACKUP_LOCATION_SETUP_FOLDER_NAME), BACKUP_LOCATION_CONFIG_FOLDER_NAME);
    }

    return joinPath(base, BACKUP_LOCATION_CONFIG_FOLDER_NAME);
}

// Gets the path to the backup initialization metadata file
QString PathServiceManager::backupInitMetadataFilePath() {
    return joinPath(backupConfigFolderPath(), BACKUP_LOCATION_FILE_INIT_METADATA);
}

// Gets the path to the backup logs folder
QString PathServiceManager::backupLogsFolderPath() {
    return joinPath(backupConfigFolderPath(), BACKUP_LOCATION_LOG_FOLDER_NAME);
}

// Gets the full path to a specific backup log file
QString PathServiceManager::backupLogFilePath(const QString& backupName) {
    return joinPath(backupLogsFolderPath(), backupName + "_" + App::Items::k_BACKUP_SETUP_CONFIG_LOGS_FILE);
}

// Gets the expected path to a named backup folder
QString PathServiceManager::expectedBackupFolderPath(const QString& backupName) {
    return joinPath(backupDataRootDir(), backupName);
}
