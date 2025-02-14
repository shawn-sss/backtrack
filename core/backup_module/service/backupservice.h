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
    explicit BackupService(const QString &backupRoot);
    Q_DISABLE_COPY(BackupService)

    void setBackupRoot(const QString &path);
    QString getBackupRoot() const;

    BackupStatus scanForBackupStatus() const;
    QJsonObject getLastBackupMetadata() const;
    void createBackupSummary(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration);

    int getBackupCount() const;
    quint64 getTotalBackupSize() const;

private:
    QJsonObject createBackupMetadata(const QString &backupFolderPath, const QStringList &selectedItems, qint64 backupDuration) const;
    qint64 calculateTotalBackupSize(const QStringList &selectedItems) const;

    void collectBackupFiles(const QString &dirPath, QSet<QString> &uniqueFiles, QJsonArray &filesArray) const;
    void collectBackupFolders(const QString &dirPath, QSet<QString> &uniqueFolders, QJsonArray &foldersArray) const;


    QString backupRootPath;
};

#endif // BACKUPSERVICE_H
