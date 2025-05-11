#ifndef CONFIGDIRECTORCONSTANTS_H
#define CONFIGDIRECTORCONSTANTS_H

// Project includes
#include "../configsettings/app_settings.h"

// Constants for file/folder paths used by ConfigDirector
namespace ConfigDirectorConstants {
inline constexpr auto kAppDataFolder     = AppConfig::k_APPDATA_SETUP_FOLDER;
inline constexpr auto kUserDataFolder    = AppConfig::k_USERDATA_SETUP_FOLDER;
inline constexpr auto kMetadataFile      = AppConfig::k_APPDATA_SETUP_INFO_FILE;
inline constexpr auto kUserSettingsFile  = AppConfig::k_APPDATA_SETUP_USER_SETTINGS_FILE;
}

#endif // CONFIGDIRECTORCONSTANTS_H
