#ifndef BACKUPCONTROLLER_H
#define BACKUPCONTROLLER_H

#include <QObject>
#include <QStringList>

// Forward Declarations
class QProgressBar;
class QThread;
class BackupService;

// Backup Controller Class
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
    // Backup Status Signals
    void backupCreated();
    void backupDeleted();
    void errorOccurred(const QString &error);

private:
    // Internal Utility Methods
    void cleanupAfterTransfer();
    bool createBackupFolder(const QString &path);

    // Backup Management Variables
    BackupService *backupService;
    QThread *workerThread = nullptr;
};

#endif // BACKUPCONTROLLER_H
