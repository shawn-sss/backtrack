// Project includes
#include "backupservice.h"
#include "../../../core/backup/service/fileoperations.h"
#include "../../../../services/ServiceManagers/JsonServiceManager/JsonServiceManager.h"
#include "../../../../services/ServiceManagers/FormatUtilsServiceManager/FormatUtilsServiceManager.h"
#include "../../../../constants/kvp_info.h"
#include "../../../../constants/app_info.h"
#include "../../../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"
#include "../../../../services/ServiceDirector/ServiceDirector.h"

// Qt includes
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// Constructor
BackupService::BackupService(const QString& backupRoot, QObject* parent)
    : QObject(parent), backupRootPath(backupRoot) {}

// Sets the backup root path
void BackupService::setBackupRoot(const QString& path) {
    backupRootPath = path;
}

// Returns the backup root path
QString BackupService::getBackupRoot() const {
    return backupRootPath;
}

// Ensures backup infrastructure exists if it doesn't already
void BackupService::initializeBackupRootIfNeeded() {
    const QString configFilePath = PathServiceManager::backupInitMetadataFilePath();
    if (QFile::exists(configFilePath)) return;

    QString errorMessage;
    if (!FileOperations::createBackupInfrastructure(backupRootPath, errorMessage)) return;

    const QJsonObject backupData{
        {"location", backupRootPath},
        {"timestamp", QDateTime::currentDateTimeUtc().toString(Qt::ISODate)}
    };

    const QJsonObject backupConfig{
        {"app_name", App::Info::k_APP_NAME},
        {"app_author", App::Info::k_AUTHOR_NAME},
        {"app_version", App::Info::k_APP_VERSION},
        {"backup", backupData}
    };

    JsonManager::saveJsonFile(configFilePath, backupConfig);
}

// Scans and evaluates current backup structure and logs
BackupScanResult BackupService::scanForBackupStatus() const {
    BackupScanResult result;
    const QDir rootDir(backupRootPath);

    const QString appInitPath = PathServiceManager::appInitMetadataFilePath();
    const QString backupInitPath = PathServiceManager::backupInitMetadataFilePath();
    const QString logsFolderPath = PathServiceManager::backupLogsFolderPath();

    const bool hasAppInit = QFile::exists(appInitPath);
    const bool hasBackupInit = QFile::exists(backupInitPath);
    const bool hasLogsFolder = QDir(logsFolderPath).exists();

    result.validAppStructure = hasAppInit;
    result.validBackupStructure = hasBackupInit && hasLogsFolder;
    result.structureExists = hasBackupInit && hasLogsFolder;

    if (hasAppInit && !hasBackupInit && !hasLogsFolder) {
        result.validBackupStructure = true;
        return result;
    }

    QSet<QString> logsSeen;
    const QFileInfoList logFiles = getBackupLogFiles();

    for (const QFileInfo& logFile : logFiles) {
        const QString backupName = logFile.fileName().section("_" + App::Items::k_BACKUP_SETUP_CONFIG_LOGS_FILE, 0, 0);
        logsSeen.insert(backupName);

        if (!QDir(rootDir.filePath(backupName)).exists()) {
            result.hasOrphanedLogs = true;
        }
    }

    const QString prefix = ServiceDirector::getInstance().getBackupPrefix();
    const QFileInfoList backupFolders = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo& dirInfo : backupFolders) {
        const QString name = dirInfo.fileName();

        if (!name.startsWith(prefix)) continue;

        const QDateTime folderCreated = dirInfo.lastModified();
        const qint64 ageSecs = folderCreated.secsTo(QDateTime::currentDateTime());
        if (ageSecs < 5) continue;

        if (!logsSeen.contains(name)) {
            result.hasMissingLogs = true;
        }
    }

    return result;
}

// Returns list of backup log files, optionally sorted by time
QFileInfoList BackupService::getBackupLogFiles(bool sortedByTime) const {
    const QDir logsDir(PathServiceManager::backupLogsFolderPath());
    return logsDir.entryInfoList(
        {"*_" + App::Items::k_BACKUP_SETUP_CONFIG_LOGS_FILE},
        QDir::Files,
        sortedByTime ? QDir::Time : QDir::NoSort
        );
}

