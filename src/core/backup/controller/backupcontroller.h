#ifndef BACKUPCONTROLLER_H
#define BACKUPCONTROLLER_H

#include <QObject>
#include <QStringList>

// Forward declarations
class QProgressBar;
class QThread;
class BackupService;

// Controller class for managing backups
class BackupController : public QObject {
    Q_OBJECT

public:
    // Constructor and Destructor
    explicit BackupController(BackupService *service, QObject *parent = nullptr);
    ~BackupController();

    // Backup Operations
    void createBackup(const QString &destinationPath, const QStringList &stagingList, QProgressBar *progressBar);
    void deleteBackup(const QString &backupPath);
    bool isBackupInProgress() const;

signals:
    // Signals for backup status updates
    void backupCreated();
    void backupDeleted();
    void progressUpdated(int progress);
    void errorOccurred(const QString &error);

private:
    // Utility methods for backup handling
    void cleanupAfterTransfer();
    bool createBackupFolder(const QString &path);

    // Member variables for backup management
    BackupService *backupService;
    QThread *workerThread;
};

#endif // BACKUPCONTROLLER_H
