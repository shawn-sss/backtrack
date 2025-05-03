#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

// Qt includes
#include <QString>
#include <QStringList>
#include <QSet>
#include <QJsonArray>
#include <QJsonObject>

// Represents the current backup status
enum class BackupStatus {
    None,
    Broken,
    Valid
};

// Handles backup operations and summary metadata
class BackupService {
public:
    // Constructor
    explicit BackupService(const QString& backupRoot);
    Q_DISABLE_COPY(BackupService)

    // Path management
    void setBackupRoot(const QString& path);
    QString getBackupRoot() const;
    void initializeBackupRootIfNeeded();

    // Backup structure validation
    BackupStatus scanForBackupStatus() const;

    // Backup metadata operations
    QJsonObject getLastBackupMetadata() const;
    void createBackupSummary(const QString& backupFolderPath, const QStringList& selectedItems, qint64 backupDuration);

    // Backup statistics
    int getBackupCount() const;
    quint64 getTotalBackupSize() const;

private:
    // Internal metadata construction
    QJsonObject createBackupMetadata(const QString& backupFolderPath, const QStringList& selectedItems, qint64 backupDuration) const;
    qint64 calculateTotalBackupSize(const QStringList& selectedItems) const;

    // Internal backup root state
    QString backupRootPath;
};

#endif // BACKUPSERVICE_H
