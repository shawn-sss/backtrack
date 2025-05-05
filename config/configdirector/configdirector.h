#ifndef CONFIGDIRECTOR_H
#define CONFIGDIRECTOR_H

// Project includes
#include "../../config/configsettings/_settings.h"

// Qt includes
#include <QString>

// Forward Declaration (Custom class)
class BackupSettingsManager;
class InstallMetadataManager;
class UserSettingsManager;

// Manages application configuration as a singleton
class ConfigDirector {
public:
    static ConfigDirector& getInstance();

    // Backup settings access and control
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

    // Theme preference management
    UserThemePreference getThemePreference() const;
    void setThemePreference(UserThemePreference preference);

    // Install metadata lifecycle
    void loadInstallMetadata();
    void saveInstallMetadata();

    // Application file path queries
    QString getAppInstallDirPublic() const;
    QString getAppInstallDir() const;
    QString getFilePath(const QString& fileName) const;

private:
    ConfigDirector();

    // Internal setup and lifecycle checks
    void setupFilePaths();
    bool isFirstRun() const;
    void setupDefaults();

    QString appMetadataPath;
    QString userConfigPath;

    InstallMetadataManager* installMetadataManager;
    UserSettingsManager* userSettingsManager;
    BackupSettingsManager* backupSettingsManager;
};

#endif // CONFIGDIRECTOR_H
