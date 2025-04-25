#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

// Project includes
#include "../../core/utils/file_utils/jsonmanager.h"
#include "../thememanager/thememanager.h"

// Qt includes
#include <QString>
#include <QJsonObject>

class ConfigManager {
public:
    // Singleton instance
    static ConfigManager& getInstance();

    // First-run check
    bool isFirstRun() const;

    // Public install directory accessor
    QString getAppInstallDirPublic() const;

    // Install metadata management
    void loadInstallMetadata();
    void saveInstallMetadata();

    // User configuration management
    void loadUserConfig();
    void saveUserConfig();

    // Theme configuration management
    UserThemePreference getThemePreference() const;
    void setThemePreference(UserThemePreference preference);

    // Backup settings management
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

private:
    // Constructor
    ConfigManager();

    // Default setup
    void setupDefaults();

    // Internal path management
    void setupFilePaths();
    QString getFilePath(const QString& fileName) const;
    QString getAppInstallDir() const;

    // Internal data storage
    QJsonObject installMetadata;
    QJsonObject userSettings;

    // Cached JSON file paths
    QString appMetadataPath;
    QString userConfigPath;
};

#endif // CONFIGMANAGER_H
