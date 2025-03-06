#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QJsonObject>

class ConfigManager {
public:
    static ConfigManager& getInstance();

    // Metadata management (App installation details)
    void loadInstallMetadata();
    void saveInstallMetadata();
    QString getInstallMetadataFilePathPublic() const;

    // User configuration management (Backup settings)
    void loadUserConfig();
    void saveUserConfig();
    QString getUserConfigFilePathPublic() const;

    // First-run check (both files required)
    bool isFirstRun() const;

    // Backup directory management (from user config)
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);

    // Backup prefix management (from user config)
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

private:
    ConfigManager();  // Singleton private constructor

    // Internal file path helpers
    QString getInstallMetadataFilePath() const;
    QString getUserConfigFilePath() const;
    QString getAppInstallDir() const;

    // Default setup for first run
    void setupDefaults();

    // Internal data storage
    QJsonObject installMetadata;  // App install metadata
    QJsonObject userSettings;     // Backup settings
};

#endif // CONFIGMANAGER_H
