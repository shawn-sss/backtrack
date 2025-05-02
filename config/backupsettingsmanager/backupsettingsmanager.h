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

    // Settings access
    QJsonObject getBackupSettings() const;

    // Directory accessors
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);

    // Prefix accessors
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

private:
    UserSettingsManager& userSettingsManager;
};

#endif // BACKUPSETTINGSMANAGER_H
