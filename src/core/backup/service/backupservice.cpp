#include "backupservice.h"
#include "core/backup/operations/fileoperations.h"
#include "core/config/constants.h"
#include "core/utils/utils.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QDateTime>
#include <QFileInfo>

// Constructor
BackupService::BackupService(const QString &backupRoot)
    : backupRootPath(backupRoot) {}

// Backup Root Management
void BackupService::setBackupRoot(const QString &path) {
    backupRootPath = path;
}

QString BackupService::getBackupRoot() const {
    return backupRootPath;
}

// Backup Metadata Management
bool BackupService::scanForBackupSummary() const {
    QDir destinationDir(backupRootPath);
    QFileInfoList subDirectories = destinationDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time);

    for (const QFileInfo &dirInfo : subDirectories) {
        QString summaryFilePath = QDir(dirInfo.absoluteFilePath()).filePath(UserSettings::BACKUP_SUMMARY_FILENAME);
        if (QFile::exists(summaryFilePath)) {
            return true;
        }
    }
    return false;
}

QJsonObject BackupService::getLastBackupMetadata() const {
    QDir dir(backupRootPath);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Time);
    QFileInfoList subDirectories = dir.entryInfoList();

    for (const QFileInfo &subDir : subDirectories) {
        QString summaryFilePath = QDir(subDir.absoluteFilePath()).filePath(UserSettings::BACKUP_SUMMARY_FILENAME);
        if (QFile::exists(summaryFilePath)) {
            return FileOperations::readJsonFromFile(summaryFilePath);
        }
    }
    return QJsonObject();
}

void BackupService::createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) {
    QJsonObject summaryObject;

    // General metadata
    summaryObject[BackupMetadataKeys::NAME] = QFileInfo(backupFolderPath).fileName();
    summaryObject[BackupMetadataKeys::TIMESTAMP] = Utils::Formatting::formatTimestamp(QDateTime::currentDateTime(), Qt::ISODate);
    summaryObject[BackupMetadataKeys::DURATION] = backupDuration;
    summaryObject[BackupMetadataKeys::DURATION_READABLE] = Utils::Formatting::formatDuration(backupDuration);

    // Backup contents
    QJsonArray filesArray, foldersArray, userSelectedItemsArray;
    QSet<QString> uniqueFiles, uniqueFolders;

    for (const QString &item : selectedItems) {
        QFileInfo fileInfo(item);
        userSelectedItemsArray.append(item);

        if (fileInfo.isDir()) {
            traverseDirectoryForFolders(item, uniqueFolders, foldersArray);
            traverseDirectory(item, uniqueFiles, filesArray);
        } else if (fileInfo.isFile() && !uniqueFiles.contains(item)) {
            uniqueFiles.insert(item);
            filesArray.append(item);
        }
    }

    // Total backup size
    qint64 totalSize = calculateTotalBackupSize(selectedItems);

    // Populate metadata
    summaryObject[BackupMetadataKeys::SIZE_BYTES] = totalSize;
    summaryObject[BackupMetadataKeys::SIZE_READABLE] = Utils::Formatting::formatSize(totalSize);
    summaryObject[BackupMetadataKeys::FILE_COUNT] = filesArray.size();
    summaryObject[BackupMetadataKeys::FOLDER_COUNT] = uniqueFolders.size();
    summaryObject[BackupMetadataKeys::USER_SELECTED_ITEMS] = userSelectedItemsArray;
    summaryObject[BackupMetadataKeys::USER_SELECTED_ITEM_COUNT] = selectedItems.size();
    summaryObject[BackupMetadataKeys::BACKUP_FILES] = filesArray;
    summaryObject[BackupMetadataKeys::BACKUP_FOLDERS] = foldersArray;

    // Write metadata
    QString summaryFilePath = QDir(backupFolderPath).filePath(UserSettings::BACKUP_SUMMARY_FILENAME);
    FileOperations::writeJsonToFile(summaryFilePath, summaryObject);
}

// Backup Statistics
int BackupService::getBackupCount() const {
    QDir dir(backupRootPath);
    QFileInfoList subDirectories = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    int count = 0;

    for (const QFileInfo &subDir : subDirectories) {
        QString metadataFile = QDir(subDir.absoluteFilePath()).filePath(UserSettings::BACKUP_SUMMARY_FILENAME);
        if (QFile::exists(metadataFile)) {
            ++count;
        }
    }
    return count;
}

quint64 BackupService::getTotalBackupSize() const {
    QDir dir(backupRootPath);
    QFileInfoList subDirectories = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    quint64 totalSize = 0;

    for (const QFileInfo &subDir : subDirectories) {
        QString metadataFile = QDir(subDir.absoluteFilePath()).filePath(UserSettings::BACKUP_SUMMARY_FILENAME);
        if (QFile::exists(metadataFile)) {
            totalSize += FileOperations::calculateDirectorySize(subDir.absoluteFilePath());
        }
    }
    return totalSize;
}

// Helper Methods
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

void BackupService::traverseDirectory(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) const {
    FileOperations::collectFilesRecursively(dirPath, uniqueFiles, filesArray);
}

void BackupService::traverseDirectoryForFolders(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray) const {
    QDir dir(dirPath);
    QFileInfoList subDirEntries = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo &subDir : subDirEntries) {
        QString subDirPath = subDir.absoluteFilePath();

        if (!uniqueFolders.contains(subDirPath)) {
            uniqueFolders.insert(subDirPath);
            foldersArray.append(subDirPath);
            traverseDirectoryForFolders(subDirPath, uniqueFolders, foldersArray);
        }
    }
}
