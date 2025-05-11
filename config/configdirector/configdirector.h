#ifndef CONFIGDIRECTOR_H
#define CONFIGDIRECTOR_H

// Project includes
#include "../../config/configsettings/app_settings.h"
#include "ConfigDirectorConstants.h"
#include "../ConfigManagers/ThemeConfigManager/ThemeConfigConstants.h"

// Qt includes
#include <QString>
#include <memory>

// Forward Declaration (Custom class)
class BackupConfigManager;
class InstallConfigManager;
class NotificationConfigManager;
class UserConfigManager;

class ConfigDirector {
public:
    // Singleton access
    static ConfigDirector& getInstance();

    // Backup settings access
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

    // User theme preference
    ThemeConfigConstants::UserThemePreference getThemePreference() const;
    void setThemePreference(ThemeConfigConstants::UserThemePreference preference);

    // Install metadata
    void loadInstallMetadata();
    void saveInstallMetadata();

    // File system access
    QString getAppInstallDir() const;
    QString getFilePath(const QString& fileName) const;

    // Public alias for install directory
    inline QString getAppInstallDirPublic() const { return getAppInstallDir(); }

private:
    // Private constructor for singleton
    ConfigDirector();

    // Setup and internal helpers
    void setupFilePaths();
    bool isFirstRun() const;
    void setupDefaults();

    // File paths
    QString appMetadataPath;
    QString userConfigPath;

    // Config managers
    std::unique_ptr<InstallConfigManager> installConfigManager;
    std::unique_ptr<UserConfigManager> userConfigManager;
    std::unique_ptr<BackupConfigManager> backupConfigManager;
};

#endif // CONFIGDIRECTOR_H
