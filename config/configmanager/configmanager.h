#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QJsonObject>

class ConfigManager {
private:
    // User Settings
    static constexpr auto DEFAULT_BACKUP_DIRECTORY = "C:\\temp";
    static constexpr auto DEFAULT_BACKUP_PREFIX = "Backup_";

public:
    static ConfigManager& getInstance();

    // Configuration Management
    void load();
    void save();

    // Backup Directory Management
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);

    // Backup Prefix Management
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

    // Config File Path
    QString getConfigFilePathPublic() const;

private:
    ConfigManager();
    QString getConfigFilePath() const;
    QJsonObject settings;
};

#endif // CONFIGMANAGER_H
