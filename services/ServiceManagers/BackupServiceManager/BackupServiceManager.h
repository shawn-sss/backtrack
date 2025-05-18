#ifndef BACKUPSERVICEMANAGER_H
#define BACKUPSERVICEMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Forward declaration (Custom class)
class UserServiceManager;

// Manages user-configurable backup settings
class BackupServiceManager {
public:
    // Constructor
    explicit BackupServiceManager(UserServiceManager& serviceManager);

    // Return the entire backup settings JSON object
    QJsonObject getBackupSettings() const;

    // Get the configured backup directory
    QString getBackupDirectory() const;

    // Set a new backup directory
    void setBackupDirectory(const QString& dir);

    // Get the configured backup prefix
    QString getBackupPrefix() const;

    // Set a new backup prefix
    void setBackupPrefix(const QString& prefix);

private:
    // Update a specific setting inside the backup group
    void updateBackupSetting(const QString& key, const QString& newValue, const QString& defaultValue);

    UserServiceManager& userServiceManager;
};

#endif // BACKUPSERVICEMANAGER_H
