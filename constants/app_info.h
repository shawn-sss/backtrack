#ifndef APP_INFO_H
#define APP_INFO_H

// Qt includes
#include <QString>

// Application metadata
namespace App::Info {
inline constexpr auto k_AUTHOR_NAME = "Shawn SSS";
inline constexpr auto k_APP_NAME = "MyDataBackupApp";
inline constexpr auto k_APP_VERSION = "0.5";
}

// Application configuration and paths
namespace App::Items {
inline constexpr auto k_APPDATA_SETUP_FOLDER = "app_config";

inline constexpr auto k_APPDATA_SETUP_INFO_FILE = "app_init.json";
inline constexpr auto k_APPDATA_SETUP_NOTIFICATIONS_FILE = "app_notifications.json";
inline constexpr auto k_APPDATA_SETUP_USER_SETTINGS_FILE = "user_settings.json";

inline constexpr auto k_BACKUP_SETUP_FOLDER = "app_backups";
inline constexpr auto k_BACKUP_SETUP_CONFIG_FOLDER = "_MyDataBackupApp";
inline constexpr auto k_BACKUP_SETUP_CONFIG_LOGS_FOLDER = "logs";

inline constexpr auto k_BACKUP_SETUP_CONFIG_INFO_FILE = "backup_init.json";
inline const auto k_BACKUP_SETUP_CONFIG_LOGS_FILE = QStringLiteral("log.json");
}

#endif // APP_INFO_H
