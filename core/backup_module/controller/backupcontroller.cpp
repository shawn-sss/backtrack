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
    : QObject(parent), backupService(service) {}

BackupController::~BackupController() {
    cleanupAfterTransfer();
}

// Backup Operations
void BackupController::createBackup(const QString &destinationPath,
                                    const QStringList &stagingList,
                                    QProgressBar *progressBar) {
    if (isBackupInProgress()) {
        emit errorOccurred(UIConfig::MESSAGE_BACKUP_IN_PROGRESS);
        return;
    }

    cleanupAfterTransfer();

    QString timestamp = QDateTime::currentDateTime().toString(BackupInfo::BACKUP_FOLDER_TIMESTAMP_FORMAT);
    QString backupFolderName = QString(BackupInfo::BACKUP_FOLDER_FORMAT)
                                   .arg(UserSettings::BACKUP_FOLDER_PREFIX, timestamp);

    QDir destDir(destinationPath);
    QString backupFolderPath = destDir.filePath(backupFolderName);

    if (!createBackupFolder(backupFolderPath)) {
        return;
    }

    progressBar->setVisible(true);
    progressBar->setValue(ProgressConfig::MIN_VALUE);

    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    workerThread = new QThread(this);
    auto *worker = new TransferWorker(stagingList, backupFolderPath);
    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, worker, &TransferWorker::startTransfer);
    connect(worker, &TransferWorker::progressUpdated, progressBar, &QProgressBar::setValue);
    connect(worker, &TransferWorker::transferComplete, this,
            [this, backupFolderPath, stagingList, startTime, progressBar]() {
                qint64 backupDuration = QDateTime::currentMSecsSinceEpoch() - startTime;

                backupService->createBackupSummary(backupFolderPath, stagingList, backupDuration);
                progressBar->setValue(ProgressConfig::MAX_VALUE);
                progressBar->setVisible(false);
                emit backupCreated();
            });

    connect(worker, &TransferWorker::errorOccurred, this, [this, progressBar](const QString &error) {
        progressBar->setVisible(false);
        emit errorOccurred(error);
        cleanupAfterTransfer();
    });

    connect(worker, &TransferWorker::finished, this, &BackupController::cleanupAfterTransfer);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);

    workerThread->start();
}

void BackupController::deleteBackup(const QString &backupPath) {
    QString logsFolderPath = QDir(backupService->getBackupRoot()).filePath(
        QString("%1/%2").arg(AppConfig::BACKUP_SETTINGS_FOLDER, AppConfig::BACKUP_LOGS_FOLDER));
    QString logFileName = QFileInfo(backupPath).fileName() + AppConfig::BACKUP_LOG_SUFFIX;
    QString logFilePath = QDir(logsFolderPath).filePath(logFileName);

    // Ensure log file exists before deleting
    if (!QFile::exists(logFilePath)) {
        emit errorOccurred(BackupInfo::ERROR_INVALID_BACKUP_LOCATION);
        return;
    }

    // Attempt to remove the backup log file first
    if (!QFile::remove(logFilePath)) {
        emit errorOccurred(BackupInfo::ERROR_BACKUP_DELETION_FAILED);
        return;
    }

    // Proceed with deleting the backup directory
    QDir backupDir(backupPath);
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

// Helper Methods
bool BackupController::createBackupFolder(const QString &path) {
    if (!QDir().mkpath(path)) {
        emit errorOccurred(BackupInfo::ERROR_BACKUP_FOLDER_CREATION_FAILED);
        return false;
    }
    return true;
}

// Cleanup Operations
void BackupController::cleanupAfterTransfer() {
    if (workerThread) {
        if (workerThread->isRunning()) {
            workerThread->quit();
            workerThread->wait();
        }
        workerThread->deleteLater();
        workerThread = nullptr;
    }
}
