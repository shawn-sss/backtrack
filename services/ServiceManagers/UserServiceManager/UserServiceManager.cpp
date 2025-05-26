// Project includes
#include "UserServiceManager.h"
#include "UserServiceConstants.h"
#include "../JsonServiceManager/JsonServiceManager.h"
#include "../PathServiceManager/PathServiceManager.h"
#include "../ThemeServiceManager/ThemeServiceConstants.h"

// Initializes the user service manager with the settings file path
UserServiceManager::UserServiceManager(const QString& serviceFilePath)
    : userServicePath(serviceFilePath) {}

// Loads user settings from the settings file
void UserServiceManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(userServicePath, rootObject) && !rootObject.isEmpty())
        userSettings = rootObject;
}

// Saves current user settings to the settings file
void UserServiceManager::save() const {
    JsonManager::saveJsonFile(userServicePath, userSettings);
}

// Returns a modifiable reference to user settings
QJsonObject& UserServiceManager::settings() {
    return userSettings;
}

// Returns a const reference to user settings
const QJsonObject& UserServiceManager::settings() const {
    return userSettings;
}

// Initializes and saves default values for user settings
void UserServiceManager::initializeDefaults() {
    QJsonObject backupGroup{
        { UserServiceKeys::k_BACKUP_DIRECTORY_KEY, PathServiceManager::backupSetupFolderPath() },
        { UserServiceKeys::k_BACKUP_PREFIX_KEY,    UserServiceDefaults::k_BACKUP_PREFIX }
    };

    QJsonObject userService;
    userService[UserServiceKeys::k_BACKUP_SERVICE_GROUP] = backupGroup;
    userService[UserServiceKeys::k_THEME_PREFERENCE_KEY] =
        userThemePreferenceToString(ThemeServiceConstants::UserThemePreference::Auto);
    userService[UserServiceKeys::k_MINIMIZE_ON_CLOSE_KEY] = true;

    JsonManager::saveJsonFile(userServicePath, userService);
    userSettings = std::move(userService);
}
