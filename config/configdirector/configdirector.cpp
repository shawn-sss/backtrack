// Project includes
#include "ConfigDirector.h"
#include "ConfigDirectorConstants.h"
#include "../ConfigManagers/BackupConfigManager/BackupConfigManager.h"
#include "../ConfigManagers/InstallConfigManager/InstallConfigManager.h"
#include "../ConfigManagers/NotificationConfigManager/NotificationConfigManager.h"
#include "../ConfigManagers/UserConfigManager/UserConfigManager.h"
#include "../ConfigManagers/UserConfigManager/UserConfigConstants.h"
#include "../ConfigManagers/ThemeConfigManager/ThemeConfigConstants.h"

// Qt includes
#include <QFile>
#include <QJsonObject>
#include <QStandardPaths>

using namespace UserConfigKeys;
using namespace ThemeConfigConstants;

// Get singleton instance of ConfigDirector
ConfigDirector& ConfigDirector::getInstance() {
    static ConfigDirector instance;
    return instance;
}

// Constructor sets up config paths and managers
ConfigDirector::ConfigDirector() {
    setupFilePaths();

    installConfigManager = std::make_unique<InstallConfigManager>(appMetadataPath);
    userConfigManager = std::make_unique<UserConfigManager>(userConfigPath);
    backupConfigManager = std::make_unique<BackupConfigManager>(*userConfigManager);

    if (isFirstRun()) {
        setupDefaults();
    }

    installConfigManager->load();
    userConfigManager->load();
}

// Returns backup directory path
QString ConfigDirector::getBackupDirectory() const {
    return backupConfigManager->getBackupDirectory();
}

// Sets backup directory path
void ConfigDirector::setBackupDirectory(const QString& dir) {
    backupConfigManager->setBackupDirectory(dir);
}

// Returns backup filename prefix
QString ConfigDirector::getBackupPrefix() const {
    return backupConfigManager->getBackupPrefix();
}

// Sets backup filename prefix
void ConfigDirector::setBackupPrefix(const QString& prefix) {
    backupConfigManager->setBackupPrefix(prefix);
}

// Returns user theme preference from config
UserThemePreference ConfigDirector::getThemePreference() const {
    const QJsonObject& settings = userConfigManager->settings();
    if (settings.contains(k_THEME_PREFERENCE_KEY)) {
        return stringToUserThemePreference(settings.value(k_THEME_PREFERENCE_KEY).toString());
    }
    return UserThemePreference::Auto;
}

// Updates theme preference in user config
void ConfigDirector::setThemePreference(UserThemePreference preference) {
    QJsonObject& settings = userConfigManager->settings();
    settings[k_THEME_PREFERENCE_KEY] = userThemePreferenceToString(preference);
    userConfigManager->save();
}

// Loads install metadata from config
void ConfigDirector::loadInstallMetadata() {
    installConfigManager->load();
}

// Saves install metadata to config
void ConfigDirector::saveInstallMetadata() {
    installConfigManager->save();
}

// Returns app installation directory path
QString ConfigDirector::getAppInstallDir() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Returns full file path within app config directory
QString ConfigDirector::getFilePath(const QString& fileName) const {
    return getAppInstallDir() + "/" + ConfigDirectorConstants::kAppDataFolder + "/" + fileName;
}

// Initializes file paths for metadata and user config
void ConfigDirector::setupFilePaths() {
    const QString baseDir = getAppInstallDir() + "/" + ConfigDirectorConstants::kAppDataFolder;
    appMetadataPath = baseDir + "/" + ConfigDirectorConstants::kMetadataFile;
    userConfigPath = baseDir + "/" + ConfigDirectorConstants::kUserSettingsFile;
}

// Checks if this is the first run (missing config files)
bool ConfigDirector::isFirstRun() const {
    return !QFile::exists(appMetadataPath) || !QFile::exists(userConfigPath);
}

// Sets default configurations on first run
void ConfigDirector::setupDefaults() {
    InstallConfigManager::initializeDefaults();
    userConfigManager->initializeDefaults();
    NotificationConfigManager::initializeDefaults();
}
