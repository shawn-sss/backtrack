#include "backupcontroller.h"
#include "../../config/_constants.h"
#include "../../config/configmanager/configmanager.h"

#include "../service/backupservice.h"
#include "../worker/transferworker.h"

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
        emit errorOccurred(ErrorMessages::WARNING_BACKUP_OPERATION_RUNNING);
        return;
    }

    cleanupAfterTransfer();

    QString timestamp = QDateTime::currentDateTime().toString(BackupInfo::BACKUP_FOLDER_TIMESTAMP_FORMAT);
    QString backupFolderName = QString("%1%2").arg(ConfigManager::getInstance().getBackupPrefix(), timestamp);

    QDir destDir(destinationPath);
    QString backupFolderPath = destDir.filePath(backupFolderName);

    if (!createBackupFolder(backupFolderPath)) return;

    progressBar->setVisible(true);
    progressBar->setValue(ProgressConfig::PROGRESS_BAR_MIN_VALUE);

    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    workerThread = new QThread(this);
    auto *worker = new TransferWorker(stagingList, backupFolderPath);
    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, worker, &TransferWorker::startTransfer);
    connect(worker, &TransferWorker::progressUpdated, progressBar, &QProgressBar::setValue);
    connect(worker, &TransferWorker::transferComplete, this, [this, backupFolderPath, stagingList, startTime, progressBar]() {
        qint64 backupDuration = QDateTime::currentMSecsSinceEpoch() - startTime;
        backupService->createBackupSummary(backupFolderPath, stagingList, backupDuration);
        progressBar->setValue(ProgressConfig::PROGRESS_BAR_MAX_VALUE);
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
        QString("%1/%2").arg(AppConfig::BACKUP_CONFIG_FOLDER, AppConfig::BACKUP_LOGS_DIRECTORY));
    QString logFileName = QFileInfo(backupPath).fileName() + AppConfig::BACKUP_LOG_FILE_SUFFIX;
    QString logFilePath = QDir(logsFolderPath).filePath(logFileName);

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

// Status Check
bool BackupController::isBackupInProgress() const {
    return workerThread && workerThread->isRunning();
}

// Helper Methods
bool BackupController::createBackupFolder(const QString &path) {
    if (!QDir().mkpath(path)) {
        emit errorOccurred(ErrorMessages::ERROR_CREATING_BACKUP_FOLDER);
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
