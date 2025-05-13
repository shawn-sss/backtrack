// Project includes
#include "backupservice.h"
#include "../../../core/shared/fileoperations.h"
#include "../../../core/shared/jsonmanager.h"
#include "../../../core/shared/formatutils.h"
#include "../../../../services/ServiceDirector/ServiceDirector.h"
#include "../../../../constants/backup_config.h"
#include "../../../../constants/app_info.h"

// Qt includes
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// Initializes the backup service with a root path
BackupService::BackupService(const QString& backupRoot)
    : backupRootPath(backupRoot) {}

// Sets the root directory for backup operations
void BackupService::setBackupRoot(const QString& path) {
    backupRootPath = path;
}

// Returns the backup root directory
QString BackupService::getBackupRoot() const {
    return backupRootPath;
}

// Initializes infrastructure folder and config if missing
void BackupService::initializeBackupRootIfNeeded() {
    const QString configFilePath = QDir(backupRootPath).filePath(
        QStringLiteral("%1/%2")
            .arg(Backup::Storage::k_BACKUP_SETUP_FOLDER,
                 Backup::Storage::k_BACKUP_SETUP_INFO_FILE));

    if (QFile::exists(configFilePath)) return;

    QString errorMessage;
    if (!FileOperations::createBackupInfrastructure(backupRootPath, errorMessage)) return;

    QJsonObject backupData{
        {"location", backupRootPath},
        {"timestamp", QDateTime::currentDateTimeUtc().toString(Qt::ISODate)}
    };

    QJsonObject backupConfig{
        {"app_name", App::Info::k_NAME},
        {"app_author", App::Info::k_AUTHOR_NAME},
        {"app_version", App::Info::k_VERSION},
        {"backup", backupData}
    };

    JsonManager::saveJsonFile(configFilePath, backupConfig);
}

// Scans backup directory and returns its status
BackupScanResult BackupService::scanForBackupStatus() const {
    BackupScanResult result;

    const QDir rootDir(backupRootPath);
    const QDir configDir(rootDir.filePath(Backup::Storage::k_BACKUP_SETUP_FOLDER));
    const QString setupInfoPath = configDir.filePath(Backup::Storage::k_BACKUP_SETUP_INFO_FILE);
    const QString logsFolderPath = configDir.filePath(Backup::Storage::k_BACKUP_LOGS_FOLDER);

    const bool hasSetupFolder = configDir.exists();
    const bool hasSetupInfo = QFile::exists(setupInfoPath);
    const bool hasLogsFolder = QDir(logsFolderPath).exists();

    if (!hasSetupFolder && !hasSetupInfo && !hasLogsFolder) {
        result.structureExists = false;
        result.validStructure = true;
        return result;
    }

    result.structureExists = true;
    result.validStructure = hasSetupInfo && hasLogsFolder;

    QSet<QString> logsSeen;
    QDir logsDir(logsFolderPath);
    const QFileInfoList logFiles = logsDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    for (const QFileInfo& logFile : logFiles) {
        const QString backupName = logFile.fileName().section("_" + Backup::Storage::k_BACKUP_LOGS_FILE, 0, 0);
        logsSeen.insert(backupName);

        const QString expectedFolder = rootDir.filePath(backupName);
        if (!QDir(expectedFolder).exists()) {
            result.hasOrphanedLogs = true;
        }
    }

    const QString prefix = ServiceDirector::getInstance().getBackupPrefix();
    const QFileInfoList backupFolders = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo& dirInfo : backupFolders) {
        const QString name = dirInfo.fileName();
        if (name.startsWith(prefix) && !logsSeen.contains(name)) {
            result.hasMissingLogs = true;
        }
    }

    return result;
}

// Returns metadata of the most recent backup
QJsonObject BackupService::getLastBackupMetadata() const {
    const QDir logsDir(QDir(backupRootPath).filePath(
        QStringLiteral("%1/%2")
            .arg(Backup::Storage::k_BACKUP_SETUP_FOLDER,
                 Backup::Storage::k_BACKUP_LOGS_FOLDER)));

    QFileInfoList logFiles = logsDir.entryInfoList(
        {"*_" + Backup::Storage::k_BACKUP_LOGS_FILE},
        QDir::Files, QDir::Time);

    if (logFiles.isEmpty()) return {};

    QJsonObject metadata;
    if (JsonManager::loadJsonFile(logFiles.first().absoluteFilePath(), metadata)) {
        return metadata;
    }

    return {};
}

