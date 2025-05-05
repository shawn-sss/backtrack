// Project includes
#include "../../configsettings/_settings.h"
#include "usersettingsmanager.h"
#include "../../../core/utils/file_utils/jsonmanager.h"

// Constructor that sets the path to the user config file
UserSettingsManager::UserSettingsManager(const QString& configFilePath)
    : userConfigPath(configFilePath) {}

// Loads user settings from the configuration file
void UserSettingsManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(userConfigPath, rootObject) && !rootObject.isEmpty()) {
        userSettings = rootObject;
    }
}

// Saves current user settings to the configuration file
void UserSettingsManager::save() const {
    JsonManager::saveJsonFile(userConfigPath, userSettings);
}

// Returns a modifiable reference to the user settings object
QJsonObject& UserSettingsManager::settings() {
    return userSettings;
}

// Returns a const reference to the user settings object
const QJsonObject& UserSettingsManager::settings() const {
    return userSettings;
}

// Initializes default settings and saves them to disk
void UserSettingsManager::initializeDefaults() {
    QJsonObject userConfig;
    userConfig[ConfigKeys::k_BACKUP_CONFIG_GROUP] = QJsonObject{
        {ConfigKeys::k_BACKUP_DIRECTORY_KEY, ConfigDefaults::k_BACKUP_DIRECTORY},
        {ConfigKeys::k_BACKUP_PREFIX_KEY, ConfigDefaults::k_BACKUP_PREFIX}
    };
    userConfig[ConfigKeys::k_THEME_PREFERENCE_KEY] = userThemePreferenceToString(UserThemePreference::Auto);

    JsonManager::saveJsonFile(userConfigPath, userConfig);
    userSettings = userConfig;
}
