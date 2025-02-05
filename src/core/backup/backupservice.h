#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

#include <QString>
#include <QJsonObject>
#include <QSet>
#include <QJsonArray>

// Service class for handling backup operations
class BackupService {
public:
    // Constructor
    explicit BackupService(const QString &backupRoot);

    // Backup root path management
    void setBackupRoot(const QString &path);
    QString getBackupRoot() const;

    // Backup metadata operations
    bool scanForBackupSummary() const;
    void createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration);
    QJsonObject getLastBackupMetadata() const;

    // Backup statistics retrieval
    int getBackupCount() const;
    quint64 getTotalBackupSize() const;

private:
    // Directory traversal and size calculation
    qint64 calculateTotalBackupSize(const QStringList &selectedItems) const;
    void traverseDirectory(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) const;
    void traverseDirectoryForFolders(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray) const;

    // Member variables for backup management
    QString backupRootPath;
};

#endif // BACKUPSERVICE_H
