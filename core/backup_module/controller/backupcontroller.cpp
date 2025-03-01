#include "backupcontroller.h"

#include "../../../config/_constants.h"
#include "../../../config/configmanager/configmanager.h"
#include "../service/backupservice.h"
#include "../worker/transferworker.h"

#include <QDir>
#include <QDateTime>
#include <QFile>
#include <QStringList>
#include <QThread>
#include <QProgressBar>

// Constructor and destructor
BackupController::BackupController(BackupService *service, QObject *parent)
    : QObject(parent), backupService(service), workerThread(nullptr) {}

BackupController::~BackupController() {
    cleanupAfterTransfer();
}

// Creates a backup process and starts the transfer worker
void BackupController::createBackup(const QString &destinationPath,
                                    const QStringList &stagingList,
                                    QProgressBar *progressBar) {
    if (isBackupInProgress()) {
        emit errorOccurred(WarningMessages::WARNING_BACKUP_OPERATION_RUNNING);
        return;
    }

    cleanupAfterTransfer();

    const QString timestamp = QDateTime::currentDateTime().toString(TimestampFormats::BACKUP_FOLDER_TIMESTAMP_FORMAT);
    const QString backupFolderName = ConfigManager::getInstance().getBackupPrefix() + timestamp;

    QDir destDir(destinationPath);
    const QString backupFolderPath = destDir.filePath(backupFolderName);

    if (!createBackupFolder(backupFolderPath)) return;

    progressBar->setVisible(true);
    progressBar->setValue(ProgressSettings::PROGRESS_BAR_MIN_VALUE);

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
                progressBar->setValue(ProgressSettings::PROGRESS_BAR_MAX_VALUE);
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

    connect(worker, &TransferWorker::finished, this, &BackupController::cleanupAfterTransfer);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);

    workerThread->start();
}

// Deletes an existing backup and its log file
void BackupController::deleteBackup(const QString &backupPath) {
    const QString logsFolderPath = QDir(backupService->getBackupRoot()).filePath(
        QString("%1/%2").arg(AppConfig::BACKUP_CONFIG_FOLDER, AppConfig::BACKUP_LOGS_DIRECTORY));
    const QString logFileName = QFileInfo(backupPath).fileName() + "_" + AppConfig::BACKUP_LOG_FILE_SUFFIX;
    const QString logFilePath = QDir(logsFolderPath).filePath(logFileName);

    if (!QFile::exists(logFilePath)) {
        emit errorOccurred(ErrorMessages::ERROR_BACKUP_LOG_NOT_FOUND);
        return;
    }

    if (!QFile::remove(logFilePath)) {
        emit errorOccurred(ErrorMessages::ERROR_DELETING_BACKUP_LOG);
        return;
    }

    QDir backupDir(backupPath);
    if (!backupDir.removeRecursively()) {
        emit errorOccurred(ErrorMessages::ERROR_DELETING_BACKUP_DIRECTORY);
        return;
    }

    emit backupDeleted();
}

// Checks if a backup operation is currently running
bool BackupController::isBackupInProgress() const {
    return workerThread && workerThread->isRunning();
}

// Creates a folder for the backup at the given path
bool BackupController::createBackupFolder(const QString &path) {
    if (!QDir().mkpath(path)) {
        emit errorOccurred(ErrorMessages::ERROR_CREATING_BACKUP_FOLDER);
        return false;
    }
    return true;
}

// Cleans up after a transfer, ensuring the worker thread is properly stopped
void BackupController::cleanupAfterTransfer() {
    if (workerThread && workerThread->isRunning()) {
        workerThread->quit();
        workerThread->wait();
    }

    if (workerThread) {
        workerThread->deleteLater();
        workerThread = nullptr;
    }
}
