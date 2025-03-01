#ifndef BACKUPCONTROLLER_H
#define BACKUPCONTROLLER_H

#include <QStringList>
#include <QObject>

// Forward declarations
class QProgressBar;
class QThread;
class BackupService;

// BackupController class definition
class BackupController : public QObject {
    Q_OBJECT

public:
    explicit BackupController(BackupService *service, QObject *parent = nullptr);
    ~BackupController();

    // Backup operations
public:
    void createBackup(const QString &destinationPath, const QStringList &stagingList, QProgressBar *progressBar);
    void deleteBackup(const QString &backupPath);
    bool isBackupInProgress() const;

    // Signals
signals:
    void backupCreated();
    void backupDeleted();
    void errorOccurred(const QString &error);

    // Internal utility methods
private:
    void cleanupAfterTransfer();
    bool createBackupFolder(const QString &path);

    // Internal variables
private:
    BackupService *backupService;
    QThread *workerThread = nullptr;
};

#endif // BACKUPCONTROLLER_H
