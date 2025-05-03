#ifndef BACKUPSETTINGSMANAGER_H
#define BACKUPSETTINGSMANAGER_H

// Qt includes
#include <QString>
#include <QJsonObject>

// Forward declarations
class UserSettingsManager;

// Handles backup settings management
class BackupSettingsManager {
public:
    // Constructor
    BackupSettingsManager(UserSettingsManager& settingsManager);

    // Access full backup configuration group
    QJsonObject getBackupSettings() const;

    // Accessors for backup directory
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);

    // Accessors for backup file prefix
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

private:
    // Reference to user settings manager
    UserSettingsManager& userSettingsManager;
};

#endif // BACKUPSETTINGSMANAGER_H
