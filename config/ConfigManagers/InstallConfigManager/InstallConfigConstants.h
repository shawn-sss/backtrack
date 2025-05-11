#ifndef INSTALLCONFIGCONSTANTS_H
#define INSTALLCONFIGCONSTANTS_H

// Project includes
#include "../../configsettings/app_settings.h"

// References to application metadata from app settings
namespace InstallMetadataSettings {
constexpr auto& kAppName        = AppInfo::k_APP_NAME;
constexpr auto& kAppAuthor      = AppInfo::k_APP_AUTHOR_NAME;
constexpr auto& kAppVersion     = AppInfo::k_APP_VERSION;
constexpr auto& kMetadataFolder = AppConfig::k_APPDATA_SETUP_FOLDER;
constexpr auto& kMetadataFile   = AppConfig::k_APPDATA_SETUP_INFO_FILE;
}

// Keys used within metadata JSON structure
namespace InstallMetadataKeys {
inline constexpr auto kMetadataVersion        = "metadata_version";
inline constexpr auto kMetadataVersionValue   = "1.0";
inline constexpr auto kAppInfoSection         = "app_info";
inline constexpr auto kInstallSection         = "app_install";
inline constexpr auto kAppName                = "app_name";
inline constexpr auto kAppAuthor              = "app_author";
inline constexpr auto kAppVersion             = "app_version";
inline constexpr auto kInstallLocation        = "install_location";
inline constexpr auto kInstallTimeUtc         = "install_time_utc";
inline constexpr auto kInstallTimeLocal       = "install_time_local";
inline constexpr auto kTimestampFormatPretty  = "MMMM d, yyyy 'at' h:mm AP";
}

#endif // INSTALLCONFIGCONSTANTS_H
