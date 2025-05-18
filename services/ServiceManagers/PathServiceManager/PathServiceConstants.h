#ifndef PATHSERVICECONSTANTS_H
#define PATHSERVICECONSTANTS_H

//Project includes
#include "../../../constants/app_info.h"

// ───────────────────────────────────|
// AppData-related folder & file names|
// ───────────────────────────────────|

// {OS}:
constexpr auto APP_DATA_OS_DRIVE_ROOT = "";

// {OS}:/{APPDATA}
constexpr auto APP_DATA_ROOT_PATH = "";

// {OS}:/{APPDATA}/*APP NAME*
constexpr auto APP_DATA_APP_FOLDER_NAME = App::Info::k_APP_NAME;

// {OS}:/{APPDATA}/*APP NAME*/*APP CONFIG FOLDER*
constexpr auto APP_DATA_CONFIG_FOLDER_NAME = App::Items::k_APPDATA_SETUP_FOLDER;

// {OS}:/{APPDATA}/*APP NAME*/*APP CONFIG FOLDER*/*APP INIT*
constexpr auto APP_DATA_FILE_APP_INIT_METADATA = App::Items::k_APPDATA_SETUP_INFO_FILE;

// {OS}:/{APPDATA}/*APP NAME*/*APP CONFIG FOLDER*/*APP NOTIFICATIONS*
constexpr auto APP_DATA_FILE_NOTIFICATIONS_LOG = App::Items::k_APPDATA_SETUP_NOTIFICATIONS_FILE;

// {OS}:/{APPDATA}/*APP NAME*/*APP CONFIG FOLDER*/*USER SETTINGS*
constexpr auto APP_DATA_FILE_USER_SETTINGS = App::Items::k_APPDATA_SETUP_USER_SETTINGS_FILE;

// ───────────────────────────────────|
// Backup-related folder & file names |
// ───────────────────────────────────|

// {USER SELECTION}:
constexpr auto BACKUP_LOCATION_ROOT_PATH = "";

// {USER SELECTION}:/*BACKUP SETUP FOLDER*
constexpr auto BACKUP_LOCATION_SETUP_FOLDER_NAME = App::Items::k_BACKUP_SETUP_FOLDER;

// {USER SELECTION}:/*BACKUP SETUP FOLDER*/*_APP NAME*
constexpr auto BACKUP_LOCATION_CONFIG_FOLDER_NAME = App::Items::k_BACKUP_SETUP_CONFIG_FOLDER;

// {USER SELECTION}:/*_APP NAME*/*BACKUP INIT*
constexpr auto BACKUP_LOCATION_FILE_INIT_METADATA = App::Items::k_BACKUP_SETUP_CONFIG_INFO_FILE;

// {USER SELECTION}:/*_APP NAME*/*BACKUP LOGS FOLDER*
constexpr auto BACKUP_LOCATION_LOG_FOLDER_NAME = App::Items::k_BACKUP_SETUP_CONFIG_LOGS_FOLDER;

#endif // PATHSERVICECONSTANTS_H
