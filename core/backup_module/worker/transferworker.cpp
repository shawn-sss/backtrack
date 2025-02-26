#include "transferworker.h"
#include "../../../config/_constants.h"

#include "../../utils/file_utils/fileoperations.h"

#include <QDir>
#include <QStringList>
#include <QStorageInfo>
#include <QFile>

// Constructor
TransferWorker::TransferWorker(const QStringList &files, const QString &destination, QObject *parent)
    : QObject(parent), files(files), destination(destination) {}

// Transfer Control
void TransferWorker::stopTransfer() {
    stopRequested = true;
}

void TransferWorker::startTransfer() {
    if (files.isEmpty()) {
        emit transferComplete();
        emit finished();
        return;
    }

    int completedFiles = 0;
    for (const QString &filePath : files) {
        if (stopRequested) {
            emit errorOccurred(WarningMessages::WARNING_OPERATION_STILL_RUNNING);
            return;
        }

        bool success = QFileInfo(filePath).isDir() && filePath.endsWith(":/")
                           ? processDriveRoot(filePath)
                           : processFileOrFolder(filePath);

        if (!success) return;

        emit progressUpdated((++completedFiles * 100) / files.size());
    }

    emit transferComplete();
    emit finished();
}

bool TransferWorker::processDriveRoot(const QString &driveRoot) {
    if (stopRequested) return false;

    QFileInfo fileInfo(driveRoot);
    if (!fileInfo.exists() || !fileInfo.isDir()) {
        emit errorOccurred(QString(ErrorMessages::ERROR_NO_ITEMS_SELECTED_FOR_BACKUP).arg(driveRoot));
        return false;
    }

    QString driveBackupFolder = QDir(destination).filePath(QString("%1 (%2 %3)")
                                                               .arg(QStorageInfo(driveRoot).displayName().isEmpty() ? BackupConfiguration::DEFAULT_DRIVE_LABEL : QStorageInfo(driveRoot).displayName(),
                                                                    driveRoot.left(1),
                                                                    BackupConfiguration::DRIVE_LABEL_SUFFIX));

    if (QDir(driveBackupFolder).exists()) QDir(driveBackupFolder).removeRecursively();
    if (!QDir().mkpath(driveBackupFolder)) {
        emit errorOccurred(ErrorMessages::ERROR_CREATING_BACKUP_FOLDER);
        return false;
    }

    QFileInfoList entries = QDir(driveRoot).entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    for (int i = 0; i < entries.size(); ++i) {
        const QFileInfo &entry = entries.at(i);
        if (!copyItem(entry, driveBackupFolder + QDir::separator() + entry.fileName())) return false;
    }

    return true;
}

bool TransferWorker::processFileOrFolder(const QString &filePath) {
    return !stopRequested && copyItem(QFileInfo(filePath), QDir(destination).filePath(QFileInfo(filePath).fileName()));
}

// Helper Methods
bool TransferWorker::copyItem(const QFileInfo &fileInfo, const QString &destinationPath) {
    if (!fileInfo.isReadable()) {
        emit errorOccurred(QString(ErrorMessages::ERROR_FILE_ACCESS_DENIED).arg(fileInfo.absoluteFilePath()));
        return false;
    }

    if (QFile::exists(destinationPath) && !QFile::remove(destinationPath)) {
        emit errorOccurred(QString(ErrorMessages::ERROR_TRANSFER_FAILED).arg(destinationPath));
        return false;
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
