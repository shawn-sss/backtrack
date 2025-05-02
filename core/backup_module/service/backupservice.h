#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

// Qt includes
#include <QString>
#include <QStringList>
#include <QSet>
#include <QJsonArray>
#include <QJsonObject>

// Defines possible backup statuses
enum class BackupStatus {
    None,
    Broken,
    Valid
};

// Handles backup operations and metadata
class BackupService {
public:
    // Constructs a BackupService for the given root path
    explicit BackupService(const QString &backupRoot);
    Q_DISABLE_COPY(BackupService)

    // Sets the backup root path
    void setBackupRoot(const QString &path);

    // Returns the current backup root path
    QString getBackupRoot() const;

    // Initializes backup folder structure if missing
    void initializeBackupRootIfNeeded();

    // Checks the status of the backup structure
    BackupStatus scanForBackupStatus() const;

    // Returns metadata for the most recent backup
    QJsonObject getLastBackupMetadata() const;

    // Writes a summary log for a completed backup
    void createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration);

    // Returns the number of completed backups
    int getBackupCount() const;

    // Returns the total size of all backups in bytes
    quint64 getTotalBackupSize() const;

private:
    // Builds metadata JSON object for a backup
    QJsonObject createBackupMetadata(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) const;

    // Calculates the total size of selected files/folders
    qint64 calculateTotalBackupSize(const QStringList &selectedItems) const;

    // Root directory for backups
    QString backupRootPath;
};

#endif // BACKUPSERVICE_H
