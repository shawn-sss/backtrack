#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

// Built-in Qt includes
#include <QString>
#include <QJsonObject>

class ConfigManager {
public:
    // Singleton instance
    static ConfigManager& getInstance();

    // Install metadata management
    void loadInstallMetadata();
    void saveInstallMetadata();

    // User configuration management
    void loadUserConfig();
    void saveUserConfig();

    // First-run check
    bool isFirstRun() const;

    // Backup settings management
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

    // Public install directory accessor
    QString getAppInstallDirPublic() const;

private:
    // Constructor
    ConfigManager();

    // Default setup
    void setupDefaults();

    // Internal path management
    QString getFilePath(const QString& fileName) const;
    QString getAppInstallDir() const;

    // JSON file handling
    bool saveJsonFile(const QString& path, const QJsonObject& data);
    bool loadJsonFile(const QString& path, QJsonObject& target);

    // Internal data storage
    QJsonObject installMetadata;
    QJsonObject userSettings;
};

#endif // CONFIGMANAGER_H
