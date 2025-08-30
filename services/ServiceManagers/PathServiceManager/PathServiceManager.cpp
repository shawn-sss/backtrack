// Project includes
#include "PathServiceManager.h"
#include "PathServiceConstants.h"

// Qt includes
#include <QDir>
#include <QStandardPaths>

static QString joinPath(const QString& base, const QString& sub) {
    return QDir(base).filePath(sub);
}

// Control: set and get backup directory
void PathServiceManager::setBackupDirectory(const QString& path) {
    userBackupDir = path;
}

QString PathServiceManager::getBackupDirectory() {
    return userBackupDir;
}

// Application data paths
QString PathServiceManager::appDataRootDir() {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

QString PathServiceManager::appConfigFolderPath() {
    return joinPath(appDataRootDir(), AppDataConfigFolderName);
}

QString PathServiceManager::appInitMetadataFilePath() {
    return joinPath(appConfigFolderPath(), AppDataFileInitMetadata);
}

QString PathServiceManager::appNotificationsFilePath() {
    return joinPath(appConfigFolderPath(), AppDataFileNotifications);
}

QString PathServiceManager::userSettingsFilePath() {
    return joinPath(appConfigFolderPath(), AppDataFileUserSettings);
}

// Backup paths
QString PathServiceManager::backupDataRootDir() {
    if (userBackupDir.isEmpty()) {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    }
    return userBackupDir;
}

QString PathServiceManager::backupSetupFolderPath() {
    return joinPath(backupDataRootDir(), BackupSetupFolderName);
}

QString PathServiceManager::backupConfigFolderPath() {
    const QString base = backupDataRootDir();
    const QString defaultRoot = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    if (base == defaultRoot) {
        return joinPath(joinPath(base, BackupSetupFolderName), BackupConfigFolderName);
    }
    return joinPath(base, BackupConfigFolderName);
}

QString PathServiceManager::backupInitMetadataFilePath() {
    return joinPath(backupConfigFolderPath(), BackupFileInitMetadata);
}

QString PathServiceManager::backupLogsFolderPath() {
    return joinPath(backupConfigFolderPath(), BackupLogsFolderName);
}

QString PathServiceManager::backupLogFilePath(const QString& backupName) {
    return joinPath(backupLogsFolderPath(), backupName + "_" + App::Items::k_BACKUP_SETUP_CONFIG_LOGS_FILE);
}

QString PathServiceManager::expectedBackupFolderPath(const QString& backupName) {
    return joinPath(backupDataRootDir(), backupName);
}
