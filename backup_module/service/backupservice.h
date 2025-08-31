#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

// Qt includes
#include <QObject>
#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QJsonObject>

// Forward declaration (Custom class)
class UserServiceManager;

// Severity level of backup archive state
enum class BackupStatusLevel {
    Ok,
    Warning,
    Error
};

// Result of scanning backup root for integrity
struct BackupScanResult {
    bool validAppStructure    = false;
    bool validBackupStructure = false;
    bool structureExists      = false;
    bool hasOrphanedLogs      = false;
    bool hasMissingLogs       = false;

    bool isBroken() const {
        return !validAppStructure || !validBackupStructure;
    }

    bool isValid() const {
        return validAppStructure && validBackupStructure && !hasOrphanedLogs && !hasMissingLogs;
    }

    BackupStatusLevel getStatusLevel() const {
        if (isBroken()) return BackupStatusLevel::Error;
        if (hasOrphanedLogs || hasMissingLogs) return BackupStatusLevel::Warning;
        return BackupStatusLevel::Ok;
    }
};

// Handles scanning, metadata, and summaries for backups
class BackupService : public QObject {
    Q_OBJECT

public:
    explicit BackupService(const QString& backupRoot, QObject* parent = nullptr);

    // Backup root handling
    void setBackupRoot(const QString& path);
    QString getBackupRoot() const;

    // Initialization
    void initializeBackupRootIfNeeded();

    // Scanning
    BackupScanResult scanForBackupStatus() const;

    // Logs and metadata
    QFileInfoList getBackupLogFiles(bool sortedByTime = false) const;
    QJsonObject getLastBackupMetadata() const;
    int getBackupCount() const;
    quint64 getTotalBackupSize() const;
    qint64 calculateTotalBackupSize(const QStringList& items) const;

    // Summaries
    void createBackupSummary(const QString& backupFolderPath,
                             const QStringList& selectedItems,
                             qint64 backupDuration);

signals:
    void backupSummaryWritten(const QString& logFilePath);

private:
    QJsonObject createBackupMetadata(const QString& backupFolderPath,
                                     const QStringList& selectedItems,
                                     qint64 backupDuration) const;

private:
    QString backupRootPath;
};

#endif // BACKUPSERVICE_H
