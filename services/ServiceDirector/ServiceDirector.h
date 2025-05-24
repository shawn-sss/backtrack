#ifndef SERVICEDIRECTOR_H
#define SERVICEDIRECTOR_H

// Project includes
#include "../ServiceManagers/ThemeServiceManager/ThemeServiceConstants.h"

// Qt includes
#include <QString>
#include <QWidget>

// C++ includes
#include <memory>

// Forward declaration (Custom class)
class InstallServiceManager;
class UserServiceManager;
class BackupServiceManager;
class UninstallServiceManager;

// Manages centralized access to service layer
class ServiceDirector {
public:
    static ServiceDirector& getInstance();

    // Backup service
    QString getBackupDirectory() const;
    void setBackupDirectory(const QString& dir);
    QString getBackupPrefix() const;
    void setBackupPrefix(const QString& prefix);

    // Theme management
    ThemeServiceConstants::UserThemePreference getThemePreference() const;
    void setThemePreference(ThemeServiceConstants::UserThemePreference preference);

    // Install metadata management
    void loadInstallMetadata();
    void saveInstallMetadata();

    // Filesystem access
    QString getAppInstallDir() const;
    QString getFilePath(const QString& fileName) const;
    inline QString getAppInstallDirPublic() const { return getAppInstallDir(); }

    // Uninstallation
    bool uninstallAppWithConfirmation(QWidget* parent);
    UninstallServiceManager* getUninstallServiceManager();
    const UninstallServiceManager* getUninstallServiceManager() const;

private:
    ServiceDirector();

    void setupFilePaths();
    void setupDefaults();
    bool isFirstRun() const;

    QString appMetadataPath;
    QString userServicePath;

    std::unique_ptr<InstallServiceManager> installServiceManager;
    std::unique_ptr<UserServiceManager> userServiceManager;
    std::unique_ptr<BackupServiceManager> backupServiceManager;
    std::unique_ptr<UninstallServiceManager> uninstallServiceManager;
};

#endif // SERVICEDIRECTOR_H
