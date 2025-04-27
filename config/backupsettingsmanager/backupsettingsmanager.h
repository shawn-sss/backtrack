#ifndef BACKUPSETTINGSMANAGER_H
#define BACKUPSETTINGSMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Forward declarations
class UserSettingsManager;

// Handles backup settings management
class BackupSettingsManager {
public:
    BackupSettingsManager(UserSettingsManager& settingsManager); // Constructor

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
