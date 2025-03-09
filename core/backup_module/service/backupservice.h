#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

// Built-in Qt includes
#include <QSet>
#include <QString>
#include <QStringList>
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
    // Constructor
    explicit BackupService(const QString &backupRoot);
    Q_DISABLE_COPY(BackupService)

    // Backup root management
    void setBackupRoot(const QString &path);
    QString getBackupRoot() const;

    // Backup initialization
    void initializeBackupRootIfNeeded();

    // Backup status retrieval
    BackupStatus scanForBackupStatus() const;
    QJsonObject getLastBackupMetadata() const;

    // Backup summary creation
    void createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration);

    // Backup statistics
    int getBackupCount() const;
    quint64 getTotalBackupSize() const;

private:
    // Generates metadata for a backup
    QJsonObject createBackupMetadata(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) const;

    // Calculates total size of selected backup items
    qint64 calculateTotalBackupSize(const QStringList &selectedItems) const;

    // Backup root directory path
    QString backupRootPath;
};

#endif // BACKUPSERVICE_H
