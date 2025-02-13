#include "transferworker.h"
#include "../../utils/file_utils/fileoperations.h"
#include "../../config/constants.h"

#include <QStringList>
#include <QFile>
#include <QDir>
#include <QStorageInfo>

// Constructor & Initialization
TransferWorker::TransferWorker(const QStringList &files, const QString &destination, QObject *parent)
    : QObject(parent), files(files), destination(destination) {}

// Transfer Control
void TransferWorker::stopTransfer() {
    stopRequested = true;
}

void TransferWorker::startTransfer() {
    int totalFiles = files.size();
    if (totalFiles == 0) {
        emit transferComplete();
        emit finished();
        return;
    }

    int completedFiles = 0;
    for (const QString &filePath : files) {
        if (stopRequested) {
            emit errorOccurred("Transfer stopped by user.");
            return;
        }

        QFileInfo fileInfo(filePath);
        bool success = fileInfo.isDir() && filePath.endsWith(":/")
                           ? processDriveRoot(filePath)
                           : processFileOrFolder(filePath);

        if (!success) {
            return;
        }

        completedFiles++;
        emit progressUpdated((completedFiles * 100) / totalFiles);
    }

    emit transferComplete();
    emit finished();
}

// File Processing
bool TransferWorker::processDriveRoot(const QString &driveRoot) {
    if (stopRequested) return false;

    QFileInfo fileInfo(driveRoot);
    if (!fileInfo.exists() || !fileInfo.isDir()) {
        emit errorOccurred(QString(UIConfig::ERROR_INVALID_SELECTION).arg(driveRoot));
        return false;
    }

    QString driveLetter = driveRoot.left(1);
    QStorageInfo storageInfo(driveRoot);
    QString driveLabel = storageInfo.displayName().isEmpty() ? "Local Disk" : storageInfo.displayName();

    QString driveName = QString("%1 (%2 Drive)").arg(driveLabel, driveLetter);

    QString driveBackupFolder = QDir(destination).filePath(driveName);

    QDir dir(driveBackupFolder);
    if (dir.exists()) {
        dir.removeRecursively();
    }

    if (!QDir().mkpath(driveBackupFolder)) {
        emit errorOccurred(BackupInfo::ERROR_BACKUP_FOLDER_CREATION_FAILED);
        return false;
    }

    QDir driveDir(driveRoot);
    QFileInfoList entries = driveDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    if (stopRequested) return false;

    for (const QFileInfo &entry : entries) {
        QString destPath = driveBackupFolder + QDir::separator() + entry.fileName();
        if (!copyItem(entry, destPath)) {
            return false;
        }
    }

    return true;
}

bool TransferWorker::processFileOrFolder(const QString &filePath) {
    if (stopRequested) return false;

    QFileInfo fileInfo(filePath);
    QString destinationPath = QDir(destination).filePath(fileInfo.fileName());

    return copyItem(fileInfo, destinationPath);
}

// Helper Methods
bool TransferWorker::copyItem(const QFileInfo &fileInfo, const QString &destinationPath) {
    if (!fileInfo.isReadable()) {
        emit errorOccurred(QString("File is locked or cannot be accessed: %1").arg(fileInfo.absoluteFilePath()));
        return false;
    }

    if (QFile::exists(destinationPath) && !QFile::remove(destinationPath)) {
        emit errorOccurred(QString("Unable to overwrite existing file: %1").arg(destinationPath));
        return false;
    }

    bool success = fileInfo.isDir()
                       ? FileOperations::copyDirectoryRecursively(fileInfo.absoluteFilePath(), destinationPath)
                       : QFile::copy(fileInfo.absoluteFilePath(), destinationPath);

    if (!success) {
        emit errorOccurred(QString("Unable to transfer: %1").arg(fileInfo.absoluteFilePath()));
        return false;
    }

    return true;
}
