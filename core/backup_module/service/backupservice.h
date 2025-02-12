#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

#include <QString>
#include <QJsonObject>
#include <QSet>
#include <QJsonArray>

// Backup Service Class
class BackupService {
public:
    // Constructor
    explicit BackupService(const QString &backupRoot);

    Q_DISABLE_COPY(BackupService)

    // Backup Root Management
    void setBackupRoot(const QString &path);
    QString getBackupRoot() const;

    // Backup Metadata Management
    bool scanForBackupSummary() const;
    QJsonObject getLastBackupMetadata() const;
    void createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration);

    // Backup Statistics
    int getBackupCount() const;
    quint64 getTotalBackupSize() const;

private:
    // Metadata Generation
    QJsonObject createBackupMetadata(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) const;
    qint64 calculateTotalBackupSize(const QStringList &selectedItems) const;

    // File Traversal Methods
    void traverseDirectory(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) const;
    void traverseDirectoryForFolders(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray) const;

    // Backup Root Path
    QString backupRootPath;
};

#endif // BACKUPSERVICE_H
