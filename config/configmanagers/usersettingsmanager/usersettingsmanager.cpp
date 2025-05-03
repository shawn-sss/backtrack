// Project includes
#include "../../../core/utils/file_utils/jsonmanager.h"
#include "usersettingsmanager.h"

// Constructor
UserSettingsManager::UserSettingsManager(const QString& configFilePath)
    : userConfigPath(configFilePath) {}

// Load user settings from file
void UserSettingsManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(userConfigPath, rootObject) && !rootObject.isEmpty()) {
        userSettings = rootObject;
    }
}

// Save user settings to file
void UserSettingsManager::save() const {
    JsonManager::saveJsonFile(userConfigPath, userSettings);
}

// Return a modifiable reference to user settings
QJsonObject& UserSettingsManager::settings() {
    return userSettings;
}

// Return a const reference to user settings
const QJsonObject& UserSettingsManager::settings() const {
    return userSettings;
}
