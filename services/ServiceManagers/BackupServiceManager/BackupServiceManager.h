#ifndef BACKUPSERVICEMANAGER_H
#define BACKUPSERVICEMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Forward declarations (Custom class)
class UserServiceManager;

// Manages backup-related user settings
class BackupServiceManager {
public:
    // Constructor
    explicit BackupServiceManager(UserServiceManager& serviceManager);

    // Access to backup service settings
    QJsonObject getBackupSettings() const;
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

private:
    // Internal update helper
    void updateBackupSetting(const QString& key, const QString& newValue, const QString& defaultValue);

    // Reference to user service manager
    UserServiceManager& userServiceManager;
};

#endif // BACKUPSERVICEMANAGER_H
