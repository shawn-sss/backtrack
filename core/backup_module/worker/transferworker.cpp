#include "transferworker.h"

#include "../../../config/_constants.h"
#include "../../utils/file_utils/fileoperations.h"

#include <QDir>
#include <QStringList>
#include <QStorageInfo>
#include <QFile>

// Constructor
TransferWorker::TransferWorker(const QStringList &files, const QString &destination, QObject *parent)
    : QObject(parent), files(files), destination(destination), stopRequested(false) {}

// Stops the transfer process
void TransferWorker::stopTransfer() {
    stopRequested = true;
}

// Starts the transfer process
void TransferWorker::startTransfer() {
    if (files.isEmpty()) {
        emit transferComplete();
        emit finished();
        return;
    }

    int totalFiles = files.size();
    int completedFiles = 0;

    for (int i = 0; i < totalFiles; ++i) {
        if (stopRequested) {
            emit errorOccurred(WarningMessages::WARNING_OPERATION_STILL_RUNNING);
            return;
        }

        const QString &filePath = files.at(i);
        QFileInfo fileInfo(filePath);

        bool success = fileInfo.isDir() && filePath.endsWith(":/")
                           ? processDriveRoot(filePath)
                           : processFileOrFolder(filePath);

        if (!success) return;

        emit progressUpdated(((++completedFiles) * 100) / totalFiles);
    }

    emit transferComplete();
    emit finished();
}

// Processes a drive root (handling full drive backup)
bool TransferWorker::processDriveRoot(const QString &driveRoot) {
    if (stopRequested) return false;

    QFileInfo fileInfo(driveRoot);
    if (!fileInfo.exists() || !fileInfo.isDir()) {
        emit errorOccurred(QString(ErrorMessages::ERROR_NO_ITEMS_SELECTED_FOR_BACKUP).arg(driveRoot));
        return false;
    }

    QStorageInfo storageInfo(driveRoot);
    QString driveName = storageInfo.displayName().isEmpty()
                            ? BackupConfiguration::DEFAULT_DRIVE_LABEL
                            : storageInfo.displayName();
    QString driveBackupFolder = QDir(destination).filePath(
        QString("%1 (%2 %3)").arg(driveName, driveRoot.left(1), BackupConfiguration::DRIVE_LABEL_SUFFIX));

    QDir backupDir(driveBackupFolder);
    if (backupDir.exists()) {
        backupDir.removeRecursively();
    }

    if (!QDir().mkpath(driveBackupFolder)) {
        emit errorOccurred(ErrorMessages::ERROR_CREATING_BACKUP_FOLDER);
        return false;
    }

    QFileInfoList entries = QDir(driveRoot).entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    for (const QFileInfo &entry : entries) {
        if (!copyItem(entry, driveBackupFolder + QDir::separator() + entry.fileName())) {
            return false;
        }
    }

    return true;
}

// Processes a single file or folder
bool TransferWorker::processFileOrFolder(const QString &filePath) {
    return !stopRequested && copyItem(QFileInfo(filePath), QDir(destination).filePath(QFileInfo(filePath).fileName()));
}

// Copies a file or directory to the target path
bool TransferWorker::copyItem(const QFileInfo &fileInfo, const QString &destinationPath) {
    if (!fileInfo.isReadable()) {
        emit errorOccurred(QString(ErrorMessages::ERROR_FILE_ACCESS_DENIED).arg(fileInfo.absoluteFilePath()));
        return false;
    }

    QFile destinationFile(destinationPath);
    if (destinationFile.exists()) {
        if (!destinationFile.remove()) {
            emit errorOccurred(QString(ErrorMessages::ERROR_TRANSFER_FAILED).arg(destinationPath));
            return false;
        }
    }

    bool success = fileInfo.isDir()
                       ? FileOperations::copyDirectoryRecursively(fileInfo.absoluteFilePath(), destinationPath)
                       : QFile::copy(fileInfo.absoluteFilePath(), destinationPath);

    if (!success) {
        emit errorOccurred(QString(ErrorMessages::ERROR_TRANSFER_FAILED).arg(fileInfo.absoluteFilePath()));
        return false;
    }

    return true;
}
