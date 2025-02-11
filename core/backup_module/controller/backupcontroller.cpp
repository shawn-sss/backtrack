#include "backupcontroller.h"
#include "../service/backupservice.h"
#include "../worker/transferworker.h"
#include "../../config/constants.h"

#include <QThread>
#include <QProgressBar>
#include <QStringList>
#include <QDateTime>
#include <QDir>
#include <QFile>

// Constructor and Destructor
BackupController::BackupController(BackupService *service, QObject *parent)
    : QObject(parent), backupService(service), workerThread(nullptr) {}

BackupController::~BackupController() {
    cleanupAfterTransfer();
}

// Helper Methods
bool BackupController::createBackupFolder(const QString &path) {
    if (!QDir().mkpath(path)) {
        emit errorOccurred(BackupInfo::ERROR_BACKUP_FOLDER_CREATION_FAILED);
        return false;
    }
    return true;
}

// Backup Operations
void BackupController::createBackup(const QString &destinationPath,
                                    const QStringList &stagingList,
                                    QProgressBar *progressBar) {
    if (isBackupInProgress()) {
        emit errorOccurred(UIConfig::MESSAGE_BACKUP_IN_PROGRESS);
        return;
    }

    // Generate timestamped backup folder name
    QString timestamp = QDateTime::currentDateTime()
                            .toString(BackupInfo::BACKUP_FOLDER_TIMESTAMP_FORMAT);
    QString backupFolderName = QString(BackupInfo::BACKUP_FOLDER_FORMAT)
                                   .arg(UserSettings::BACKUP_FOLDER_PREFIX, timestamp);
    QString backupFolderPath = QDir(destinationPath).filePath(backupFolderName);

    // Ensure backup folder is created
    if (!createBackupFolder(backupFolderPath)) {
        return;
    }

    // Initialize progress bar
    progressBar->setVisible(true);
    progressBar->setValue(ProgressConfig::MIN_VALUE);

    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    // Create and start worker thread for file transfer
    workerThread = new QThread(this);
    auto *worker = new TransferWorker(stagingList, backupFolderPath);
    worker->moveToThread(workerThread);

    // Connect worker signals to manage progress and completion
    connect(worker, &TransferWorker::progressUpdated, progressBar, &QProgressBar::setValue);
    connect(worker, &TransferWorker::transferComplete, this,
            [this, backupFolderPath, stagingList, startTime, progressBar]() {
                qint64 endTime = QDateTime::currentMSecsSinceEpoch();
                qint64 backupDuration = endTime - startTime;

                // Create backup metadata
                backupService->createBackupSummary(backupFolderPath, stagingList, backupDuration);

                // Finalize progress UI
                progressBar->setValue(ProgressConfig::MAX_VALUE);
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

    connect(workerThread, &QThread::started, worker, &TransferWorker::startTransfer);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);

    workerThread->start();
}

void BackupController::deleteBackup(const QString &backupPath) {
    if (!QFile::exists(backupPath)) {
        emit errorOccurred(BackupInfo::ERROR_INVALID_BACKUP_LOCATION);
        return;
    }

    QDir backupDir(backupPath);
    QString summaryFilePath = backupDir.filePath(UserSettings::BACKUP_SUMMARY_FILENAME);

    // Remove metadata file if it exists
    if (QFile::exists(summaryFilePath)) {
        QFile::remove(summaryFilePath);
    }

    // Delete backup directory recursively
    if (!backupDir.removeRecursively()) {
        emit errorOccurred(BackupInfo::ERROR_BACKUP_DELETION_FAILED);
        return;
    }

    emit backupDeleted();
}

// Status Check
bool BackupController::isBackupInProgress() const {
    return workerThread && workerThread->isRunning();
}

// Cleanup Operations
void BackupController::cleanupAfterTransfer() {
    if (!workerThread) return;

    if (workerThread->isRunning()) {
        workerThread->quit();
        workerThread->wait();
    }

    delete workerThread;
    workerThread = nullptr;
}
