// ServiceDirector.h
#ifndef SERVICEDIRECTOR_H
#define SERVICEDIRECTOR_H

// Project includes
#include "../ServiceManagers/ThemeServiceManager/ThemeServiceConstants.h"

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

// Small constants
namespace ServiceDirectorConstants {
inline constexpr int kImmediateMs = 0;
}

class ServiceDirector {
public:
    // Lifecycle
    static ServiceDirector& getInstance();

    // Theme application & event filtering
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

    // First-run & onboarding
    bool isFirstRun() const;
    void maybeShowWelcomeDialog(QWidget* parent = nullptr);

private:
    // Construction & defaults
    ServiceDirector();
    void setupDefaults();

    std::unique_ptr<InstallServiceManager> installServiceManager;
    std::unique_ptr<UserServiceManager> userServiceManager;
    std::unique_ptr<BackupServiceManager> backupServiceManager;

    bool firstRun = false;
};

#endif // SERVICEDIRECTOR_H
