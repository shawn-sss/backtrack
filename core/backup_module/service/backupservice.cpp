// Project includes same directory
#include "backupservice.h"

// Project includes different directory
#include "../../../config/_constants.h"
#include "../../utils/file_utils/fileoperations.h"
#include "../../../core/utils/common_utils/utils.h"
#include "../../../core/utils/file_utils/jsonmanager.h"

// Built-in Qt includes
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Constructor
BackupService::BackupService(const QString &backupRoot)
    : backupRootPath(backupRoot) {}

// Sets the backup root path
void BackupService::setBackupRoot(const QString &path) {
    backupRootPath = path;
}

// Returns the current backup root path
QString BackupService::getBackupRoot() const {
    return backupRootPath;
}

// Initializes backup folder structure if missing
void BackupService::initializeBackupRootIfNeeded() {
    const QString configFilePath = QDir(backupRootPath).filePath(QStringLiteral("%1/%2").arg(Backup::Infrastructure::k_BACKUP_SETUP_FOLDER, Backup::Infrastructure::k_BACKUP_SETUP_INFO_FILE));

    if (QFile::exists(configFilePath)) {
        return;
    }

    QString errorMessage;
    if (!FileOperations::createBackupInfrastructure(backupRootPath, errorMessage)) {
        return;
    }

    QJsonObject backupData;
    backupData["location"] = backupRootPath;
    backupData["timestamp"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    QJsonObject backupConfig;
    backupConfig["app_name"] = AppInfo::k_APP_NAME;
    backupConfig["app_author"] = AppInfo::k_APP_AUTHOR_NAME;
    backupConfig["app_version"] = AppInfo::k_APP_VERSION;
    backupConfig["backup"] = backupData;

    JsonManager::saveJsonFile(configFilePath, backupConfig);
}

// Calculates total size of selected backup items
qint64 BackupService::calculateTotalBackupSize(const QStringList &items) const {
    qint64 totalSize = 0;
    for (const QString &item : items) {
        QFileInfo fileInfo(item);
        totalSize += fileInfo.isDir() ? FileOperations::calculateDirectorySize(QDir(item)) : fileInfo.size();
    }
    return totalSize;
}

// Scans the backup folder for structure and config validity
BackupStatus BackupService::scanForBackupStatus() const {
    const QDir configDir(QDir(backupRootPath).filePath(Backup::Infrastructure::k_BACKUP_SETUP_FOLDER));
    if (!configDir.exists()) return BackupStatus::None;
    bool validLogs = QDir(configDir.filePath(Backup::Infrastructure::k_BACKUP_LOGS_FOLDER)).exists();
    bool validConfig = QFile::exists(configDir.filePath(Backup::Infrastructure::k_BACKUP_SETUP_INFO_FILE));
    return (validLogs && validConfig) ? BackupStatus::Valid : BackupStatus::Broken;
}

// Retrieves the metadata of the most recent backup
QJsonObject BackupService::getLastBackupMetadata() const {
    const QDir logsDir(QDir(backupRootPath).filePath(QStringLiteral("%1/%2").arg(Backup::Infrastructure::k_BACKUP_SETUP_FOLDER, Backup::Infrastructure::k_BACKUP_LOGS_FOLDER)));
    QFileInfoList logFiles = logsDir.entryInfoList({"*_" + Backup::Infrastructure::k_BACKUP_LOGS_FILE}, QDir::Files, QDir::Time);

    if (logFiles.isEmpty()) {
        return QJsonObject();
    }

    QJsonObject metadata;
    if (JsonManager::loadJsonFile(logFiles.first().absoluteFilePath(), metadata)) {
        return metadata;
    }

    return QJsonObject();
}

// Creates a new backup summary file in logs
void BackupService::createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) {
    const QString logsFolderPath = QDir(backupRootPath).filePath(QStringLiteral("%1/%2").arg(Backup::Infrastructure::k_BACKUP_SETUP_FOLDER, Backup::Infrastructure::k_BACKUP_LOGS_FOLDER));
    if (!QDir(logsFolderPath).exists()) {
        QDir().mkpath(logsFolderPath);
    }
    QString logFileName = QFileInfo(backupFolderPath).fileName() + "_" + Backup::Infrastructure::k_BACKUP_LOGS_FILE;
    JsonManager::saveJsonFile(QDir(logsFolderPath).filePath(logFileName), createBackupMetadata(backupFolderPath, selectedItems, backupDuration));
}

