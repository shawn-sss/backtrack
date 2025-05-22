#ifndef BACKUPCONTROLLER_H
#define BACKUPCONTROLLER_H

// Qt includes
#include <QObject>
#include <QStringList>

// Forward declaration (Qt class)
class QProgressBar;
class QThread;

// Forward declaration (Custom class)
class BackupService;

// Manages backup operations
class BackupController : public QObject {
    Q_OBJECT

public:
    // Constructor and destructor
    explicit BackupController(BackupService* service, QObject* parent = nullptr);
    ~BackupController();

    // Public interface
    void createBackup(const QString& destinationPath,
                      const QStringList& stagingList,
                      QProgressBar* progressBar);
    void deleteBackup(const QString& backupPath);
    bool isBackupInProgress() const;
    void resetBackupArchive(const QString& directoryPath);

signals:
    // Backup operation events
    void backupCreated();
    void backupDeleted();
    void errorOccurred(const QString& error);

private:
    // Internal helpers
    void cleanupAfterTransfer();
    bool createBackupFolder(const QString& path);
    QString generateBackupFolderPath(const QString& destinationPath) const;

    // Members
    BackupService* backupService;
    QThread* workerThread;
};

#endif // BACKUPCONTROLLER_H
