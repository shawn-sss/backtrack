#ifndef BACKUPSETTINGSMANAGER_H
#define BACKUPSETTINGSMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Forward Declaration (Custom class)
class UserSettingsManager;

// Manages backup settings configuration via user settings
class BackupSettingsManager {
public:
    // Lifecycle
    explicit BackupSettingsManager(UserSettingsManager& settingsManager);

    // Retrieves the full backup settings object
    QJsonObject getBackupSettings() const;

    // Backup directory management
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);

    // Backup file prefix management
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

private:
    UserSettingsManager& userSettingsManager;
};

#endif // BACKUPSETTINGSMANAGER_H
