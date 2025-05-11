// Project includes
#include "UserConfigManager.h"
#include "UserConfigConstants.h"
#include "../../configsettings/app_settings.h"
#include "../../ConfigManagers/ThemeConfigManager/ThemeConfigConstants.h"
#include "../../../core/utils/file_utils/jsonmanager.h"

using namespace ThemeConfigConstants;

// Constructor that sets the path to the user config file
UserConfigManager::UserConfigManager(const QString& configFilePath)
    : userConfigPath(configFilePath) {}

// Loads user settings from the configuration file
void UserConfigManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(userConfigPath, rootObject) && !rootObject.isEmpty()) {
        userSettings = rootObject;
    }
}

// Saves current user settings to the configuration file
void UserConfigManager::save() const {
    JsonManager::saveJsonFile(userConfigPath, userSettings);
}

// Returns a modifiable reference to the user settings object
QJsonObject& UserConfigManager::settings() {
    return userSettings;
}

// Returns a const reference to the user settings object
const QJsonObject& UserConfigManager::settings() const {
    return userSettings;
}

// Initializes default settings and saves them to disk
void UserConfigManager::initializeDefaults() {
    QJsonObject userConfig;
    userConfig[UserConfigKeys::k_BACKUP_CONFIG_GROUP] = QJsonObject{
        { UserConfigKeys::k_BACKUP_DIRECTORY_KEY, UserConfigDefaults::k_BACKUP_DIRECTORY },
        { UserConfigKeys::k_BACKUP_PREFIX_KEY, UserConfigDefaults::k_BACKUP_PREFIX }
    };
    userConfig[UserConfigKeys::k_THEME_PREFERENCE_KEY] =
        userThemePreferenceToString(UserThemePreference::Auto);

    JsonManager::saveJsonFile(userConfigPath, userConfig);
    userSettings = userConfig;
}
