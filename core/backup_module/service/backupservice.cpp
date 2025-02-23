#include "backupservice.h"
#include "../../utils/file_utils/fileoperations.h"
#include "../../../core/config/_constants.h"
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

// Traversal Calls
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

        if (fileInfo.isDir()) {
            totalSize += FileOperations::calculateDirectorySize(QDir(item));
        } else {
            totalSize += fileInfo.size();
        }
    }

    return totalSize;
}

// Backup Metadata Management
BackupStatus BackupService::scanForBackupStatus() const {
    QString configFolderPath = QDir(backupRootPath).filePath(AppConfig::BACKUP_CONFIG_FOLDER);
    QString logsFolderPath = QDir(configFolderPath).filePath(AppConfig::BACKUP_LOGS_DIRECTORY);
    QString configFilePath = QDir(configFolderPath).filePath(AppConfig::CONFIG_FILE_NAME);

    QDir configFolderDir(configFolderPath);
    QDir logsFolderDir(logsFolderPath);

    if (!configFolderDir.exists()) {
        return BackupStatus::None;
    }

    if (!logsFolderDir.exists() || !QFile::exists(configFilePath)) {
        return BackupStatus::Broken;
    }

    return BackupStatus::Valid;
}

QJsonObject BackupService::getLastBackupMetadata() const {
    QString logsFolderPath = QDir(backupRootPath).filePath(
        QString("%1/%2").arg(AppConfig::BACKUP_CONFIG_FOLDER, AppConfig::BACKUP_LOGS_DIRECTORY));

    QDir logsDir(logsFolderPath);
    logsDir.setSorting(QDir::Time);

    QFileInfoList logFiles = logsDir.entryInfoList(
        QStringList() << ("*" + QString(AppConfig::BACKUP_LOG_FILE_SUFFIX)), QDir::Files, QDir::Time);


    if (!logFiles.isEmpty()) {
        return FileOperations::readJsonFromFile(logFiles.first().absoluteFilePath());
    }
    return QJsonObject();
}

// Backup Summary Creation
void BackupService::createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) {
    QJsonObject summaryObject = createBackupMetadata(backupFolderPath, selectedItems, backupDuration);

    QString logsFolderPath = QDir(QDir(backupRootPath).filePath(AppConfig::BACKUP_CONFIG_FOLDER))
                                 .filePath(AppConfig::BACKUP_LOGS_DIRECTORY);
    QDir logDir(logsFolderPath);
    if (!logDir.exists()) {
        logDir.mkpath(logsFolderPath);
    }

    QString logFileName = QFileInfo(backupFolderPath).fileName() + AppConfig::BACKUP_LOG_FILE_SUFFIX;
    FileOperations::writeJsonToFile(QDir(logsFolderPath).filePath(logFileName), summaryObject);
}

// Backup Statistics
int BackupService::getBackupCount() const {
    QString logsFolderPath = QDir(backupRootPath).filePath(
        QString("%1/%2").arg(AppConfig::BACKUP_CONFIG_FOLDER, AppConfig::BACKUP_LOGS_DIRECTORY));

    QDir logsDir(logsFolderPath);

    if (!logsDir.exists()) {
        return 0;
    }

    return logsDir.entryList(QStringList() << "*" + AppConfig::BACKUP_LOG_FILE_SUFFIX, QDir::Files).size();
}

quint64 BackupService::getTotalBackupSize() const {
    quint64 totalSize = 0;
    QString logsFolderPath = QDir(backupRootPath).filePath(
        QString("%1/%2").arg(AppConfig::BACKUP_CONFIG_FOLDER, AppConfig::BACKUP_LOGS_DIRECTORY));

    QDir logsDir(logsFolderPath);
    const QFileInfoList logFiles = logsDir.entryInfoList(QStringList() << "*" + AppConfig::BACKUP_LOG_FILE_SUFFIX, QDir::Files);


    for (int i = 0; i < logFiles.size(); ++i) {
        const QFileInfo &logFile = logFiles.at(i);
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
            collectBackupFolders(item, uniqueFolders, foldersArray);
            collectBackupFiles(item, uniqueFiles, filesArray);
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
