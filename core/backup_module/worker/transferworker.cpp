// Project includes
#include "../../../config/configsettings/_settings.h"
#include "../../utils/file_utils/fileoperations.h"
#include "transferworker.h"

// Qt includes
#include <QDir>
#include <QFile>
#include <QStorageInfo>

// Constructor
TransferWorker::TransferWorker(const QStringList& files, const QString& destination, QObject* parent)
    : QObject(parent), files(files), destination(destination) {}

// Request transfer to stop early
void TransferWorker::stopTransfer() {
    stopRequested.store(true);
}

// Start file/folder transfer in sequence
void TransferWorker::startTransfer() {
    if (files.isEmpty()) {
        emit transferComplete();
        emit finished();
        return;
    }

    const int totalFiles = files.size();
    int completedFiles = 0;

    for (const QString& filePath : std::as_const(files)) {
        if (stopRequested.load()) {
            emit errorOccurred(WarningMessages::k_WARNING_OPERATION_STILL_RUNNING);
            emit finished();
            return;
        }

        const QFileInfo fileInfo(filePath);
        bool success = fileInfo.isDir() && filePath.endsWith(":/")
                           ? processDriveRoot(filePath)
                           : processFileOrFolder(filePath);

        if (!success) {
            emit finished();
            return;
        }

        emit progressUpdated((++completedFiles * 100) / totalFiles);
    }

    emit transferComplete();
    emit finished();
}

// Copy the contents of a full drive (e.g., C:/)
bool TransferWorker::processDriveRoot(const QString& driveRoot) {
    if (stopRequested.load()) return false;

    const QFileInfo fileInfo(driveRoot);
    if (!fileInfo.isDir() || !fileInfo.exists()) {
        emit errorOccurred(QString(ErrorMessages::k_ERROR_NO_ITEMS_SELECTED_FOR_BACKUP).arg(driveRoot));
        return false;
    }

    const QStorageInfo storageInfo(driveRoot);
    const QString driveName = storageInfo.displayName().isEmpty()
                                  ? Backup::DriveConfig::k_DEFAULT_DRIVE_LABEL
                                  : storageInfo.displayName();

    const QString driveBackupFolder = QDir(destination).filePath(
        QStringLiteral("%1 (%2 %3)")
            .arg(driveName, driveRoot.left(1), Backup::DriveConfig::k_DRIVE_LABEL_SUFFIX));

    QDir backupDir(driveBackupFolder);
    if (backupDir.exists() && !backupDir.removeRecursively()) {
        emit errorOccurred(ErrorMessages::k_ERROR_CREATING_BACKUP_FOLDER);
        return false;
    }

    if (!backupDir.mkpath(".")) {
        emit errorOccurred(ErrorMessages::k_ERROR_CREATING_BACKUP_FOLDER);
        return false;
    }

    const QFileInfoList entries = QDir(driveRoot).entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    for (const QFileInfo& entry : entries) {
        if (!copyItem(entry, backupDir.filePath(entry.fileName()))) {
            return false;
        }
    }

    return true;
}

// Copy an individual file or folder
bool TransferWorker::processFileOrFolder(const QString& filePath) {
    if (stopRequested.load()) return false;

    const QFileInfo fileInfo(filePath);
    const QString destPath = QDir(destination).filePath(fileInfo.fileName());

    return copyItem(fileInfo, destPath);
}

// Copy a file or recursively copy a directory
bool TransferWorker::copyItem(const QFileInfo& fileInfo, const QString& destinationPath) {
    if (!fileInfo.isReadable()) {
        emit errorOccurred(QString(ErrorMessages::k_ERROR_FILE_ACCESS_DENIED).arg(fileInfo.absoluteFilePath()));
        return false;
    }

    QFile destinationFile(destinationPath);
    if (destinationFile.exists() && !destinationFile.remove()) {
        emit errorOccurred(QString(ErrorMessages::k_ERROR_TRANSFER_FAILED).arg(destinationPath));
        return false;
    }

    const bool success = fileInfo.isDir()
                             ? FileOperations::copyDirectoryRecursively(fileInfo.absoluteFilePath(), destinationPath)
                             : QFile::copy(fileInfo.absoluteFilePath(), destinationPath);

    if (!success) {
        emit errorOccurred(QString(ErrorMessages::k_ERROR_TRANSFER_FAILED).arg(fileInfo.absoluteFilePath()));
        return false;
    }

    return true;
}
