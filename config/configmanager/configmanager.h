#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

// Qt includes
#include <QString>

// Project includes
#include "../thememanager/themeutils.h"

// Forward declarations
class InstallMetadataManager;
class UserSettingsManager;
class BackupSettingsManager;

// Manages application configuration
class ConfigManager {
public:
    static ConfigManager& getInstance(); // Singleton access

    // Backup settings management
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

    // Theme preference management
    UserThemePreference getThemePreference() const;
    void setThemePreference(UserThemePreference preference);

    // Install metadata management
    void loadInstallMetadata();
    void saveInstallMetadata();

    // Application paths management
    QString getAppInstallDirPublic() const;
    QString getAppInstallDir() const;
    QString getFilePath(const QString& fileName) const;

private:
    ConfigManager(); // Constructor

    // Internal helpers
    void setupFilePaths();
    bool isFirstRun() const;
    void setupDefaults();

    // File paths
    QString appMetadataPath;
    QString userConfigPath;

    // Sub-managers
    InstallMetadataManager* installMetadataManager;
    UserSettingsManager* userSettingsManager;
    BackupSettingsManager* backupSettingsManager;
};

#endif // CONFIGMANAGER_H
