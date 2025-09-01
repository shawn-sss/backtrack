#ifndef APP_INFO_H
#define APP_INFO_H

// Qt includes
#include <QString>
#include <QList>

// Application metadata
namespace App::Info {
inline constexpr auto k_AUTHOR_NAME = "Shawn SSS";
inline constexpr auto k_APP_NAME    = "Backtrack";
inline constexpr auto k_APP_ABOUT =
    "Backtrack is a lightweight C++ utility for backing up and managing data—"
    "from files and folders to full drives.\n"
    "It offers encrypted drive support, backup health monitoring, templates, "
    "and a streamlined settings panel—delivering fast, reliable, and transparent backups.";
inline constexpr auto k_APP_VERSION = "0.7";
}

// Application configuration and paths
namespace App::Items {
inline constexpr auto k_APPDATA_SETUP_FOLDER              = "app_config";
inline constexpr auto k_APPDATA_SETUP_INFO_FILE           = "app_init.json";
inline constexpr auto k_APPDATA_SETUP_NOTIFICATIONS_FILE  = "app_notifications.json";
inline constexpr auto k_APPDATA_SETUP_USER_SETTINGS_FILE  = "user_settings.json";
inline constexpr auto k_APPDATA_SETUP_USER_TEMPLATES_FILE = "user_templates.json";
inline constexpr auto k_APPDATA_COMBINED_PREFERENCES_FILE = "user_preferences_backup.json";

inline constexpr auto k_BACKUP_SETUP_FOLDER              = "app_backups";
inline constexpr auto k_BACKUP_SETUP_CONFIG_FOLDER       = "_Backtrack";
inline constexpr auto k_BACKUP_SETUP_CONFIG_INFO_FILE    = "backup_init.json";
inline constexpr auto k_BACKUP_SETUP_CONFIG_LOGS_FOLDER  = "logs";
inline constexpr auto k_BACKUP_SETUP_CONFIG_LOGS_FILE    = "log.json";
inline constexpr auto k_BACKUP_SETUP_CONFIG_LOGS_SUFFIX  = "_backup_logs.json";
}

#endif // APP_INFO_H
