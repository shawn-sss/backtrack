#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QJsonObject>

// Constants for default values
class ConfigManager {
private:
    static constexpr auto DEFAULT_BACKUP_DIRECTORY = "C:\\temp";
    static constexpr auto DEFAULT_BACKUP_PREFIX = "Backup_";

    // Singleton instance management
public:
    static ConfigManager& getInstance();

    // Configuration handling
public:
    void load();
    void save();

    // Backup directory management
public:
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);

    // Backup prefix management
public:
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

    // Config file path access
public:
    QString getConfigFilePathPublic() const;

    // Internal/private methods and members
private:
    ConfigManager();
    QString getConfigFilePath() const;
    QJsonObject settings;
};

#endif // CONFIGMANAGER_H
