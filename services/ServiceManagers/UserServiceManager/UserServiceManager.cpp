// Project includes
#include "UserServiceManager.h"
#include "UserServiceConstants.h"
#include "../../ServiceManagers/ThemeServiceManager/ThemeServiceConstants.h"
#include "../../../../core/shared/jsonmanager.h"

using namespace ThemeServiceConstants;

// Constructor that sets the path to the user service file
UserServiceManager::UserServiceManager(const QString& serviceFilePath)
    : userServicePath(serviceFilePath) {}

// Loads user settings from the service file
void UserServiceManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(userServicePath, rootObject) && !rootObject.isEmpty()) {
        userSettings = rootObject;
    }
}

// Saves current user settings to the service file
void UserServiceManager::save() const {
    JsonManager::saveJsonFile(userServicePath, userSettings);
}

// Returns a modifiable reference to the user settings object
QJsonObject& UserServiceManager::settings() {
    return userSettings;
}

// Returns a const reference to the user settings object
const QJsonObject& UserServiceManager::settings() const {
    return userSettings;
}

// Initializes default settings and saves them to disk
void UserServiceManager::initializeDefaults() {
    QJsonObject userService;
    userService[UserServiceKeys::k_BACKUP_SERVICE_GROUP] = QJsonObject{
        { UserServiceKeys::k_BACKUP_DIRECTORY_KEY, UserServiceDefaults::k_BACKUP_DIRECTORY },
        { UserServiceKeys::k_BACKUP_PREFIX_KEY,    UserServiceDefaults::k_BACKUP_PREFIX }
    };
    userService[UserServiceKeys::k_THEME_PREFERENCE_KEY] =
        userThemePreferenceToString(UserThemePreference::Auto);

    JsonManager::saveJsonFile(userServicePath, userService);
    userSettings = userService;
}