// Returns metadata from the most recent backup log file
QJsonObject BackupService::getLastBackupMetadata() const {
    const QFileInfoList logFiles = getBackupLogFiles(true);
    if (logFiles.isEmpty()) return {};

    QJsonObject metadata;
    if (JsonManager::loadJsonFile(logFiles.first().absoluteFilePath(), metadata)) {
        return metadata;
    }

    return {};
}

// Returns total count of existing backup logs
int BackupService::getBackupCount() const {
    return getBackupLogFiles().size();
}

// Computes total size of all recorded backups
quint64 BackupService::getTotalBackupSize() const {
    quint64 totalSize = 0;
    const QFileInfoList& logFiles = getBackupLogFiles();
    for (const QFileInfo& logFile : logFiles) {
        QJsonObject metadata;
        if (JsonManager::loadJsonFile(logFile.absoluteFilePath(), metadata)) {
            totalSize += metadata.value(App::KVP::BackupMetadata::k_SIZE_BYTES)
            .toVariant().toULongLong();
        }
    }
    return totalSize;
}

// Computes total size of a given set of paths
qint64 BackupService::calculateTotalBackupSize(const QStringList& items) const {
    qint64 totalSize = 0;
    for (const QString& item : items) {
        const QFileInfo fileInfo(item);
        totalSize += fileInfo.isDir()
                         ? FileOperations::calculateDirectorySize(QDir(item))
                         : fileInfo.size();
    }
    return totalSize;
}

// Creates and writes backup summary log file
void BackupService::createBackupSummary(const QString& backupFolderPath,
                                        const QStringList& selectedItems,
                                        qint64 backupDuration) {
    const QString logsFolderPath = PathServiceManager::backupLogsFolderPath();
    if (!QDir(logsFolderPath).exists()) {
        QDir().mkpath(logsFolderPath);
    }

    const QString logFilePath = PathServiceManager::backupLogFilePath(
        QFileInfo(backupFolderPath).fileName());

    const QJsonObject summary = createBackupMetadata(backupFolderPath, selectedItems, backupDuration);
    JsonManager::saveJsonFile(logFilePath, summary);

    emit backupSummaryWritten(logFilePath);
}

// Constructs JSON metadata object for backup summary
QJsonObject BackupService::createBackupMetadata(const QString& backupFolderPath,
                                                const QStringList& selectedItems,
                                                qint64 backupDuration) const {
    QJsonArray filesArray, foldersArray, userItemsArray;
    QSet<QString> uniqueFiles, uniqueFolders;

    for (const QString& item : selectedItems) {
        userItemsArray.append(item);
        const QFileInfo fileInfo(item);

        if (fileInfo.isDir()) {
            FileOperations::collectDirectoriesRecursively(item, uniqueFolders, foldersArray);
            FileOperations::collectFilesRecursively(item, uniqueFiles, filesArray);
        } else if (!uniqueFiles.contains(item)) {
            uniqueFiles.insert(item);
            filesArray.append(item);
        }
    }

    const qint64 totalSize = calculateTotalBackupSize(selectedItems);
    const QString folderName = QFileInfo(backupFolderPath).fileName();
    const QDateTime now = QDateTime::currentDateTime();

    return QJsonObject{
        {App::KVP::BackupMetadata::k_NAME, folderName},
        {App::KVP::BackupMetadata::k_TIMESTAMP, Shared::Formatting::formatTimestamp(now, Qt::ISODate)},
        {App::KVP::BackupMetadata::k_DURATION, backupDuration},
        {App::KVP::BackupMetadata::k_DURATION_READABLE, Shared::Formatting::formatDuration(backupDuration)},
        {App::KVP::BackupMetadata::k_SIZE_BYTES, totalSize},
        {App::KVP::BackupMetadata::k_SIZE_READABLE, Shared::Formatting::formatSize(totalSize)},
        {App::KVP::BackupMetadata::k_FILE_COUNT, filesArray.size()},
        {App::KVP::BackupMetadata::k_FOLDER_COUNT, uniqueFolders.size()},
        {App::KVP::BackupMetadata::k_USER_SELECTED_ITEMS, userItemsArray},
        {App::KVP::BackupMetadata::k_USER_SELECTED_ITEM_COUNT, selectedItems.size()},
        {App::KVP::BackupMetadata::k_BACKUP_FILES, filesArray},
        {App::KVP::BackupMetadata::k_BACKUP_FOLDERS, foldersArray}
    };
}
