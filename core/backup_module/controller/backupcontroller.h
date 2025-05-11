#ifndef BACKUPCONTROLLER_H
#define BACKUPCONTROLLER_H

// Qt includes
#include <QObject>
#include <QStringList>

// Forward Declaration (Qt class)
class QProgressBar;
class QThread;

// Forward Declaration (Custom class)
class BackupService;

// Manages backup operations
class BackupController : public QObject {
    Q_OBJECT

public:
    // Constructor and destructor
    explicit BackupController(BackupService* service, QObject* parent = nullptr);
    ~BackupController();

    // Backup interface methods
    void createBackup(const QString& destinationPath, const QStringList& stagingList, QProgressBar* progressBar);
    void deleteBackup(const QString& backupPath);
    bool isBackupInProgress() const;

signals:
    // Events for backup lifecycle
    void backupCreated();
    void backupDeleted();
    void errorOccurred(const QString& error);

private:
    // Helper methods
    void cleanupAfterTransfer();
    bool createBackupFolder(const QString& path);

    // Internal state
    BackupService* backupService;
    QThread* workerThread;
};

#endif // BACKUPCONTROLLER_H
