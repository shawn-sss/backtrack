#ifndef USERSERVICECONSTANTS_H
#define USERSERVICECONSTANTS_H

// Default values for user service settings
namespace UserServiceDefaults {
inline constexpr const char* k_BACKUP_PREFIX = "Backup";
}

// Keys for the user settings file
namespace UserServiceKeys {
inline constexpr auto k_BACKUP_SERVICE_GROUP  = "backup_service";
inline constexpr auto k_BACKUP_DIRECTORY_KEY  = "backup_directory";
inline constexpr auto k_BACKUP_PREFIX_KEY     = "backup_prefix";
inline constexpr auto k_THEME_PREFERENCE_KEY  = "theme_preference";
inline constexpr auto k_MINIMIZE_ON_CLOSE_KEY = "minimize_on_close";
inline constexpr auto k_SCHEDULE_ENABLED_KEY    = "schedule_enabled";
inline constexpr auto k_SCHEDULE_WHEN_ISO_KEY   = "schedule_when_iso";
inline constexpr auto k_SCHEDULE_RECURRENCE_KEY = "schedule_recur"; // 0=Once,1=Daily,2=Weekly,3=Monthly
}

#endif // USERSERVICECONSTANTS_H
