#include "backupcontroller.h"
#include "backupservice.h"
#include "transferworker.h"
#include "../Utils/constants.h"

#include <QThread>
#include <QProgressBar>
#include <QStringList>
#include <QDateTime>
#include <QDir>
#include <QFile>

// Constructor
BackupController::BackupController(BackupService *service, QObject *parent)
    : QObject(parent), backupService(service), workerThread(nullptr) {}

// Destructor
BackupController::~BackupController() {
    if (workerThread && workerThread->isRunning()) {
        workerThread->quit();
        workerThread->wait();
    }
    delete workerThread;
}

// Create a backup and manage the transfer process
void BackupController::createBackup(const QString &destinationPath,
                                    const QStringList &stagingList,
                                    QProgressBar *progressBar) {
    if (isBackupInProgress()) {
        emit errorOccurred(Constants::MESSAGE_BACKUP_IN_PROGRESS);
        return;
    }

    // Generate a timestamped backup folder name
    QString timestamp = QDateTime::currentDateTime()
                            .toString(Constants::BACKUP_FOLDER_TIMESTAMP_FORMAT);
    QString backupFolderName = QString(Constants::BACKUP_FOLDER_FORMAT)
                                   .arg(Constants::BACKUP_FOLDER_PREFIX, timestamp);
    QString backupFolderPath = QDir(destinationPath).filePath(backupFolderName);

    // Ensure the backup folder is created
    if (!QDir().mkpath(backupFolderPath)) {
        emit errorOccurred(Constants::ERROR_BACKUP_FOLDER_CREATION_FAILED);
        return;
    }

    // Initialize progress bar
    progressBar->setVisible(true);
    progressBar->setValue(Constants::PROGRESS_BAR_MIN_VALUE);

    // Capture the start time
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    // Setup worker and thread
    workerThread = new QThread(this);
    auto *worker = new TransferWorker(stagingList, backupFolderPath);
    worker->moveToThread(workerThread);

    // Connect signals for progress, completion, and errors
    connect(worker, &TransferWorker::progressUpdated, progressBar, &QProgressBar::setValue);
    connect(worker, &TransferWorker::transferComplete, this,
            [this, backupFolderPath, stagingList, startTime, progressBar]() {
                qint64 endTime = QDateTime::currentMSecsSinceEpoch();
                qint64 backupDuration = endTime - startTime;

                backupService->createBackupSummary(backupFolderPath, stagingList, backupDuration);

                progressBar->setValue(Constants::PROGRESS_BAR_MAX_VALUE);
                progressBar->setVisible(false);

                emit backupCreated();
                cleanupAfterTransfer();
            });
    connect(worker, &TransferWorker::errorOccurred, this,
            [this, progressBar](const QString &error) {
                progressBar->setVisible(false);
                emit errorOccurred(error);
                cleanupAfterTransfer();
            });

    // Start worker thread
    connect(workerThread, &QThread::started, worker, &TransferWorker::startTransfer);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);

    workerThread->start();
}

// Delete a backup folder and its metadata
void BackupController::deleteBackup(const QString &backupPath) {
    if (!QFile::exists(backupPath)) {
        emit errorOccurred(Constants::ERROR_INVALID_BACKUP_LOCATION);
        return;
    }

    QDir backupDir(backupPath);
    QString summaryFilePath = backupDir.filePath(Constants::BACKUP_SUMMARY_FILENAME);

    if (QFile::exists(summaryFilePath)) {
        QFile::remove(summaryFilePath);
    }

    if (!backupDir.removeRecursively()) {
        emit errorOccurred(Constants::ERROR_BACKUP_DELETION_FAILED);
        return;
    }

    emit backupDeleted();
}

// Check if a backup process is in progress
bool BackupController::isBackupInProgress() const {
    return workerThread && workerThread->isRunning();
}

// Clean up the worker thread after a transfer
void BackupController::cleanupAfterTransfer() {
    if (workerThread && workerThread->isRunning()) {
        workerThread->quit();
        workerThread->wait();
    }
    delete workerThread;
    workerThread = nullptr;
}
