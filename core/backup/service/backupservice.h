#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

// Qt includes
#include <QJsonObject>
#include <QJsonArray>
#include <QSet>
#include <QString>
#include <QStringList>

// Represents backup folder status
enum class BackupStatus {
    None,
    Broken,
    Valid
};

// Describes results of a backup structure scan
struct BackupScanResult {
    bool hasOrphanedLogs = false;
    bool hasMissingLogs = false;
    bool validStructure = true;
    bool structureExists = false;

    bool isBroken() const {
        return structureExists && (!validStructure || hasOrphanedLogs || hasMissingLogs);
    }
};

// Handles creation, scanning, and summary of backups
class BackupService {
public:
    explicit BackupService(const QString& backupRoot);
    Q_DISABLE_COPY(BackupService)

    // Backup root management
    void setBackupRoot(const QString& path);
    QString getBackupRoot() const;
    void initializeBackupRootIfNeeded();

    // Backup validation
    BackupScanResult scanForBackupStatus() const;

    // Metadata retrieval
    QJsonObject getLastBackupMetadata() const;
    void createBackupSummary(const QString& backupFolderPath, const QStringList& selectedItems, qint64 backupDuration);

    // Backup statistics
    int getBackupCount() const;
    quint64 getTotalBackupSize() const;

private:
    // Metadata construction
    QJsonObject createBackupMetadata(const QString& backupFolderPath, const QStringList& selectedItems, qint64 backupDuration) const;

    // Size calculation
    qint64 calculateTotalBackupSize(const QStringList& selectedItems) const;

    // Root path
    QString backupRootPath;
};

#endif // BACKUPSERVICE_H
