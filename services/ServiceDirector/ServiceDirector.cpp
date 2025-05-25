// Project includes
#include "ServiceDirector.h"
#include "ServiceDirectorConstants.h"
#include "../ServiceManagers/BackupServiceManager/BackupServiceManager.h"
#include "../ServiceManagers/InstallServiceManager/InstallServiceManager.h"
#include "../ServiceManagers/NotificationServiceManager/NotificationServiceManager.h"
#include "../ServiceManagers/UninstallServiceManager/UninstallServiceManager.h"
#include "../ServiceManagers/UserServiceManager/UserServiceManager.h"
#include "../ServiceManagers/UserServiceManager/UserServiceConstants.h"

// Qt includes
#include <QFile>
#include <QJsonObject>
#include <QStandardPaths>

// C++ includes

// Forward declaration (Custom class)

// Forward declaration (Qt class)

using namespace ThemeServiceConstants;

// Singleton accessor
ServiceDirector& ServiceDirector::getInstance() {
    static ServiceDirector instance;
    return instance;
}

// Constructor
ServiceDirector::ServiceDirector() {
    setupFilePaths();
    installServiceManager = std::make_unique<InstallServiceManager>(appMetadataPath);
    userServiceManager = std::make_unique<UserServiceManager>(userServicePath);
    backupServiceManager = std::make_unique<BackupServiceManager>(*userServiceManager);
    uninstallServiceManager = std::make_unique<UninstallServiceManager>();
    if (isFirstRun()) setupDefaults();
    installServiceManager->load();
    userServiceManager->load();
}

// Theme preference operations
UserThemePreference ServiceDirector::getThemePreference() const {
    const QJsonObject& settings = userServiceManager->settings();
    const auto key = UserServiceKeys::k_THEME_PREFERENCE_KEY;
    return settings.contains(key)
               ? stringToUserThemePreference(settings.value(key).toString())
               : UserThemePreference::Auto;
}

void ServiceDirector::setThemePreference(UserThemePreference preference) {
    QJsonObject& settings = userServiceManager->settings();
    settings[UserServiceKeys::k_THEME_PREFERENCE_KEY] = userThemePreferenceToString(preference);
    userServiceManager->save();
}

// Backup service operations
QString ServiceDirector::getBackupDirectory() const {
    return backupServiceManager->getBackupDirectory();
}

void ServiceDirector::setBackupDirectory(const QString& dir) {
    backupServiceManager->setBackupDirectory(dir);
}

QString ServiceDirector::getBackupPrefix() const {
    return backupServiceManager->getBackupPrefix();
}

void ServiceDirector::setBackupPrefix(const QString& prefix) {
    backupServiceManager->setBackupPrefix(prefix);
}

// Install metadata operations
void ServiceDirector::loadInstallMetadata() {
    installServiceManager->load();
}

void ServiceDirector::saveInstallMetadata() {
    installServiceManager->save();
}

// Uninstall operations
bool ServiceDirector::uninstallAppWithConfirmation(QWidget* parent) {
    if (!uninstallServiceManager || !uninstallServiceManager->confirmUninstall(parent))
        return false;
    return uninstallServiceManager->performUninstall();
}

UninstallServiceManager* ServiceDirector::getUninstallServiceManager() {
    return uninstallServiceManager.get();
}

const UninstallServiceManager* ServiceDirector::getUninstallServiceManager() const {
    return uninstallServiceManager.get();
}

// Filesystem path accessors
QString ServiceDirector::getAppInstallDir() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

QString ServiceDirector::getFilePath(const QString& fileName) const {
    return getAppInstallDir() + "/" + ServiceDirectorConstants::kAppDataFolder + "/" + fileName;
}

// Internal helper methods
void ServiceDirector::setupFilePaths() {
    const QString baseDir = getFilePath("");
    appMetadataPath = baseDir + ServiceDirectorConstants::kMetadataFile;
    userServicePath = baseDir + ServiceDirectorConstants::kUserSettingsFile;
}

bool ServiceDirector::isFirstRun() const {
    return !QFile::exists(appMetadataPath) || !QFile::exists(userServicePath);
}

void ServiceDirector::setupDefaults() {
    InstallServiceManager::initializeDefaults();
    userServiceManager->initializeDefaults();
    NotificationServiceManager::initializeDefaults();
}
