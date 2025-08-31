#ifndef BACKUPCONTROLLER_H
#define BACKUPCONTROLLER_H

// Project includes
#include "../service/backupservice.h"

// Qt includes
#include <QObject>
#include <QStringList>

// Forward declaration (Qt class)
class QThread;
class QProgressBar;

// Manages backup creation, deletion, and worker thread orchestration
class BackupController : public QObject {
    Q_OBJECT

public:
    explicit BackupController(BackupService* service, QObject* parent = nullptr);
    ~BackupController();

    // Backup state
    bool isBackupInProgress() const;

    // Backup lifecycle
    void createBackup(const QString& destinationPath,
                      const QStringList& stagingList,
                      QProgressBar* progressBar);
    void deleteBackup(const QString& backupPath);
    void resetBackupArchive(const QString& directoryPath);

signals:
    void errorOccurred(const QString& errorMessage);
    void backupCreated();
    void backupDeleted();

private:
    // Internal helpers
    QString generateBackupFolderPath(const QString& destinationPath) const;
    void cleanupAfterTransfer();

private:
    BackupService* backupService;
    QThread* workerThread;
};

#endif // BACKUPCONTROLLER_H
