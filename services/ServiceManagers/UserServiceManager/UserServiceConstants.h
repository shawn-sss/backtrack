#ifndef USERSERVICECONSTANTS_H
#define USERSERVICECONSTANTS_H

// Keys used in user settings storage
namespace UserServiceKeys {
inline constexpr const char* k_BACKUP_SERVICE_GROUP = "backup_service";
inline constexpr const char* k_BACKUP_DIRECTORY_KEY = "backup_directory";
inline constexpr const char* k_BACKUP_PREFIX_KEY    = "backup_prefix";
inline constexpr const char* k_THEME_PREFERENCE_KEY = "theme_preference";
}

// Default values for user settings
namespace UserServiceDefaults {
inline constexpr const char* k_BACKUP_DIRECTORY = "C:\\temp";
inline constexpr const char* k_BACKUP_PREFIX    = "Backup";
}

#endif // USERSERVICECONSTANTS_H
