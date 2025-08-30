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
    explicit BackupServiceManager(UserServiceManager& serviceManager);

    // Access backup configuration
    [[nodiscard]] QJsonObject getBackupSettings() const;

    // Backup directory
    [[nodiscard]] QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);

    // Backup prefix
    [[nodiscard]] QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

private:
    // Internal persistence helper
    void updateBackupSetting(const QString& key, const QString& newValue, const QString& defaultValue);

    UserServiceManager& userServiceManager;
};

#endif
