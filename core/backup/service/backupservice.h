#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

// Qt includes
#include <QFileInfoList>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
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
    bool validAppStructure = false;
    bool validBackupStructure = false;
    bool structureExists = false;
    bool hasOrphanedLogs = false;
    bool hasMissingLogs = false;

    bool isBroken() const {
        return !validBackupStructure || hasOrphanedLogs || hasMissingLogs;
    }
};

// Handles creation, scanning, and summary of backups
class BackupService : public QObject {
    Q_OBJECT

public:
    explicit BackupService(const QString& backupRoot, QObject* parent = nullptr);
    Q_DISABLE_COPY(BackupService)

    // Configuration
    void setBackupRoot(const QString& path);
    QString getBackupRoot() const;
    void initializeBackupRootIfNeeded();

    // Structure scanning
    BackupScanResult scanForBackupStatus() const;

    // Backup metadata access
    QJsonObject getLastBackupMetadata() const;
    void createBackupSummary(const QString& backupFolderPath, const QStringList& selectedItems, qint64 backupDuration);

    // Backup statistics
    int getBackupCount() const;
    quint64 getTotalBackupSize() const;

signals:
    void backupSummaryWritten(const QString& logFilePath);

private:
    // Internal helpers for metadata and file analysis
    QJsonObject createBackupMetadata(const QString& backupFolderPath, const QStringList& selectedItems, qint64 backupDuration) const;
    qint64 calculateTotalBackupSize(const QStringList& selectedItems) const;
    QFileInfoList getBackupLogFiles(bool sortedByTime = false) const;

    QString backupRootPath;
};

#endif // BACKUPSERVICE_H
