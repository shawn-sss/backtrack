#ifndef SERVICEDIRECTOR_H
#define SERVICEDIRECTOR_H

// Project includes
#include "../ServiceManagers/ThemeServiceManager/ThemeServiceConstants.h"

// Qt includes

// C++ includes
#include <memory>

// Forward declaration (Custom class)
class InstallServiceManager;
class UserServiceManager;
class BackupServiceManager;
class ThemeServiceManager;

// Forward declaration (Qt class)
class QObject;
class QWidget;

// Lifecycle
class ServiceDirector {
public:
    static ServiceDirector& getInstance();

    // Theme handling
    void applyTheme();
    void installThemeEventFilter(QObject* target);

    // Theme preference access
    ThemeServiceConstants::UserThemePreference getThemePreference() const;
    void setThemePreference(ThemeServiceConstants::UserThemePreference preference);

    // Install metadata operations
    void loadInstallMetadata();
    void saveInstallMetadata();

    // Service manager accessors
    UserServiceManager* getUserServiceManager();
    BackupServiceManager* getBackupServiceManager();
    InstallServiceManager* getInstallServiceManager();
    ThemeServiceManager* getThemeServiceManager();

    // First-run detection and onboarding
    bool isFirstRun() const;
    void maybeShowWelcomeDialog(QWidget* parent = nullptr);

private:
    ServiceDirector();
    void setupDefaults();

    std::unique_ptr<InstallServiceManager> installServiceManager;
    std::unique_ptr<UserServiceManager> userServiceManager;
    std::unique_ptr<BackupServiceManager> backupServiceManager;

    bool firstRun = false;
};

#endif
