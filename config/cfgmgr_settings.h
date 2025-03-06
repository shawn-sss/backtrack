#ifndef CFGMGR_SETTINGS_H
#define CFGMGR_SETTINGS_H

#include <QString>
#include <QDir>

// Configuration keys
namespace ConfigKeys {
constexpr auto BACKUP_DIRECTORY_KEY = "backup_directory";
constexpr auto BACKUP_PREFIX_KEY = "backup_prefix";
}

// Default values (moved from ConfigManager.h)
namespace ConfigDefaults {
constexpr auto BACKUP_DIRECTORY = "C:\\temp";
constexpr auto BACKUP_PREFIX = "Backup_";
}

namespace InstallMetadataKeys {
constexpr auto APP_NAME = "app_name";
constexpr auto APP_AUTHOR = "app_author";
constexpr auto APP_VERSION = "app_version";
constexpr auto INSTALL_DIRECTORY = "install_dir";
constexpr auto INSTALL_TIMESTAMP = "install_time";
}

#endif // CFGMGR_SETTINGS_H
