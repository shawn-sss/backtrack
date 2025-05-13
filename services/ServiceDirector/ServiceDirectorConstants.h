#ifndef SERVICEDIRECTORCONSTANTS_H
#define SERVICEDIRECTORCONSTANTS_H

// Project includes
#include "../../../../constants/window_config.h"

// Constants for file/folder paths used by ServiceDirector
namespace ServiceDirectorConstants {
inline constexpr auto kAppDataFolder    = App::ConfigFiles::k_APPDATA_SETUP_FOLDER;
inline constexpr auto kUserDataFolder   = App::ConfigFiles::k_USERDATA_SETUP_FOLDER;
inline constexpr auto kMetadataFile     = App::ConfigFiles::k_APPDATA_SETUP_INFO_FILE;
inline constexpr auto kUserSettingsFile = App::ConfigFiles::k_APPDATA_SETUP_USER_SETTINGS_FILE;
}

#endif // SERVICEDIRECTORCONSTANTS_H
