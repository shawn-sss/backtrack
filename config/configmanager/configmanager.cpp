// Project includes
#include "configmanager.h"
#include "../_constants.h"
#include "../backupsettingsmanager/backupsettingsmanager.h"
#include "../installmetadatamanager/installmetadatamanager.h"
#include "../thememanager/themeutils.h"
#include "../usersettingsmanager/usersettingsmanager.h"
#include "../../core/utils/file_utils/jsonmanager.h"

// Qt includes
#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <QDateTime>
#include <QStandardPaths>
#include <QJsonDocument>

// Singleton access
ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

// Constructor
ConfigManager::ConfigManager()
    : installMetadataManager(nullptr)
    , userSettingsManager(nullptr)
    , backupSettingsManager(nullptr)
{
    setupFilePaths();

    installMetadataManager = new InstallMetadataManager(appMetadataPath);
    userSettingsManager = new UserSettingsManager(userConfigPath);
    backupSettingsManager = new BackupSettingsManager(*userSettingsManager);

    if (isFirstRun()) {
        setupDefaults();
    }

    installMetadataManager->load();
    userSettingsManager->load();
}

// Backup settings management
QString ConfigManager::getBackupDirectory() const {
    return backupSettingsManager->getBackupDirectory();
}

void ConfigManager::setBackupDirectory(const QString& dir) {
    backupSettingsManager->setBackupDirectory(dir);
}

QString ConfigManager::getBackupPrefix() const {
    return backupSettingsManager->getBackupPrefix();
}

void ConfigManager::setBackupPrefix(const QString& prefix) {
    backupSettingsManager->setBackupPrefix(prefix);
}

// Theme preference management
UserThemePreference ConfigManager::getThemePreference() const {
    const QJsonObject& settings = userSettingsManager->settings();
    if (settings.contains(ConfigKeys::k_THEME_PREFERENCE_KEY)) {
        const QString prefStr = settings.value(ConfigKeys::k_THEME_PREFERENCE_KEY).toString();
        return stringToUserThemePreference(prefStr);
    }
    return UserThemePreference::Auto;
}

void ConfigManager::setThemePreference(UserThemePreference preference) {
    QJsonObject& settings = userSettingsManager->settings();
    settings[ConfigKeys::k_THEME_PREFERENCE_KEY] = userThemePreferenceToString(preference);
    userSettingsManager->save();
}

// Install metadata management
void ConfigManager::loadInstallMetadata() {
    installMetadataManager->load();
}

void ConfigManager::saveInstallMetadata() {
    installMetadataManager->save();
}

// Application paths management
QString ConfigManager::getAppInstallDirPublic() const {
    return getAppInstallDir();
}

QString ConfigManager::getAppInstallDir() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

QString ConfigManager::getFilePath(const QString& fileName) const {
    return getAppInstallDir() + "/" + AppConfig::k_APPDATA_SETUP_FOLDER + "/" + fileName;
}

// Internal helpers
void ConfigManager::setupFilePaths() {
    appMetadataPath = getFilePath(AppConfig::k_APPDATA_SETUP_INFO_FILE);
    userConfigPath = getFilePath(AppConfig::k_APPDATA_SETUP_USER_SETTINGS_FILE);
}

bool ConfigManager::isFirstRun() const {
    return !QFile::exists(appMetadataPath) || !QFile::exists(userConfigPath);
}

void ConfigManager::setupDefaults() {
    installMetadataManager->setupDefaults(getAppInstallDir());

    QJsonObject userConfig;
    userConfig[ConfigKeys::k_BACKUP_CONFIG_GROUP] = QJsonObject{
        {ConfigKeys::k_BACKUP_DIRECTORY_KEY, ConfigDefaults::k_BACKUP_DIRECTORY},
        {ConfigKeys::k_BACKUP_PREFIX_KEY, ConfigDefaults::k_BACKUP_PREFIX}
    };

    userConfig[ConfigKeys::k_THEME_PREFERENCE_KEY] = userThemePreferenceToString(UserThemePreference::Auto);

    JsonManager::saveJsonFile(userConfigPath, userConfig);
}
