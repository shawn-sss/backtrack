#include "transferworker.h"
#include "fileoperations.h"
#include "../../core/utils/constants.h"

#include <QStringList>
#include <QFile>
#include <QDir>
#include <QStorageInfo>

// Constructor
TransferWorker::TransferWorker(const QStringList &files,
                               const QString &destination,
                               QObject *parent)
    : QObject(parent), files(files), destination(destination) {}

// Start the file transfer process
void TransferWorker::startTransfer() {
    int totalFiles = files.size();
    int completedFiles = 0;

    for (const QString &filePath : files) {
        QFileInfo fileInfo(filePath);

        bool success = fileInfo.isDir() && filePath.endsWith(":/")
                           ? processDriveRoot(filePath)  // Handle drive roots
                           : processFileOrFolder(filePath);  // Handle files and folders

        if (!success) {
            return;  // Stop the transfer process on failure
        }

        completedFiles++;
        int progress = static_cast<int>((static_cast<double>(completedFiles) / totalFiles) * 100);
        emit progressUpdated(progress);
    }

    emit transferComplete();
}

// Handle drive root transfers (e.g., D:/)
bool TransferWorker::processDriveRoot(const QString &driveRoot) {
    QFileInfo fileInfo(driveRoot);
    if (!fileInfo.exists() || !fileInfo.isDir()) {
        emit errorOccurred(QString(UIConfig::ERROR_INVALID_SELECTION).arg(driveRoot));
        return false;
    }

    QString driveLetter = driveRoot.left(1);
    QStorageInfo storageInfo(driveRoot);
    QString driveLabel = storageInfo.displayName().isEmpty()
                             ? BackupInfo::DEFAULT_DRIVE_LABEL
                             : storageInfo.displayName();

    QString driveName = QString("%1 (%2)").arg(driveLabel, driveLetter);
    QString driveBackupFolder = QDir(destination).filePath(driveName);

    // Ensure a clean backup folder
    QDir dir(driveBackupFolder);
    if (dir.exists()) {
        dir.removeRecursively();
    }

    if (!QDir().mkpath(driveBackupFolder)) {
        emit errorOccurred(BackupInfo::ERROR_BACKUP_FOLDER_CREATION_FAILED);
        return false;
    }

    // Copy drive contents
    QDir driveDir(driveRoot);
    QFileInfoList entries = driveDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    for (const QFileInfo &entry : entries) {
        QString destPath = QDir(driveBackupFolder).filePath(entry.fileName());

        bool success = entry.isDir()
                           ? FileOperations::copyDirectoryRecursively(entry.absoluteFilePath(), destPath)
                           : QFile::copy(entry.absoluteFilePath(), destPath);

        if (!success) {
            emit errorOccurred(QString(UIConfig::ERROR_TRANSFER_FAILED).arg(entry.absoluteFilePath()));
            return false;
        }
    }

    return true;
}

// Handle file or folder transfers
bool TransferWorker::processFileOrFolder(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    QString destinationPath = QDir(destination).filePath(fileInfo.fileName());

    bool success = fileInfo.isDir()
                       ? FileOperations::copyDirectoryRecursively(filePath, destinationPath)
                       : QFile::copy(filePath, destinationPath);

    if (!success) {
        emit errorOccurred(QString(UIConfig::ERROR_TRANSFER_FAILED).arg(filePath));
        return false;
    }

    return true;
}
