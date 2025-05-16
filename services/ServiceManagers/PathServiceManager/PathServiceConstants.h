#ifndef PATHSERVICECONSTANTS_H
#define PATHSERVICECONSTANTS_H

// Qt includes
#include <QString>

// ───────────────────────────────────|
// AppData-related folder & file names|
// ───────────────────────────────────|

// {OS}:
constexpr auto APP_DATA_OS_DRIVE_ROOT = "";

// {OS}:/{APPDATA}
constexpr auto APP_DATA_ROOT_PATH = "";

// {OS}:/{APPDATA}/*APP NAME*
constexpr auto APP_DATA_APP_FOLDER_NAME = "MyDataBackupApp";

// {OS}:/{APPDATA}/*APP NAME*/*APP CONFIG FOLDER*
constexpr auto APP_DATA_CONFIG_FOLDER_NAME = "app_config";

// {OS}:/{APPDATA}/*APP NAME*/*APP CONFIG FOLDER*/*APP INIT*
constexpr auto APP_DATA_FILE_APP_INIT_METADATA = "app_init.json";

// {OS}:/{APPDATA}/*APP NAME*/*APP CONFIG FOLDER*/*APP NOTIFICATIONS*
constexpr auto APP_DATA_FILE_NOTIFICATIONS_LOG = "app_notifications.json";

// {OS}:/{APPDATA}/*APP NAME*/*APP CONFIG FOLDER*/*USER SETTINGS*
constexpr auto APP_DATA_FILE_USER_SETTINGS = "user_settings.json";

// ───────────────────────────────────|
// Backup-related folder & file names |
// ───────────────────────────────────|

// {USER SELECTION}:
constexpr auto BACKUP_LOCATION_ROOT_PATH = "";

// {USER SELECTION}:/*_APP NAME*
constexpr auto BACKUP_LOCATION_CONFIG_FOLDER_NAME = "_MyDataBackupApp";

// {USER SELECTION}:/*_APP NAME*/*BACKUP INIT*
constexpr auto BACKUP_LOCATION_FILE_INIT_METADATA = "backup_init.json";

// {USER SELECTION}:/*_APP NAME*/*BACKUP LOGS FOLDER*
constexpr auto BACKUP_LOCATION_LOG_FOLDER_NAME = "logs";

#endif // PATHSERVICECONSTANTS_H
