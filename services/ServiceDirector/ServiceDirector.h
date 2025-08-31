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

namespace ServiceDirectorConstants {
inline constexpr int kImmediateMs = 0;
}

class ServiceDirector {
public:
    // Lifecycle
    static ServiceDirector& getInstance();

    // Theme
    void applyTheme();
    void installThemeEventFilter(QObject* target);
    ThemeServiceConstants::UserThemePreference getThemePreference() const;
    void setThemePreference(ThemeServiceConstants::UserThemePreference preference);

    // Install metadata
    void loadInstallMetadata();
    void saveInstallMetadata();

    // Accessors
    UserServiceManager* getUserServiceManager();
    BackupServiceManager* getBackupServiceManager();
    InstallServiceManager* getInstallServiceManager();
    ThemeServiceManager* getThemeServiceManager();

    // First run
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
