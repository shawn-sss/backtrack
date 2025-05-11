#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

// Qt includes
#include <QJsonArray>
#include <QJsonObject>
#include <QSet>
#include <QString>
#include <QStringList>

// Represents backup folder status
enum class BackupStatus {
    None,
    Broken,
    Valid
};

// Handles creation, scanning, and summary of backups
class BackupService {
public:
    // Constructor
    explicit BackupService(const QString& backupRoot);
    Q_DISABLE_COPY(BackupService)

    // Path management
    void setBackupRoot(const QString& path);
    QString getBackupRoot() const;
    void initializeBackupRootIfNeeded();

    // Validation
    BackupStatus scanForBackupStatus() const;

    // Metadata operations
    QJsonObject getLastBackupMetadata() const;
    void createBackupSummary(const QString& backupFolderPath, const QStringList& selectedItems, qint64 backupDuration);

    // Statistics
    int getBackupCount() const;
    quint64 getTotalBackupSize() const;

private:
    // Metadata builder
    QJsonObject createBackupMetadata(const QString& backupFolderPath, const QStringList& selectedItems, qint64 backupDuration) const;

    // Utility for size calculation
    qint64 calculateTotalBackupSize(const QStringList& selectedItems) const;

    // Path state
    QString backupRootPath;
};

#endif // BACKUPSERVICE_H
