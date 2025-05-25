// Project includes
#include "UserServiceManager.h"
#include "UserServiceConstants.h"
#include "../JsonServiceManager/JsonServiceManager.h"
#include "../PathServiceManager/PathServiceManager.h"
#include "../ThemeServiceManager/ThemeServiceConstants.h"

using namespace ThemeServiceConstants;

// Constructs the user service manager with a path to the settings file
UserServiceManager::UserServiceManager(const QString& serviceFilePath)
    : userServicePath(serviceFilePath) {}

// Loads settings from file
void UserServiceManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(userServicePath, rootObject) && !rootObject.isEmpty())
        userSettings = rootObject;
}

// Saves settings to file
void UserServiceManager::save() const {
    JsonManager::saveJsonFile(userServicePath, userSettings);
}

// Returns a modifiable reference to the settings object
QJsonObject& UserServiceManager::settings() {
    return userSettings;
}

// Returns a const reference to the settings object
const QJsonObject& UserServiceManager::settings() const {
    return userSettings;
}

// Initializes default values for user settings
void UserServiceManager::initializeDefaults() {
    QJsonObject backupGroup{
        { UserServiceKeys::k_BACKUP_DIRECTORY_KEY, PathServiceManager::backupSetupFolderPath() },
        { UserServiceKeys::k_BACKUP_PREFIX_KEY,    UserServiceDefaults::k_BACKUP_PREFIX }
    };

    QJsonObject userService;
    userService[UserServiceKeys::k_BACKUP_SERVICE_GROUP] = backupGroup;
    userService[UserServiceKeys::k_THEME_PREFERENCE_KEY] =
        userThemePreferenceToString(UserThemePreference::Auto);

    JsonManager::saveJsonFile(userServicePath, userService);
    userSettings = std::move(userService);
}
