#include "backupservice.h"
#include "fileoperations.h"
#include "../Utils/constants.h"
#include "../Utils/utils.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QDateTime>
#include <QFileInfo>

// Constructor to initialize BackupService
BackupService::BackupService(const QString &backupRoot)
    : backupRootPath(backupRoot) {}

// Set the backup root path
void BackupService::setBackupRoot(const QString &path) {
    backupRootPath = path;
}

// Get the backup root path
QString BackupService::getBackupRoot() const {
    return backupRootPath;
}

// Scan for the existence of a backup summary file
bool BackupService::scanForBackupSummary() const {
    QDir destinationDir(backupRootPath);
    QFileInfoList subDirectories = destinationDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time);

    for (int i = 0; i < subDirectories.size(); ++i) { // Use indexed access
        const QFileInfo &dirInfo = subDirectories.at(i);
        QString summaryFilePath = QDir(dirInfo.absoluteFilePath()).filePath(Constants::BACKUP_SUMMARY_FILENAME);
        if (QFile::exists(summaryFilePath)) {
            return true;
        }
    }
    return false;
}

// Create metadata for a backup
void BackupService::createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) {
    QJsonObject summaryObject;

    // General backup information
    summaryObject["backup_name"] = QFileInfo(backupFolderPath).fileName();
    summaryObject["backup_timestamp"] = Utils::formatTimestamp(QDateTime::currentDateTime(), Qt::ISODate);
    summaryObject["backup_duration"] = backupDuration;
    summaryObject["backup_duration_readable"] = Utils::formatDuration(backupDuration);

    QJsonArray filesArray, foldersArray, userSelectedItemsArray;
    QSet<QString> uniqueFiles;

    for (const QString &item : selectedItems) {
        QFileInfo fileInfo(item);
        userSelectedItemsArray.append(item);

        if (fileInfo.isDir()) {
            foldersArray.append(item);
            traverseDirectory(item, uniqueFiles, filesArray);
        } else if (fileInfo.isFile() && !uniqueFiles.contains(item)) {
            uniqueFiles.insert(item);
            filesArray.append(item);
        }
    }

    qint64 totalSize = calculateTotalBackupSize(selectedItems);

    // Populate metadata
    summaryObject["total_size_bytes"] = totalSize;
    summaryObject["total_size_readable"] = Utils::formatSize(totalSize);
    summaryObject["file_count"] = filesArray.size();
    summaryObject["folder_count"] = foldersArray.size();
    summaryObject["user_selected_item_count"] = selectedItems.size();
    summaryObject["backup_files"] = filesArray;
    summaryObject["backup_folders"] = foldersArray;
    summaryObject["user_selected_items"] = userSelectedItemsArray;

    QString summaryFilePath = QDir(backupFolderPath).filePath(Constants::BACKUP_SUMMARY_FILENAME);
    FileOperations::writeJsonToFile(summaryFilePath, summaryObject);
}

// Retrieve metadata for the last backup
QJsonObject BackupService::getLastBackupMetadata() const {
    QDir dir(backupRootPath);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Time);

    QFileInfoList subDirectories = dir.entryInfoList();
    for (int i = 0; i < subDirectories.size(); ++i) { // Use indexed access
        const QFileInfo &subDir = subDirectories.at(i);
        QString summaryFilePath = QDir(subDir.absoluteFilePath()).filePath(Constants::BACKUP_SUMMARY_FILENAME);
        if (QFile::exists(summaryFilePath)) {
            return FileOperations::readJsonFromFile(summaryFilePath);
        }
    }
    return QJsonObject();
}

// Get the number of backups available
int BackupService::getBackupCount() const {
    QDir dir(backupRootPath);
    QFileInfoList subDirectories = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    int count = 0;

    for (int i = 0; i < subDirectories.size(); ++i) { // Use indexed access
        const QFileInfo &subDir = subDirectories.at(i);
        QString metadataFile = QDir(subDir.absoluteFilePath()).filePath(Constants::BACKUP_SUMMARY_FILENAME);
        if (QFile::exists(metadataFile)) {
            ++count;
        }
    }
    return count;
}

// Get the total size of all backups
quint64 BackupService::getTotalBackupSize() const {
    QDir dir(backupRootPath);
    QFileInfoList subDirectories = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    quint64 totalSize = 0;

    for (int i = 0; i < subDirectories.size(); ++i) { // Use indexed access
        const QFileInfo &subDir = subDirectories.at(i);
        QString metadataFile = QDir(subDir.absoluteFilePath()).filePath(Constants::BACKUP_SUMMARY_FILENAME);
        if (QFile::exists(metadataFile)) {
            totalSize += FileOperations::calculateDirectorySize(subDir.absoluteFilePath());
        }
    }
    return totalSize;
}

// Calculate the total size of selected items
qint64 BackupService::calculateTotalBackupSize(const QStringList &selectedItems) const {
    qint64 totalSize = 0;

    for (const QString &item : selectedItems) {
        QFileInfo fileInfo(item);
        if (fileInfo.isDir()) {
            totalSize += FileOperations::calculateDirectorySize(item);
        } else if (fileInfo.isFile()) {
            totalSize += fileInfo.size();
        }
    }

    return totalSize;
}

// Traverse a directory and collect unique files
void BackupService::traverseDirectory(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) const {
    FileOperations::collectFilesRecursively(dirPath, uniqueFiles, filesArray);
}
