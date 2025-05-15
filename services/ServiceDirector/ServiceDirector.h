#ifndef SERVICEDIRECTOR_H
#define SERVICEDIRECTOR_H

// Project includes
#include "../ServiceManagers/ThemeServiceManager/ThemeServiceConstants.h"
#include "../ServiceManagers/UninstallServiceManager/UninstallServiceManager.h"


// Qt includes
#include <QString>
#include <memory>
#include <QWidget>

// Forward declarations (Custom class)
class BackupServiceManager;
class InstallServiceManager;
class NotificationServiceManager;
class UserServiceManager;
class UninstallServiceManager;

// Coordinates access to core service managers and configuration
class ServiceDirector {
public:
    // Singleton access
    static ServiceDirector& getInstance();

    // Backup settings access
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

    // User theme preference
    ThemeServiceConstants::UserThemePreference getThemePreference() const;
    void setThemePreference(ThemeServiceConstants::UserThemePreference preference);

    // Install metadata
    void loadInstallMetadata();
    void saveInstallMetadata();

    // File system access
    QString getAppInstallDir() const;
    QString getFilePath(const QString& fileName) const;

    // Public alias for install directory
    inline QString getAppInstallDirPublic() const { return getAppInstallDir(); }

    bool uninstallAppWithConfirmation(QWidget* parent);

private:
    // Private constructor for singleton
    ServiceDirector();

    // Setup and internal helpers
    void setupFilePaths();
    bool isFirstRun() const;
    void setupDefaults();

    // File paths
    QString appMetadataPath;
    QString userServicePath;

    // Service managers
    std::unique_ptr<InstallServiceManager> installServiceManager;
    std::unique_ptr<UserServiceManager> userServiceManager;
    std::unique_ptr<BackupServiceManager> backupServiceManager;
    std::unique_ptr<UninstallServiceManager> uninstallServiceManager;
};

#endif // SERVICEDIRECTOR_H
