// Project includes
#include "UserServiceManager.h"
#include "UserServiceConstants.h"
#include "../JsonServiceManager/JsonServiceManager.h"
#include "../PathServiceManager/PathServiceManager.h"
#include "../ThemeServiceManager/ThemeServiceConstants.h"

// Lifecycle
UserServiceManager::UserServiceManager(const QString& serviceFilePath)
    : userServicePath(serviceFilePath) {}

// Persistence: load
void UserServiceManager::load() {
    QJsonObject rootObject;
    if (JsonManager::loadJsonFile(userServicePath, rootObject) && !rootObject.isEmpty())
        userSettings = rootObject;
}

// Persistence: save
void UserServiceManager::save() const {
    JsonManager::saveJsonFile(userServicePath, userSettings);
}

// Persistence: initialize defaults
void UserServiceManager::initializeDefaults() {
    QJsonObject backupGroup{
        { UserServiceKeys::BackupDirectoryKey, PathServiceManager::backupSetupFolderPath() },
        { UserServiceKeys::BackupPrefixKey,    UserServiceDefaults::BackupPrefix }
    };

    QJsonObject userService;
    userService[UserServiceKeys::BackupServiceGroup] = backupGroup;
    userService[UserServiceKeys::ThemePreferenceKey] =
        userThemePreferenceToString(ThemeServiceConstants::UserThemePreference::Auto);
    userService[UserServiceKeys::MinimizeOnCloseKey] = UserServiceDefaults::MinimizeOnClose;

    JsonManager::saveJsonFile(userServicePath, userService);
    userSettings = std::move(userService);
}

// Accessors
QJsonObject& UserServiceManager::settings() {
    return userSettings;
}

const QJsonObject& UserServiceManager::settings() const {
    return userSettings;
}
