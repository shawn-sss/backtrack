#ifndef USERCONFIGCONSTANTS_H
#define USERCONFIGCONSTANTS_H

// Keys used in user settings storage
namespace UserConfigKeys {
inline constexpr const char* k_BACKUP_CONFIG_GROUP   = "backup_config";
inline constexpr const char* k_BACKUP_DIRECTORY_KEY  = "backup_directory";
inline constexpr const char* k_BACKUP_PREFIX_KEY     = "backup_prefix";
inline constexpr const char* k_THEME_PREFERENCE_KEY  = "theme_preference";
}

// Default values for user settings
namespace UserConfigDefaults {
inline constexpr const char* k_BACKUP_DIRECTORY = "C:\\temp";
inline constexpr const char* k_BACKUP_PREFIX    = "Backup";
}

#endif // USERCONFIGCONSTANTS_H