// Returns the number of backup logs
int BackupService::getBackupCount() const {
    const QString logsFolderPath = QDir(backupRootPath).filePath(
        QStringLiteral("%1/%2")
            .arg(Backup::Storage::k_BACKUP_SETUP_FOLDER,
                 Backup::Storage::k_BACKUP_LOGS_FOLDER));

    return QDir(logsFolderPath).entryList(
                                   {"*_" + Backup::Storage::k_BACKUP_LOGS_FILE},
                                   QDir::Files).size();
}

// Returns total backup size across all logs
quint64 BackupService::getTotalBackupSize() const {
    quint64 totalSize = 0;

    const QDir logsDir(QDir(backupRootPath).filePath(
        QStringLiteral("%1/%2")
            .arg(Backup::Storage::k_BACKUP_SETUP_FOLDER,
                 Backup::Storage::k_BACKUP_LOGS_FOLDER)));

    const QFileInfoList logFiles = logsDir.entryInfoList(
        {"*_" + Backup::Storage::k_BACKUP_LOGS_FILE},
        QDir::Files);

    for (const QFileInfo& logFile : logFiles) {
        QJsonObject metadata;
        if (JsonManager::loadJsonFile(logFile.absoluteFilePath(), metadata)) {
            totalSize += metadata.value(Backup::Metadata::k_SIZE_BYTES)
            .toVariant().toULongLong();
        }
    }

    return totalSize;
}

// Calculates total size of selected files and folders
qint64 BackupService::calculateTotalBackupSize(const QStringList& items) const {
    qint64 totalSize = 0;
    for (const QString& item : items) {
        QFileInfo fileInfo(item);
        totalSize += fileInfo.isDir()
                         ? FileOperations::calculateDirectorySize(QDir(item))
                         : fileInfo.size();
    }
    return totalSize;
}

// Creates and saves summary metadata for a backup
void BackupService::createBackupSummary(const QString& backupFolderPath,
                                        const QStringList& selectedItems,
                                        qint64 backupDuration) {
    const QString logsFolderPath = QDir(backupRootPath).filePath(
        QStringLiteral("%1/%2")
            .arg(Backup::Storage::k_BACKUP_SETUP_FOLDER,
                 Backup::Storage::k_BACKUP_LOGS_FOLDER));

    if (!QDir(logsFolderPath).exists()) {
        QDir().mkpath(logsFolderPath);
    }

    QString logFileName = QFileInfo(backupFolderPath).fileName() + "_" +
                          Backup::Storage::k_BACKUP_LOGS_FILE;

    JsonManager::saveJsonFile(QDir(logsFolderPath).filePath(logFileName),
                              createBackupMetadata(backupFolderPath, selectedItems, backupDuration));
}

// Builds metadata object for a backup session
QJsonObject BackupService::createBackupMetadata(const QString& backupFolderPath,
                                                const QStringList& selectedItems,
                                                qint64 backupDuration) const {
    QJsonArray filesArray, foldersArray, userItemsArray;
    QSet<QString> uniqueFiles, uniqueFolders;

    for (const QString& item : selectedItems) {
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
        {Backup::Metadata::k_NAME, QFileInfo(backupFolderPath).fileName()},
        {Backup::Metadata::k_TIMESTAMP, Shared::Formatting::formatTimestamp(QDateTime::currentDateTime(), Qt::ISODate)},
        {Backup::Metadata::k_DURATION, backupDuration},
        {Backup::Metadata::k_DURATION_READABLE, Shared::Formatting::formatDuration(backupDuration)},
        {Backup::Metadata::k_SIZE_BYTES, totalSize},
        {Backup::Metadata::k_SIZE_READABLE, Shared::Formatting::formatSize(totalSize)},
        {Backup::Metadata::k_FILE_COUNT, filesArray.size()},
        {Backup::Metadata::k_FOLDER_COUNT, uniqueFolders.size()},
        {Backup::Metadata::k_USER_SELECTED_ITEMS, userItemsArray},
        {Backup::Metadata::k_USER_SELECTED_ITEM_COUNT, selectedItems.size()},
        {Backup::Metadata::k_BACKUP_FILES, filesArray},
        {Backup::Metadata::k_BACKUP_FOLDERS, foldersArray}
    };
}
