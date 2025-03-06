#include "backupservice.h"

#include "../../../config/_constants.h"
#include "../../utils/file_utils/fileoperations.h"
#include "../../../core/utils/common_utils/utils.h"

#include <QDir>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>

// Constructor
BackupService::BackupService(const QString &backupRoot)
    : backupRootPath(backupRoot) {}

// Backup root management
void BackupService::setBackupRoot(const QString &path) {
    backupRootPath = path;
}

QString BackupService::getBackupRoot() const {
    return backupRootPath;
}

// File and folder collection
void BackupService::collectBackupFiles(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) const {
    FileOperations::collectFilesRecursively(dirPath, uniqueFiles, filesArray);
}

void BackupService::collectBackupFolders(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray) const {
    FileOperations::collectDirectoriesRecursively(dirPath, uniqueFolders, foldersArray);
}

qint64 BackupService::calculateTotalBackupSize(const QStringList &selectedItems) const {
    qint64 totalSize = 0;
    for (const QString &item : selectedItems) {
        QFileInfo fileInfo(item);
        totalSize += fileInfo.isDir() ? FileOperations::calculateDirectorySize(QDir(item)) : fileInfo.size();
    }
    return totalSize;
}

// Backup status and metadata
BackupStatus BackupService::scanForBackupStatus() const {
    const QString configFolderPath = QDir(backupRootPath).filePath(AppConfig::BACKUP_CONFIG_FOLDER);
    const QString logsFolderPath = QDir(configFolderPath).filePath(AppConfig::BACKUP_LOGS_DIRECTORY);
    const QString configFilePath = QDir(configFolderPath).filePath(AppConfig::BACKUP_CONFIG_FILE_NAME);

    if (!QDir(configFolderPath).exists()) return BackupStatus::None;
    return (!QDir(logsFolderPath).exists() || !QFile::exists(configFilePath)) ? BackupStatus::Broken : BackupStatus::Valid;
}

QJsonObject BackupService::getLastBackupMetadata() const {
    const QString logsFolderPath = QDir(backupRootPath).filePath(QString("%1/%2")
                                                                     .arg(AppConfig::BACKUP_CONFIG_FOLDER, AppConfig::BACKUP_LOGS_DIRECTORY));
    QDir logsDir(logsFolderPath);
    logsDir.setSorting(QDir::Time);

    QFileInfoList logFiles = logsDir.entryInfoList(QStringList() << ("*_" + AppConfig::BACKUP_LOG_FILE_SUFFIX), QDir::Files, QDir::Time);
    return logFiles.isEmpty() ? QJsonObject() : FileOperations::readJsonFromFile(logFiles.first().absoluteFilePath());
}

// Creates a backup summary file
void BackupService::createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) {
    const QJsonObject summaryObject = createBackupMetadata(backupFolderPath, selectedItems, backupDuration);
    const QString logsFolderPath = QDir(QDir(backupRootPath).filePath(AppConfig::BACKUP_CONFIG_FOLDER)).filePath(AppConfig::BACKUP_LOGS_DIRECTORY);

    if (!QDir(logsFolderPath).exists()) {
        QDir().mkpath(logsFolderPath);
    }

    const QString logFileName = QFileInfo(backupFolderPath).fileName() + "_" + AppConfig::BACKUP_LOG_FILE_SUFFIX;
    FileOperations::writeJsonToFile(QDir(logsFolderPath).filePath(logFileName), summaryObject);
}

// Counts the number of existing backups
int BackupService::getBackupCount() const {
    const QString logsFolderPath = QDir(backupRootPath).filePath(QString("%1/%2")
                                                                     .arg(AppConfig::BACKUP_CONFIG_FOLDER, AppConfig::BACKUP_LOGS_DIRECTORY));

    return QDir(logsFolderPath).exists()
               ? QDir(logsFolderPath).entryList(QStringList() << "*_" + AppConfig::BACKUP_LOG_FILE_SUFFIX, QDir::Files).size()
               : 0;
}

// Calculates the total size of all backups
quint64 BackupService::getTotalBackupSize() const {
    quint64 totalSize = 0;
    const QString logsFolderPath = QDir(backupRootPath).filePath(QString("%1/%2")
                                                                     .arg(AppConfig::BACKUP_CONFIG_FOLDER, AppConfig::BACKUP_LOGS_DIRECTORY));

    QFileInfoList logFiles = QDir(logsFolderPath).entryInfoList(QStringList() << "*_" + AppConfig::BACKUP_LOG_FILE_SUFFIX, QDir::Files);
    for (const QFileInfo &logFile : logFiles) {
        QJsonObject metadata = FileOperations::readJsonFromFile(logFile.absoluteFilePath());
        totalSize += metadata.value(BackupMetadataKeys::SIZE_BYTES).toVariant().toULongLong();
    }
    return totalSize;
}

// Generates metadata for a new backup
QJsonObject BackupService::createBackupMetadata(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) const {
    QJsonObject summaryObject;
    summaryObject[BackupMetadataKeys::NAME] = QFileInfo(backupFolderPath).fileName();
    summaryObject[BackupMetadataKeys::TIMESTAMP] = Utils::Formatting::formatTimestamp(QDateTime::currentDateTime(), Qt::ISODate);
    summaryObject[BackupMetadataKeys::DURATION] = backupDuration;
    summaryObject[BackupMetadataKeys::DURATION_READABLE] = Utils::Formatting::formatDuration(backupDuration);

    QJsonArray filesArray, foldersArray, userSelectedItemsArray;
    QSet<QString> uniqueFiles, uniqueFolders;

    for (const QString &item : selectedItems) {
        userSelectedItemsArray.append(item);
        QFileInfo fileInfo(item);
        if (fileInfo.isDir()) {
            collectBackupFolders(item, uniqueFolders, foldersArray);
            collectBackupFiles(item, uniqueFiles, filesArray);
        } else if (!uniqueFiles.contains(item)) {
            uniqueFiles.insert(item);
            filesArray.append(item);
        }
    }

    const qint64 totalSize = calculateTotalBackupSize(selectedItems);
    summaryObject[BackupMetadataKeys::SIZE_BYTES] = totalSize;
    summaryObject[BackupMetadataKeys::SIZE_READABLE] = Utils::Formatting::formatSize(totalSize);
    summaryObject[BackupMetadataKeys::FILE_COUNT] = filesArray.size();
    summaryObject[BackupMetadataKeys::FOLDER_COUNT] = uniqueFolders.size();
    summaryObject[BackupMetadataKeys::USER_SELECTED_ITEMS] = userSelectedItemsArray;
    summaryObject[BackupMetadataKeys::USER_SELECTED_ITEM_COUNT] = selectedItems.size();
    summaryObject[BackupMetadataKeys::BACKUP_FILES] = filesArray;
    summaryObject[BackupMetadataKeys::BACKUP_FOLDERS] = foldersArray;

    return summaryObject;
}
