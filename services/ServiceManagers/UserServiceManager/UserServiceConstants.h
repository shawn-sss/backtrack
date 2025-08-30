#ifndef USERSERVICECONSTANTS_H
#define USERSERVICECONSTANTS_H

// Defaults
namespace UserServiceDefaults {
inline constexpr const char* BackupPrefix     = "Backup";
inline constexpr bool        MinimizeOnClose = true;
}

// Keys
namespace UserServiceKeys {
inline constexpr const char* BackupServiceGroup   = "backup_service";
inline constexpr const char* BackupDirectoryKey   = "backup_directory";
inline constexpr const char* BackupPrefixKey      = "backup_prefix";
inline constexpr const char* ThemePreferenceKey   = "theme_preference";
inline constexpr const char* MinimizeOnCloseKey   = "minimize_on_close";

// Legacy (schedule migration support)
inline constexpr const char* ScheduleEnabledKey    = "schedule_enabled";
inline constexpr const char* ScheduleWhenIsoKey   = "schedule_when_iso";
inline constexpr const char* ScheduleRecurrenceKey = "schedule_recur";
}

#endif
