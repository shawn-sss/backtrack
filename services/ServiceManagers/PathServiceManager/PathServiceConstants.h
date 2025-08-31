#ifndef PATHSERVICECONSTANTS_H
#define PATHSERVICECONSTANTS_H

// Project includes
#include "../../../constants/app_info.h"

// Application data
constexpr auto AppDataConfigFolderName  = App::Items::k_APPDATA_SETUP_FOLDER;
constexpr auto AppDataFileInitMetadata  = App::Items::k_APPDATA_SETUP_INFO_FILE;
constexpr auto AppDataFileNotifications = App::Items::k_APPDATA_SETUP_NOTIFICATIONS_FILE;
constexpr auto AppDataFileUserSettings  = App::Items::k_APPDATA_SETUP_USER_SETTINGS_FILE;
constexpr auto AppDataFileUserTemplates = App::Items::k_APPDATA_SETUP_USER_TEMPLATES_FILE;

// Backup data
constexpr auto BackupSetupFolderName    = App::Items::k_BACKUP_SETUP_FOLDER;
constexpr auto BackupConfigFolderName   = App::Items::k_BACKUP_SETUP_CONFIG_FOLDER;
constexpr auto BackupFileInitMetadata   = App::Items::k_BACKUP_SETUP_CONFIG_INFO_FILE;
constexpr auto BackupLogsFolderName     = App::Items::k_BACKUP_SETUP_CONFIG_LOGS_FOLDER;

#endif
