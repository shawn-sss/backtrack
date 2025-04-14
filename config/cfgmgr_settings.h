#ifndef CFGMGR_SETTINGS_H
#define CFGMGR_SETTINGS_H

#include <QDir>
#include <QString>

// Configuration keys for backup settings
namespace ConfigKeys {
constexpr auto k_BACKUP_CONFIG_GROUP = "backup_config";
constexpr auto k_BACKUP_DIRECTORY_KEY = "backup_directory";
constexpr auto k_BACKUP_PREFIX_KEY = "backup_prefix";
}

// Default values for backup configuration
namespace ConfigDefaults {
constexpr auto k_BACKUP_DIRECTORY = "C:\\temp";
constexpr auto k_BACKUP_PREFIX = "Backup_";
}

// Metadata keys for installation information
namespace InstallMetadataKeys {
constexpr auto k_APP_NAME = "app_name";
constexpr auto k_APP_AUTHOR = "app_author";
constexpr auto k_APP_VERSION = "app_version";
constexpr auto k_INSTALL_DIRECTORY = "install_dir";
constexpr auto k_INSTALL_TIMESTAMP = "install_time";
}

// Notes related to install metadata
namespace InstallMetadata {
constexpr auto k_INSTALL_NOTES = "Install location metadata. Do not modify.";
}

#endif // CFGMGR_SETTINGS_H
