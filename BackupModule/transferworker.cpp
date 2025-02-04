#include "transferworker.h"
#include "fileoperations.h"
#include "../Utils/constants.h"

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

    for (int i = 0; i < totalFiles; ++i) {
        const QString &filePath = files.at(i);
        QFileInfo fileInfo(filePath);

        // Check if the user selected a drive root (e.g., D:/)
        if (fileInfo.isDir() && filePath.endsWith(":/")) {
            // Extract the drive letter (e.g., "D") and label (e.g., "Staging")
            QString driveLetter = filePath.left(1); // "D"
            QStorageInfo storageInfo(filePath);
            QString driveLabel = storageInfo.displayName(); // Retrieve the drive label

            // Use "Local Disk" as fallback if no label is found
            if (driveLabel.isEmpty()) {
                driveLabel = "Local Disk";
            }

            // Construct folder name: "DriveLabel (DriveLetter)"
            QString driveName = QString("%1 (%2)").arg(driveLabel, driveLetter); // Avoid using ':' in folder name

            // Create a new folder inside the backup destination
            QString driveBackupFolder = QDir(destination).filePath(driveName);

            // Check and clean up any partially created folders
            if (QDir(driveBackupFolder).exists()) {
                QDir(driveBackupFolder).removeRecursively();
            }

            // Attempt to create the backup folder
            if (!QDir().mkpath(driveBackupFolder)) {
                emit errorOccurred(QString(Constants::ERROR_BACKUP_FOLDER_CREATION_FAILED));
                return;
            }

            // Copy all contents of the drive root (excluding the root itself)
            QDir driveDir(filePath);
            QFileInfoList entries = driveDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

            for (const QFileInfo &entry : entries) {
                QString destPath = QDir(driveBackupFolder).filePath(entry.fileName());

                bool success = entry.isDir()
                                   ? FileOperations::copyDirectoryRecursively(entry.absoluteFilePath(), destPath)
                                   : QFile::copy(entry.absoluteFilePath(), destPath);

                if (!success) {
                    emit errorOccurred(QString(Constants::ERROR_TRANSFER_FAILED).arg(entry.absoluteFilePath()));
                    return;
                }
            }
        } else {
            // Normal file/folder transfer
            QString destinationPath = QDir(destination).filePath(fileInfo.fileName());
            bool success = fileInfo.isDir()
                               ? FileOperations::copyDirectoryRecursively(filePath, destinationPath)
                               : QFile::copy(filePath, destinationPath);

            if (!success) {
                emit errorOccurred(QString(Constants::ERROR_TRANSFER_FAILED).arg(filePath));
                return;
            }
        }

        // Update progress
        completedFiles++;
        int progress = static_cast<int>((static_cast<double>(completedFiles) / totalFiles) * 100);
        emit progressUpdated(progress);
    }

    emit transferComplete();
}

