#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

#include <QString>
#include <QJsonObject>
#include <QSet>
#include <QJsonArray>

// Enum for Backup Status
enum class BackupStatus {
    None,
    Broken,
    Valid
};

// Backup Service Class
class BackupService {
public:
    // Constructor
    explicit BackupService(const QString &backupRoot);
    Q_DISABLE_COPY(BackupService)

    // Backup Root Management
    void setBackupRoot(const QString &path);
    QString getBackupRoot() const;

    // Backup Status and Metadata
    BackupStatus scanForBackupStatus() const;
    QJsonObject getLastBackupMetadata() const;

    // Backup Summary
    void createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration);

    // Backup Statistics
    int getBackupCount() const;
    quint64 getTotalBackupSize() const;

private:
    // Metadata Creation
    QJsonObject createBackupMetadata(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) const;

    // Backup Size Calculation
    qint64 calculateTotalBackupSize(const QStringList &selectedItems) const;

    // Backup Collection Helpers
    void collectBackupFiles(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) const;
    void collectBackupFolders(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray) const;

    // Private Member Variables
    QString backupRootPath;
};

#endif // BACKUPSERVICE_H
