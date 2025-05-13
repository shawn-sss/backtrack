#ifndef BACKUPSERVICECONSTANTS_H
#define BACKUPSERVICECONSTANTS_H

// Qt includes
#include <QString>

// Keys used for backup service settings
namespace ServiceKeys {
inline constexpr const char* BACKUP_SERVICE_GROUP = "backup_service";
inline constexpr const char* BACKUP_DIRECTORY_KEY = "backup_directory";
inline constexpr const char* BACKUP_PREFIX_KEY    = "backup_prefix";
}

// Default values for backup service
namespace ServiceDefaults {
inline constexpr const char* BACKUP_DIRECTORY = "C:\\temp";
inline constexpr const char* BACKUP_PREFIX    = "Backup";
}

#endif // BACKUPSERVICECONSTANTS_H