// Returns the number of completed backups
int BackupService::getBackupCount() const {
    const QString logsFolderPath = QDir(backupRootPath).filePath(QStringLiteral("%1/%2").arg(Backup::Infrastructure::k_BACKUP_SETUP_FOLDER, Backup::Infrastructure::k_BACKUP_LOGS_FOLDER));
    return QDir(logsFolderPath).entryList({"*_" + Backup::Infrastructure::k_BACKUP_LOGS_FILE}, QDir::Files).size();
}

// Calculates total storage used by all backups
quint64 BackupService::getTotalBackupSize() const {
    quint64 totalSize = 0;
    const QDir logsDir(QDir(backupRootPath).filePath(QStringLiteral("%1/%2")
                                                         .arg(Backup::Infrastructure::k_BACKUP_SETUP_FOLDER, Backup::Infrastructure::k_BACKUP_LOGS_FOLDER)));
    const QFileInfoList logFiles = logsDir.entryInfoList({"*_" + Backup::Infrastructure::k_BACKUP_LOGS_FILE}, QDir::Files);

    for (const QFileInfo &logFile : logFiles) {
        QJsonObject metadata;
        if (JsonManager::loadJsonFile(logFile.absoluteFilePath(), metadata)) {
            totalSize += metadata.value(Backup::MetadataKeys::k_SIZE_BYTES).toVariant().toULongLong();
        }
    }
    return totalSize;
}

// Builds metadata object for a new backup
QJsonObject BackupService::createBackupMetadata(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) const {
    QJsonArray filesArray, foldersArray, userItemsArray;
    QSet<QString> uniqueFiles, uniqueFolders;

    for (const QString &item : selectedItems) {
        userItemsArray.append(item);
        QFileInfo fileInfo(item);
        if (fileInfo.isDir()) {
            FileOperations::collectDirectoriesRecursively(item, uniqueFolders, foldersArray);
            FileOperations::collectFilesRecursively(item, uniqueFiles, filesArray);
        } else if (!uniqueFiles.contains(item)) {
            uniqueFiles.insert(item);
            filesArray.append(item);
        }
    }

    const qint64 totalSize = calculateTotalBackupSize(selectedItems);

    return QJsonObject{
        {Backup::MetadataKeys::k_NAME, QFileInfo(backupFolderPath).fileName()},
        {Backup::MetadataKeys::k_TIMESTAMP, Utils::Formatting::formatTimestamp(QDateTime::currentDateTime(), Qt::ISODate)},
        {Backup::MetadataKeys::k_DURATION, backupDuration},
        {Backup::MetadataKeys::k_DURATION_READABLE, Utils::Formatting::formatDuration(backupDuration)},
        {Backup::MetadataKeys::k_SIZE_BYTES, totalSize},
        {Backup::MetadataKeys::k_SIZE_READABLE, Utils::Formatting::formatSize(totalSize)},
        {Backup::MetadataKeys::k_FILE_COUNT, filesArray.size()},
        {Backup::MetadataKeys::k_FOLDER_COUNT, uniqueFolders.size()},
        {Backup::MetadataKeys::k_USER_SELECTED_ITEMS, userItemsArray},
        {Backup::MetadataKeys::k_USER_SELECTED_ITEM_COUNT, selectedItems.size()},
        {Backup::MetadataKeys::k_BACKUP_FILES, filesArray},
        {Backup::MetadataKeys::k_BACKUP_FOLDERS, foldersArray}
    };
}
