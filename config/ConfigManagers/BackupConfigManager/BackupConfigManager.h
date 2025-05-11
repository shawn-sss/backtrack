#ifndef BACKUPCONFIGMANAGER_H
#define BACKUPCONFIGMANAGER_H

// Qt includes
#include <QJsonObject>
#include <QString>

// Forward Declaration (Custom class)
class UserConfigManager;

class BackupConfigManager {
public:
    // Constructor
    explicit BackupConfigManager(UserConfigManager& configManager);

    // Access to backup configuration settings
    QJsonObject getBackupSettings() const;
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

private:
    // Internal update helper
    void updateBackupSetting(const QString& key, const QString& newValue, const QString& defaultValue);

    // Reference to user config manager
    UserConfigManager& userConfigManager;
};

#endif // BACKUPCONFIGMANAGER_H
