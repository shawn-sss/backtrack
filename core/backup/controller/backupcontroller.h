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

    // Backup interface methods
    void createBackup(const QString& destinationPath,
                      const QStringList& stagingList,
                      QProgressBar* progressBar);
    void deleteBackup(const QString& backupPath);
    bool isBackupInProgress() const;

signals:
    // Events for backup lifecycle
    void backupCreated();
    void backupDeleted();
    void errorOccurred(const QString& error);

private:
    // Internal utilities
    void cleanupAfterTransfer();
    bool createBackupFolder(const QString& path);
    QString generateBackupFolderPath(const QString& destinationPath) const;

    // Member variables
    BackupService* backupService;
    QThread* workerThread;
};

#endif // BACKUPCONTROLLER_H
