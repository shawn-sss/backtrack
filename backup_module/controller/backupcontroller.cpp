// Project includes
#include "backupcontroller.h"
#include "../constants/backupconstants.h"
#include "../service/fileoperations.h"
#include "../worker/transferworker.h"
#include "../../../../constants/app_info.h"
#include "../../../../constants/interface_config.h"
#include "../../../../services/ServiceDirector/ServiceDirector.h"
#include "../../../../services/ServiceManagers/BackupServiceManager/BackupServiceManager.h"
#include "../../../../services/ServiceManagers/PathServiceManager/PathServiceManager.h"

// Qt includes
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QProgressBar>
#include <QThread>

// Constructor / Destructor
BackupController::BackupController(BackupService* service, QObject* parent)
    : QObject(parent), backupService(service), workerThread(nullptr) {}

BackupController::~BackupController() {
    cleanupAfterTransfer();
}

// Check if backup is in progress
bool BackupController::isBackupInProgress() const {
    return workerThread && workerThread->isRunning();
}

// Generate a timestamped backup folder path
QString BackupController::generateBackupFolderPath(const QString& destinationPath) const {
    return QDir(destinationPath).filePath(
        ServiceDirector::getInstance()
            .getBackupServiceManager()
            ->getBackupPrefix() +
        QDateTime::currentDateTime().toString(Backup::Timestamps::FolderFormat));
}

// Start a new backup
void BackupController::createBackup(const QString& destinationPath,
                                    const QStringList& stagingList,
                                    QProgressBar* progressBar) {
    if (workerThread && workerThread->isRunning()) {
        emit errorOccurred(Backup::Errors::Messages[Backup::ErrorCode::BackupAlreadyRunning]);
        return;
    }

    cleanupAfterTransfer();

    const QString configFilePath = PathServiceManager::backupInitMetadataFilePath();
    if (!QFile::exists(configFilePath)) {
        backupService->initializeBackupRootIfNeeded();
    }

    const QString backupFolderPath = generateBackupFolderPath(destinationPath);
    if (!FileOperations::createDirectory(backupFolderPath)) {
        emit errorOccurred(Backup::Errors::Messages[Backup::ErrorCode::CreateBackupFolder]);
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

// Delete a backup and its log file
void BackupController::deleteBackup(const QString& backupPath) {
    const QString logFilePath =
        QDir(PathServiceManager::backupLogsFolderPath())
            .filePath(QFileInfo(backupPath).fileName() + "_" +
                      App::Items::k_BACKUP_SETUP_CONFIG_LOGS_FILE);

    if (!QFile::remove(logFilePath)) {
        emit errorOccurred(QFile::exists(logFilePath)
                           ? Backup::Errors::Messages[Backup::ErrorCode::DeletingBackupLog]
                           : Backup::Errors::Messages[Backup::ErrorCode::BackupLogNotFound]);
        return;
    }

    if (!FileOperations::deleteDirectory(backupPath)) {
        emit errorOccurred(Backup::Errors::Messages[Backup::ErrorCode::DeletingBackupDirectory]);
        return;
    }

    emit backupDeleted();
}

// Cleanup worker thread after transfer
void BackupController::cleanupAfterTransfer() {
    if (!workerThread) return;

    workerThread->quit();
    workerThread->wait();
    workerThread->deleteLater();
    workerThread = nullptr;
}

// Reset backup archive (clear folder contents)
void BackupController::resetBackupArchive(const QString& directoryPath) {
    QDir dir(directoryPath);
    const QFileInfoList entries = dir.entryInfoList(Backup::Filters::All);

    for (const QFileInfo& entry : entries) {
        const QString path = entry.absoluteFilePath();
        if (entry.isDir()) {
            FileOperations::deleteDirectory(path);
        } else {
            QFile::remove(path);
        }
    }
}
