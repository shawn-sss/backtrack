// Project includes
#include "backupcontroller.h"
#include "../service/backupservice.h"
#include "../worker/transferworker.h"
#include "../../../ui/mainwindow/mainwindowmessages.h"
#include "../../../services/ServiceDirector/ServiceDirector.h"
#include "../../../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"
#include "../../../../constants/interface_config.h"
#include "../../../../constants/backup_config.h"
#include "../../../../constants/app_info.h"

// Qt includes
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QProgressBar>
#include <QStringList>
#include <QThread>

// Initializes the controller with service and parent
BackupController::BackupController(BackupService* service, QObject* parent)
    : QObject(parent), backupService(service), workerThread(nullptr) {}

// Cleans up when controller is destroyed
BackupController::~BackupController() {
    cleanupAfterTransfer();
}

// Returns whether a backup operation is running
bool BackupController::isBackupInProgress() const {
    return workerThread && workerThread->isRunning();
}

// Creates a new folder for backup
bool BackupController::createBackupFolder(const QString& path) {
    return QDir().mkpath(path);
}

// Generates a timestamped backup folder path
QString BackupController::generateBackupFolderPath(const QString& destinationPath) const {
    return QDir(destinationPath).filePath(
        ServiceDirector::getInstance().getBackupPrefix() +
        QDateTime::currentDateTime().toString(
            Backup::Timestamps::k_BACKUP_FOLDER_TIMESTAMP_FORMAT));
}

// Stops and cleans up the worker thread
void BackupController::cleanupAfterTransfer() {
    if (!workerThread)
        return;

    workerThread->quit();
    workerThread->wait();
    workerThread->deleteLater();
    workerThread = nullptr;
}

// Clears contents of the backup archive directory
void BackupController::resetBackupArchive(const QString& directoryPath) {
    QDir dir(directoryPath);
    const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    for (const QFileInfo& entry : entries) {
        const QString path = entry.absoluteFilePath();
        if (entry.isDir()) {
            QDir(path).removeRecursively();
        } else {
            QFile::remove(path);
        }
    }
}

// Starts a new backup operation in a worker thread
void BackupController::createBackup(const QString& destinationPath,
                                    const QStringList& stagingList,
                                    QProgressBar* progressBar) {
    if (workerThread && workerThread->isRunning()) {
        emit errorOccurred(WarningMessages::k_WARNING_BACKUP_OPERATION_RUNNING);
        return;
    }

    cleanupAfterTransfer();

    const QString configFilePath = PathServiceManager::backupInitMetadataFilePath();
    if (!QFile::exists(configFilePath)) {
        backupService->initializeBackupRootIfNeeded();
    }

    const QString backupFolderPath = generateBackupFolderPath(destinationPath);
    if (!createBackupFolder(backupFolderPath)) {
        emit errorOccurred(ErrorMessages::k_ERROR_CREATE_BACKUP_FOLDER);
        return;
    }

    progressBar->setRange(UI::Progress::k_PROGRESS_BAR_MIN_VALUE,
                          UI::Progress::k_PROGRESS_BAR_MAX_VALUE);
    progressBar->setValue(UI::Progress::k_PROGRESS_BAR_MIN_VALUE);
    progressBar->setVisible(true);

    const qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    workerThread = new QThread(this);
    auto* worker = new TransferWorker(stagingList, backupFolderPath);
    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, worker, &TransferWorker::startTransfer);
    connect(worker, &TransferWorker::progressUpdated, progressBar, &QProgressBar::setValue);

    connect(worker, &TransferWorker::transferComplete, this,
            [this, backupFolderPath, stagingList, startTime, progressBar]() {
                const qint64 backupDuration =
                    QDateTime::currentMSecsSinceEpoch() - startTime;

                connect(backupService, &BackupService::backupSummaryWritten, this,
                        [this, progressBar]() {
                            emit backupCreated();
                            progressBar->setValue(UI::Progress::k_PROGRESS_BAR_MAX_VALUE);
                            progressBar->setVisible(false);
                            cleanupAfterTransfer();
                        });

                backupService->createBackupSummary(backupFolderPath, stagingList, backupDuration);
            });

    connect(worker, &TransferWorker::errorOccurred, this,
            [this, progressBar](const QString& error) {
                progressBar->setVisible(false);
                emit errorOccurred(error);
                cleanupAfterTransfer();
            });

    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);

    workerThread->start();
}

// Deletes a backup folder and its log file
void BackupController::deleteBackup(const QString& backupPath) {
    const QString logFilePath =
        QDir(PathServiceManager::backupLogsFolderPath())
            .filePath(QFileInfo(backupPath).fileName() + "_" +
                      App::Items::k_BACKUP_SETUP_CONFIG_LOGS_FILE);

    if (!QFile::remove(logFilePath)) {
        emit errorOccurred(QFile::exists(logFilePath)
                           ? ErrorMessages::k_ERROR_DELETING_BACKUP_LOG
                           : ErrorMessages::k_ERROR_BACKUP_LOG_NOT_FOUND);
        return;
    }

    if (!QDir(backupPath).removeRecursively()) {
        emit errorOccurred(ErrorMessages::k_ERROR_DELETING_BACKUP_DIRECTORY);
        return;
    }

    emit backupDeleted();
}
