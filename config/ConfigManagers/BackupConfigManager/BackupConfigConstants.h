#ifndef BACKUPCONFIGCONSTANTS_H
#define BACKUPCONFIGCONSTANTS_H

// Qt includes
#include <QString>

// Keys used for backup configuration settings
namespace ConfigKeys {
inline constexpr const char* BACKUP_CONFIG_GROUP  = "backup_config";
inline constexpr const char* BACKUP_DIRECTORY_KEY = "backup_directory";
inline constexpr const char* BACKUP_PREFIX_KEY    = "backup_prefix";
}

// Default values for backup configuration
namespace ConfigDefaults {
inline constexpr const char* BACKUP_DIRECTORY = "C:\\temp";
inline constexpr const char* BACKUP_PREFIX    = "Backup";
}

#endif // BACKUPCONFIGCONSTANTS_H
