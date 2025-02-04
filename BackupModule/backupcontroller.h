#ifndef BACKUPCONTROLLER_H
#define BACKUPCONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QProgressBar>
#include <QThread>

// Forward declaration of dependent classes
class BackupService;

// Controller class for managing backups
class BackupController : public QObject {
    Q_OBJECT

public:
    // Constructor and destructor
    explicit BackupController(BackupService *service, QObject *parent = nullptr);
    ~BackupController();

    // Public methods for backup management
    void createBackup(const QString &destinationPath, const QStringList &stagingList, QProgressBar *progressBar);
    void deleteBackup(const QString &backupPath);
    bool isBackupInProgress() const;

signals:
    // Signals for backup progress or errors
    void backupCreated();
    void backupDeleted();
    void progressUpdated(int progress);
    void errorOccurred(const QString &error);

private:
    // Private helper methods
    void cleanupAfterTransfer();

    // Member variables
    BackupService *backupService;
    QThread *workerThread;
};

#endif // BACKUPCONTROLLER_H
