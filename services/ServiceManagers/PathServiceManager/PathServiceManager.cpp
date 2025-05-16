// Project includes
#include "PathServiceManager.h"
#include "PathServiceConstants.h"

// Qt includes
#include <QDir>
#include <QStandardPaths>

// Returns the app's root directory for internal data
QString PathServiceManager::appDataRootDir() {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

// Returns the directory for app-level configuration files
QString PathServiceManager::appConfigFolderPath() {
    return appDataRootDir() + "/" + APP_DATA_CONFIG_FOLDER_NAME;
}

// Returns the path to the app's initialization metadata file
QString PathServiceManager::appInitMetadataFilePath() {
    return appDataRootDir() + "/" + APP_DATA_FILE_APP_INIT_METADATA;
}

// Returns the path to the app's notifications log file
QString PathServiceManager::appNotificationsFilePath() {
    return appDataRootDir() + "/" + APP_DATA_FILE_NOTIFICATIONS_LOG;
}

// Returns the path to the user's settings configuration file
QString PathServiceManager::userSettingsFilePath() {
    return appDataRootDir() + "/" + APP_DATA_FILE_USER_SETTINGS;
}

// Returns the root directory for backup data
QString PathServiceManager::backupDataRootDir() {
    return QDir::tempPath();
}

// Returns the backup configuration directory inside the backup root
QString PathServiceManager::backupConfigFolderPath() {
    return backupDataRootDir() + "/" + BACKUP_LOCATION_CONFIG_FOLDER_NAME;
}

// Returns the path to the backup initialization metadata file
QString PathServiceManager::backupInitMetadataFilePath() {
    return backupDataRootDir() + "/" + BACKUP_LOCATION_FILE_INIT_METADATA;
}

// Returns the directory for backup-related logs
QString PathServiceManager::backupLogsFolderPath() {
    return backupDataRootDir() + "/" + BACKUP_LOCATION_LOG_FOLDER_NAME;
}
