#ifndef INSTALLSERVICECONSTANTS_H
#define INSTALLSERVICECONSTANTS_H

// Project includes
#include "../../../../constants/app_info.h"

// Aliases to application metadata
namespace InstallMetadataSettings {
constexpr auto& kAppName = App::Info::k_APP_NAME;
constexpr auto& kAppAuthor = App::Info::k_AUTHOR_NAME;
constexpr auto& kAppVersion = App::Info::k_APP_VERSION;
constexpr auto& kMetadataFolder = App::Items::k_APPDATA_SETUP_FOLDER;
constexpr auto& kMetadataFile = App::Items::k_APPDATA_SETUP_INFO_FILE;
}

// Keys for install metadata JSON
namespace InstallMetadataKeys {
inline constexpr auto kInstallSection = "app_install";
inline constexpr auto kAppName = "app_name";
inline constexpr auto kAppAuthor = "app_author";
inline constexpr auto kAppVersion = "app_version";
inline constexpr auto kInstallLocation = "install_location";
inline constexpr auto kInstallTimeUtc = "install_time_utc";
}

#endif
