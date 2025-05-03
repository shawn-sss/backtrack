#ifndef CONFIGDIRECTOR_H
#define CONFIGDIRECTOR_H

// Project includes
#include "../../config/configsettings/_settings.h"

// Qt includes
#include <QString>

// Forward declarations
class InstallMetadataManager;
class UserSettingsManager;
class BackupSettingsManager;

// Manages application configuration
class ConfigDirector {
public:
    static ConfigDirector& getInstance(); // Singleton access

    // Backup settings
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

    // Theme preference
    UserThemePreference getThemePreference() const;
    void setThemePreference(UserThemePreference preference);

    // Install metadata
    void loadInstallMetadata();
    void saveInstallMetadata();

    // File path access
    QString getAppInstallDirPublic() const;
    QString getAppInstallDir() const;
    QString getFilePath(const QString& fileName) const;

private:
    ConfigDirector(); // Private constructor for singleton

    // Helpers for internal setup
    void setupFilePaths();
    bool isFirstRun() const;
    void setupDefaults();

    // Cached config file paths
    QString appMetadataPath;
    QString userConfigPath;

    // Configuration managers
    InstallMetadataManager* installMetadataManager;
    UserSettingsManager* userSettingsManager;
    BackupSettingsManager* backupSettingsManager;
};

#endif // CONFIGDIRECTOR_H
