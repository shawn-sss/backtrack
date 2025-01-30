#include "transferworker.h"
#include "fileoperations.h"
#include "../Utils/constants.h"

#include <QStringList>
#include <QFile>
#include <QDir>

// Constructor
TransferWorker::TransferWorker(const QStringList &files,
                               const QString &destination,
                               QObject *parent)
    : QObject(parent), files(files), destination(destination) {}

// Start the file transfer process
void TransferWorker::startTransfer() {
    int totalFiles = files.size();
    int completedFiles = 0;

    // Iterate through each file or directory
    for (int i = 0; i < totalFiles; ++i) {
        const QString &filePath = files.at(i);
        QFileInfo fileInfo(filePath);
        QString destinationPath = QDir(destination).filePath(fileInfo.fileName());

        // Copy directory or file to the destination
        bool success = fileInfo.isDir()
                           ? FileOperations::copyDirectoryRecursively(filePath, destinationPath)
                           : QFile::copy(filePath, destinationPath);

        if (!success) {
            // Emit error signal if transfer fails
            emit errorOccurred(QString(Constants::ERROR_TRANSFER_FAILED).arg(filePath));
            return;
        }

        // Update progress
        completedFiles++;
        int progress = static_cast<int>(
            (static_cast<double>(completedFiles) / totalFiles) * 100
            );
        emit progressUpdated(progress);
    }

    // Emit completion signal when all transfers are successful
    emit transferComplete();
}
