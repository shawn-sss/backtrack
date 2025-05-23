// Project includes
#include "transferworker.h"
#include "../../shared/fileoperations.h"
#include "../../../ui/mainwindow/mainwindowmessages.h"
#include "../../../constants/backup_config.h"

// Qt includes
#include <QDir>
#include <QFile>
#include <QStorageInfo>

TransferWorker::TransferWorker(const QStringList& files, const QString& destination, QObject* parent)
    : QObject(parent), files(files), destination(destination) {}

// Stop the transfer process externally
void TransferWorker::stopTransfer() {
    stopRequested.store(true);
}

// Check if a stop has been requested
bool TransferWorker::shouldStop() {
    if (stopRequested.load()) {
        emit errorOccurred(WarningMessages::k_WARNING_OPERATION_STILL_RUNNING);
        return true;
    }
    return false;
}

// Start the file or folder transfer process
void TransferWorker::startTransfer() {
    if (files.isEmpty()) {
        emit transferComplete();
        emit finished();
        return;
    }

    const int totalFiles = files.size();
    int completedFiles = 0;

    for (const QString& filePath : std::as_const(files)) {
        if (shouldStop()) {
            emit finished();
            return;
        }

        const QFileInfo fileInfo(filePath);
        if (!fileInfo.exists() || !fileInfo.isReadable()) {
            emit errorOccurred(QString(ErrorMessages::k_ERROR_FILE_ACCESS_DENIED).arg(filePath));
            emit removeFromStaging(filePath);
            emit finished();
            return;
        }

        const bool isDriveRoot = fileInfo.isDir() && filePath.endsWith(":/");
        const bool success = isDriveRoot
                                 ? processDriveRoot(filePath)
                                 : processFileOrFolder(filePath);

        if (!success) {
            emit finished();
            return;
        }

        emit progressUpdated(++completedFiles * 100 / totalFiles);
    }

    emit transferComplete();
    emit finished();
}

// Handle copying when the source is a drive root
bool TransferWorker::processDriveRoot(const QString& driveRoot) {
    if (shouldStop()) return false;

    const QFileInfo fileInfo(driveRoot);
    if (!fileInfo.isDir() || !fileInfo.exists()) {
        emit errorOccurred(QString(ErrorMessages::k_ERROR_NO_ITEMS_SELECTED_FOR_BACKUP).arg(driveRoot));
        return false;
    }

    const QStorageInfo storageInfo(driveRoot);
    const QString driveName = storageInfo.displayName().isEmpty()
                                  ? Backup::Drive::k_DEFAULT_DRIVE_LABEL
                                  : storageInfo.displayName();

    const QString driveBackupFolder = QDir(destination).filePath(
        QStringLiteral("%1 (%2 %3)")
            .arg(driveName, driveRoot.left(1), Backup::Drive::k_DRIVE_LABEL_SUFFIX));

    QDir backupDir(driveBackupFolder);
    if (backupDir.exists() && !backupDir.removeRecursively()) {
        emit errorOccurred(ErrorMessages::k_ERROR_CREATE_BACKUP_FOLDER);
        return false;
    }

    if (!backupDir.mkpath(".")) {
        emit errorOccurred(ErrorMessages::k_ERROR_CREATE_BACKUP_FOLDER);
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

// Handle copying a single file or directory
bool TransferWorker::processFileOrFolder(const QString& filePath) {
    if (shouldStop()) return false;

    const QFileInfo fileInfo(filePath);
    const QString destPath = QDir(destination).filePath(fileInfo.fileName());

    return copyItem(fileInfo, destPath);
}

// Copy a file or directory to a destination
bool TransferWorker::copyItem(const QFileInfo& fileInfo, const QString& destinationPath) {
    if (!fileInfo.isReadable()) {
        emit errorOccurred(QString(ErrorMessages::k_ERROR_FILE_ACCESS_DENIED).arg(fileInfo.absoluteFilePath()));
        return false;
    }

    QFile destFile(destinationPath);
    if (destFile.exists() && !destFile.remove()) {
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
