#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

#include <QString>
#include <QSet>
#include <QJsonObject>
#include <QJsonArray>

// Backup status enum
enum class BackupStatus {
    None,
    Broken,
    Valid
};

// BackupService class definition
class BackupService {
public:
    explicit BackupService(const QString &backupRoot);
    Q_DISABLE_COPY(BackupService)

    // Backup root management
public:
    void setBackupRoot(const QString &path);
    QString getBackupRoot() const;

    // Backup status and metadata
public:
    BackupStatus scanForBackupStatus() const;
    QJsonObject getLastBackupMetadata() const;

    // Backup summary management
public:
    void createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration);

    // Backup statistics
public:
    int getBackupCount() const;
    quint64 getTotalBackupSize() const;

    // Internal metadata creation
private:
    QJsonObject createBackupMetadata(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) const;

    // Internal size calculations
private:
    qint64 calculateTotalBackupSize(const QStringList &selectedItems) const;

    // File and folder collection
private:
    void collectBackupFiles(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) const;
    void collectBackupFolders(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray) const;

    // Internal variables
private:
    QString backupRootPath;
};

#endif // BACKUPSERVICE_H
