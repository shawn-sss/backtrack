#ifndef SERVICEDIRECTOR_H
#define SERVICEDIRECTOR_H

// Project includes
#include "../ServiceManagers/ThemeServiceManager/ThemeServiceConstants.h"

// Qt includes
#include <QString>
#include <QWidget>

// C++ includes
#include <memory>

// Forward declarations
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

    // Theme
    ThemeServiceConstants::UserThemePreference getThemePreference() const;
    void setThemePreference(ThemeServiceConstants::UserThemePreference preference);

    // Install metadata
    void loadInstallMetadata();
    void saveInstallMetadata();

    // Filesystem
    QString getAppInstallDir() const;
    QString getFilePath(const QString& fileName) const;
    inline QString getAppInstallDirPublic() const { return getAppInstallDir(); }

    // Uninstall
    bool uninstallAppWithConfirmation(QWidget* parent);

private:
    ServiceDirector();
    void setupFilePaths();
    bool isFirstRun() const;
    void setupDefaults();

    QString appMetadataPath;
    QString userServicePath;

    std::unique_ptr<InstallServiceManager> installServiceManager;
    std::unique_ptr<UserServiceManager> userServiceManager;
    std::unique_ptr<BackupServiceManager> backupServiceManager;
    std::unique_ptr<UninstallServiceManager> uninstallServiceManager;
};

#endif // SERVICEDIRECTOR_H
