#ifndef CFGMGR_SETTINGS_H
#define CFGMGR_SETTINGS_H

// Built-in Qt includes
#include <QDir>
#include <QString>

// Configuration keys
namespace ConfigKeys {
constexpr auto BACKUP_DIRECTORY_KEY = "backup_directory";
constexpr auto BACKUP_PREFIX_KEY = "backup_prefix";
}

// Default configuration values
namespace ConfigDefaults {
constexpr auto BACKUP_DIRECTORY = "C:\\temp";
constexpr auto BACKUP_PREFIX = "Backup_";
}

// Install metadata keys
namespace InstallMetadataKeys {
constexpr auto APP_NAME = "app_name";
constexpr auto APP_AUTHOR = "app_author";
constexpr auto APP_VERSION = "app_version";
constexpr auto INSTALL_DIRECTORY = "install_dir";
constexpr auto INSTALL_TIMESTAMP = "install_time";
}

namespace InstallMetadata {
constexpr auto INSTALL_NOTES = "Install location metadata. Do not modify.";
}

#endif
 // CFGMGR_SETTINGS_H
