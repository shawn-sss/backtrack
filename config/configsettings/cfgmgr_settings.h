#ifndef CFGMGR_SETTINGS_H
#define CFGMGR_SETTINGS_H

// Qt includes
#include <QString>
#include <QDir>

// Keys used in backup configuration settings
namespace ConfigKeys {
constexpr auto k_BACKUP_CONFIG_GROUP = "backup_config";
constexpr auto k_BACKUP_DIRECTORY_KEY = "backup_directory";
constexpr auto k_BACKUP_PREFIX_KEY = "backup_prefix";
constexpr auto k_THEME_PREFERENCE_KEY = "theme_preference";
}

// Default values for backup configuration
namespace ConfigDefaults {
constexpr auto k_BACKUP_DIRECTORY = "C:\\temp";
constexpr auto k_BACKUP_PREFIX = "Backup";
}

// Keys used in install metadata
namespace InstallMetadataKeys {
constexpr auto k_APP_NAME = "app_name";
constexpr auto k_APP_AUTHOR = "app_author";
constexpr auto k_APP_VERSION = "app_version";
constexpr auto k_INSTALL_DIRECTORY = "install_dir";
constexpr auto k_INSTALL_TIMESTAMP = "install_time";
}

// Annotation string for install metadata file
namespace InstallMetadata {
constexpr auto k_INSTALL_NOTES = "Install location metadata. Do not modify.";
}

#endif // CFGMGR_SETTINGS_H
