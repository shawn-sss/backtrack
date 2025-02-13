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

// Helper
void BackupService::traverseDirectoryForFolders(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray) const {
    QDir dir(dirPath);
    QFileInfoList subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo &subDir : subDirs) {
        QString subDirPath = subDir.absoluteFilePath();
        if (!uniqueFolders.contains(subDirPath)) {
            uniqueFolders.insert(subDirPath);
            foldersArray.append(subDirPath);
            traverseDirectoryForFolders(subDirPath, uniqueFolders, foldersArray);
        }
    }
}

void BackupService::traverseDirectory(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) const {
    QDir dir(dirPath);
    QFileInfoList fileEntries = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    // Collect all files in the directory
    for (const QFileInfo &entry : fileEntries) {
        QString fullPath = entry.absoluteFilePath();
        if (!uniqueFiles.contains(fullPath)) {
            uniqueFiles.insert(fullPath);
            filesArray.append(fullPath);
        }
    }

    // Recursively process subdirectories
    QFileInfoList subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &subDir : subDirs) {
        traverseDirectory(subDir.absoluteFilePath(), uniqueFiles, filesArray);
    }
}

qint64 BackupService::calculateTotalBackupSize(const QStringList &selectedItems) const {
    qint64 totalSize = 0;

    for (const QString &item : selectedItems) {
        QFileInfo fileInfo(item);

        if (fileInfo.isDir()) {
            totalSize += FileOperations::calculateDirectorySize(item);
        } else {
            totalSize += fileInfo.size();
        }
    }

    return totalSize;
}

// Backup Metadata Management
BackupStatus BackupService::scanForBackupStatus() const {
    QString backupSettingsPath = QDir(backupRootPath).filePath(AppConfig::BACKUP_SETTINGS_FOLDER);
    QString logsFolderPath = QDir(backupSettingsPath).filePath(AppConfig::BACKUP_LOGS_FOLDER);
    QString settingsFilePath = QDir(backupSettingsPath).filePath(AppConfig::SETTINGS_FILE_NAME);

    QDir backupSettingsDir(backupSettingsPath);
    QDir logsDir(logsFolderPath);

    if (!backupSettingsDir.exists()) {
        return BackupStatus::None;
    }

    if (!logsDir.exists() || !QFile::exists(settingsFilePath)) {
        return BackupStatus::Broken;
    }

    return BackupStatus::Valid;
}

QJsonObject BackupService::getLastBackupMetadata() const {
    QString logsFolderPath = QDir(backupRootPath).filePath(
        QString("%1/%2").arg(AppConfig::BACKUP_SETTINGS_FOLDER, AppConfig::BACKUP_LOGS_FOLDER));

    QDir logsDir(logsFolderPath);
    logsDir.setSorting(QDir::Time);

    QFileInfoList logFiles = logsDir.entryInfoList(
        QStringList() << "*" + AppConfig::BACKUP_LOG_SUFFIX, QDir::Files, QDir::Time);

    if (!logFiles.isEmpty()) {
        return FileOperations::readJsonFromFile(logFiles.first().absoluteFilePath());
    }
    return QJsonObject();
}

// Backup Summary Creation
void BackupService::createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) {
    QJsonObject summaryObject = createBackupMetadata(backupFolderPath, selectedItems, backupDuration);

    // Ensure logs folder exists
    QString logsFolderPath = QDir(QDir(backupRootPath).filePath(AppConfig::BACKUP_SETTINGS_FOLDER))
                                 .filePath(AppConfig::BACKUP_LOGS_FOLDER);
    QDir logDir(logsFolderPath);
    if (!logDir.exists()) {
        logDir.mkpath(logsFolderPath);
    }

    // Save metadata only to the logs folder
    QString logFileName = QFileInfo(backupFolderPath).fileName() + AppConfig::BACKUP_LOG_SUFFIX;
    FileOperations::writeJsonToFile(QDir(logsFolderPath).filePath(logFileName), summaryObject);
}

// Backup Statistics
int BackupService::getBackupCount() const {
    QString logsFolderPath = QDir(backupRootPath).filePath(
        QString("%1/%2").arg(AppConfig::BACKUP_SETTINGS_FOLDER, AppConfig::BACKUP_LOGS_FOLDER));

    QDir logsDir(logsFolderPath);

    if (!logsDir.exists()) {
        return 0;
    }

    return logsDir.entryList(QStringList() << "*" + AppConfig::BACKUP_LOG_SUFFIX, QDir::Files).size();
}

quint64 BackupService::getTotalBackupSize() const {
    quint64 totalSize = 0;
    QString logsFolderPath = QDir(backupRootPath).filePath(
        QString("%1/%2").arg(AppConfig::BACKUP_SETTINGS_FOLDER, AppConfig::BACKUP_LOGS_FOLDER));

    QDir logsDir(logsFolderPath);
    QFileInfoList logFiles = logsDir.entryInfoList(QStringList() << "*" + AppConfig::BACKUP_LOG_SUFFIX, QDir::Files);

    for (const QFileInfo &logFile : logFiles) {
        QJsonObject metadata = FileOperations::readJsonFromFile(logFile.absoluteFilePath());
        totalSize += metadata.value(BackupMetadataKeys::SIZE_BYTES).toVariant().toULongLong();
    }

    return totalSize;
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
