// Project includes
#include "UserServiceManager.h"
#include "UserServiceConstants.h"
#include "../JsonServiceManager/JsonServiceManager.h"
#include "../PathServiceManager/PathServiceManager.h"
#include "../ThemeServiceManager/ThemeServiceConstants.h"

// Initializes the user service manager
UserServiceManager::UserServiceManager(const QString& serviceFilePath)
    : userServicePath(serviceFilePath) {}

// Loads user settings from file
void UserServiceManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(userServicePath, rootObject) && !rootObject.isEmpty())
        userSettings = rootObject;
}

// Saves user settings to file
void UserServiceManager::save() const {
    JsonManager::saveJsonFile(userServicePath, userSettings);
}

// Initializes and saves default user settings
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

// Returns modifiable user settings
QJsonObject& UserServiceManager::settings() {
    return userSettings;
}

// Returns read-only user settings
const QJsonObject& UserServiceManager::settings() const {
    return userSettings;
}
