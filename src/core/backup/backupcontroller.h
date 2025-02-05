#ifndef BACKUPCONTROLLER_H
#define BACKUPCONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QProgressBar>
#include <QThread>

// Forward declaration of dependent class
class BackupService;

// Controller class for managing backups
class BackupController : public QObject {
    Q_OBJECT

public:
    // Constructor and destructor
    explicit BackupController(BackupService *service, QObject *parent = nullptr);
    ~BackupController();

    // Backup operation methods
    void createBackup(const QString &destinationPath, const QStringList &stagingList, QProgressBar *progressBar);
    void deleteBackup(const QString &backupPath);
    bool isBackupInProgress() const;

signals:
    // Signals for tracking backup progress and errors
    void backupCreated();
    void backupDeleted();
    void progressUpdated(int progress);
    void errorOccurred(const QString &error);

private:
    // Helper for cleanup after backup operations
    void cleanupAfterTransfer();

    // Member variables for backup management
    BackupService *backupService;
    QThread *workerThread;
};

#endif // BACKUPCONTROLLER_H
