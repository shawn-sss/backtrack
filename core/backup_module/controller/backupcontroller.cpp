// Project includes same directory
#include "backupcontroller.h"

// Project includes different directory
#include "../../../config/_constants.h"
#include "../../../config/configmanager/configmanager.h"
#include "../service/backupservice.h"
#include "../worker/transferworker.h"

// Built-in Qt includes
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QThread>
#include <QProgressBar>
#include <QStringList>

// Constructor
BackupController::BackupController(BackupService *service, QObject *parent)
    : QObject(parent), backupService(service), workerThread(nullptr) {}

// Destructor
BackupController::~BackupController() {
    cleanupAfterTransfer();
}

// Starts a new backup operation in a worker thread
void BackupController::createBackup(const QString &destinationPath,
                                    const QStringList &stagingList,
                                    QProgressBar *progressBar) {
    if (workerThread && workerThread->isRunning()) {
        emit errorOccurred(WarningMessages::WARNING_BACKUP_OPERATION_RUNNING);
        return;
    }

    cleanupAfterTransfer();

    const QString configFilePath = QDir(destinationPath).filePath(
        QStringLiteral("%1/%2").arg(AppConfig::BACKUP_SETUP_FOLDER, AppConfig::BACKUP_SETUP_INFO_FILE));

    if (!QFile::exists(configFilePath)) {
        backupService->initializeBackupRootIfNeeded();
    }

    const QString backupFolderPath = QDir(destinationPath).filePath(
        ConfigManager::getInstance().getBackupPrefix() +
        QDateTime::currentDateTime().toString(TimestampFormats::BACKUP_FOLDER_TIMESTAMP_FORMAT));

    if (!createBackupFolder(backupFolderPath)) {
        emit errorOccurred(ErrorMessages::ERROR_CREATING_BACKUP_FOLDER);
        return;
    }

    progressBar->setRange(ProgressSettings::PROGRESS_BAR_MIN_VALUE,
                          ProgressSettings::PROGRESS_BAR_MAX_VALUE);
    progressBar->setValue(ProgressSettings::PROGRESS_BAR_MIN_VALUE);
    progressBar->setVisible(true);

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

    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);

    workerThread->start();
}

// Deletes a backup and its associated log file
void BackupController::deleteBackup(const QString &backupPath) {
    const QString logFilePath = QDir(backupService->getBackupRoot()).filePath(
        QStringLiteral("%1/%2/%3_%4")
            .arg(AppConfig::BACKUP_SETUP_FOLDER,
                 AppConfig::BACKUP_LOGS_FOLDER,
                 QFileInfo(backupPath).fileName(),
                 AppConfig::BACKUP_LOGS_FILE));

    if (!QFile::remove(logFilePath)) {
        emit errorOccurred(QFile::exists(logFilePath)
                           ? ErrorMessages::ERROR_DELETING_BACKUP_LOG
                           : ErrorMessages::ERROR_BACKUP_LOG_NOT_FOUND);
        return;
    }

    if (!QDir(backupPath).removeRecursively()) {
        emit errorOccurred(ErrorMessages::ERROR_DELETING_BACKUP_DIRECTORY);
        return;
    }

    emit backupDeleted();
}

// Returns true if a backup operation is in progress
bool BackupController::isBackupInProgress() const {
    return workerThread && workerThread->isRunning();
}

// Creates the backup destination folder
bool BackupController::createBackupFolder(const QString &path) {
    return QDir().mkpath(path);
}

// Cleans up the worker thread after a backup transfer
void BackupController::cleanupAfterTransfer() {
    if (workerThread) {
        workerThread->quit();
        workerThread->wait();
        workerThread->deleteLater();
        workerThread = nullptr;
    }
}
