// Project includes
#include "transferworker.h"
#include "../constants/backupconstants.h"
#include "../service/fileoperations.h"

// Qt includes
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStorageInfo>

// Constructor
TransferWorker::TransferWorker(const QStringList& files,
                               const QString& destination,
                               QObject* parent)
    : QObject(parent), files(files), destination(destination) {}

// Stop transfer
void TransferWorker::stopTransfer() {
    stopRequested.store(true);
}

// Check if stop was requested
bool TransferWorker::shouldStop() {
    if (stopRequested.load()) {
        emit errorOccurred(Backup::Errors::Messages[Backup::ErrorCode::OperationStillRunning]);
        return true;
    }
    return false;
}

// Start transfer
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
            emit errorOccurred(
                Backup::Errors::Messages[Backup::ErrorCode::FileAccessDenied].arg(filePath));
            emit removeFromStaging(filePath);
            emit finished();
            return;
        }

        const bool isDriveRoot = fileInfo.isDir() &&
                                 filePath.endsWith(Backup::Drive::RootSuffix);

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

// Process drive root
bool TransferWorker::processDriveRoot(const QString& driveRoot) {
    if (shouldStop()) return false;

    const QFileInfo fileInfo(driveRoot);
    if (!fileInfo.isDir() || !fileInfo.exists()) {
        emit errorOccurred(
            Backup::Errors::Messages[Backup::ErrorCode::NoItemsSelected].arg(driveRoot));
        return false;
    }

    const QStorageInfo storageInfo(driveRoot);
    const QString driveName = storageInfo.displayName().isEmpty()
                                  ? Backup::Drive::DefaultLabel
                                  : storageInfo.displayName();

    const QString driveBackupFolder = QDir(destination).filePath(
        Backup::Drive::FolderFormat.arg(driveName,
                                        driveRoot.left(1),
                                        Backup::Drive::LabelSuffix));

    QDir backupDir(driveBackupFolder);
    if (backupDir.exists() && !backupDir.removeRecursively()) {
        emit errorOccurred(Backup::Errors::Messages[Backup::ErrorCode::CreateBackupFolder]);
        return false;
    }

    if (!backupDir.mkpath(".")) {
        emit errorOccurred(Backup::Errors::Messages[Backup::ErrorCode::CreateBackupFolder]);
        return false;
    }

    const QFileInfoList entries = QDir(driveRoot).entryInfoList(Backup::Transfer::EntryFilter);
    for (const QFileInfo& entry : entries) {
        if (!copyItem(entry, backupDir.filePath(entry.fileName()))) {
            return false;
        }
    }

    return true;
}

// Process file or folder
bool TransferWorker::processFileOrFolder(const QString& filePath) {
    if (shouldStop()) return false;

    const QFileInfo fileInfo(filePath);
    const QString destPath = QDir(destination).filePath(fileInfo.fileName());

    return copyItem(fileInfo, destPath);
}

// Copy item
bool TransferWorker::copyItem(const QFileInfo& fileInfo, const QString& destinationPath) {
    if (!fileInfo.isReadable()) {
        emit errorOccurred(
            Backup::Errors::Messages[Backup::ErrorCode::FileAccessDenied].arg(fileInfo.absoluteFilePath()));
        return false;
    }

    QFile destFile(destinationPath);
    if (destFile.exists() && !destFile.remove()) {
        emit errorOccurred(
            Backup::Errors::Messages[Backup::ErrorCode::TransferFailed].arg(destinationPath));
        return false;
    }

    const bool success = fileInfo.isDir()
                             ? FileOperations::copyDirectoryRecursively(fileInfo.absoluteFilePath(),
                                                                        destinationPath)
                             : QFile::copy(fileInfo.absoluteFilePath(), destinationPath);

    if (!success) {
        emit errorOccurred(
            Backup::Errors::Messages[Backup::ErrorCode::TransferFailed].arg(fileInfo.absoluteFilePath()));
        return false;
    }

    return true;
}
