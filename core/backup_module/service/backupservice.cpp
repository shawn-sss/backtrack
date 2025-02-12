#include "backupservice.h"
#include "../../utils/file_utils/fileoperations.h"
#include "../../../core/config/constants.h"
#include "../../../core/utils/common_utils/utils.h"

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
    for (const QFileInfo &dirInfo : QDir(backupRootPath).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time)) {
        if (QFile::exists(QDir(dirInfo.absoluteFilePath()).filePath(UserSettings::BACKUP_SUMMARY_FILENAME))) {
            return true;
        }
    }
    return false;
}

QJsonObject BackupService::getLastBackupMetadata() const {
    for (const QFileInfo &subDir : QDir(backupRootPath).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time)) {
        QString summaryFilePath = QDir(subDir.absoluteFilePath()).filePath(UserSettings::BACKUP_SUMMARY_FILENAME);
        if (QFile::exists(summaryFilePath)) {
            return FileOperations::readJsonFromFile(summaryFilePath);
        }
    }
    return QJsonObject();
}

void BackupService::createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) {
    QJsonObject summaryObject = createBackupMetadata(backupFolderPath, selectedItems, backupDuration);

    QString summaryFilePath = QDir(backupFolderPath).filePath(UserSettings::BACKUP_SUMMARY_FILENAME);
    FileOperations::writeJsonToFile(summaryFilePath, summaryObject);

    QString logsFolderPath = QDir(QDir(backupRootPath).filePath(AppConfig::BACKUP_SETTINGS_FOLDER))
                                 .filePath(AppConfig::BACKUP_LOGS_FOLDER);

    QDir logDir(logsFolderPath);
    if (!logDir.exists()) {
        logDir.mkpath(logsFolderPath);
    }

    QString logFileName = QFileInfo(backupFolderPath).fileName() + "_backup_log.json";
    FileOperations::writeJsonToFile(QDir(logsFolderPath).filePath(logFileName), summaryObject);
}

// Metadata Generation
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
            traverseDirectoryForFolders(item, uniqueFolders, foldersArray);
            traverseDirectory(item, uniqueFiles, filesArray);
        } else if (!uniqueFiles.contains(item)) {
            uniqueFiles.insert(item);
            filesArray.append(item);
        }
    }

    qint64 totalSize = calculateTotalBackupSize(selectedItems);
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

// Backup Statistics
int BackupService::getBackupCount() const {
    int count = 0;
    for (const QFileInfo &subDir : QDir(backupRootPath).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (QFile::exists(QDir(subDir.absoluteFilePath()).filePath(UserSettings::BACKUP_SUMMARY_FILENAME))) {
            ++count;
        }
    }
    return count;
}

quint64 BackupService::getTotalBackupSize() const {
    quint64 totalSize = 0;
    for (const QFileInfo &subDir : QDir(backupRootPath).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
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
        totalSize += fileInfo.isDir() ? FileOperations::calculateDirectorySize(item) : fileInfo.size();
    }
    return totalSize;
}

void BackupService::traverseDirectory(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) const {
    FileOperations::collectFilesRecursively(dirPath, uniqueFiles, filesArray);
}

void BackupService::traverseDirectoryForFolders(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray) const {
    for (const QFileInfo &subDir : QDir(dirPath).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (!uniqueFolders.contains(subDir.absoluteFilePath())) {
            uniqueFolders.insert(subDir.absoluteFilePath());
            foldersArray.append(subDir.absoluteFilePath());
            traverseDirectoryForFolders(subDir.absoluteFilePath(), uniqueFolders, foldersArray);
        }
    }
}
