#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// Built-in Qt includes
#include <QDir>
#include <QSize>
#include <QString>
#include <QStringLiteral>

// Application metadata
namespace AppInfo {
constexpr auto k_AUTHOR_NAME = "Shawn SSS";
constexpr auto k_APP_DISPLAY_TITLE = "MyDataBackupApp";
constexpr auto k_APP_VERSION = "0.4";
}

// Application settings
namespace AppConfig {
constexpr QSize k_MINIMUM_WINDOW_SIZE(500, 250);
constexpr QSize k_DEFAULT_WINDOW_SIZE(1300, 320);
constexpr QSize k_MAXIMUM_WINDOW_SIZE(2000, 800);

// App-specific data paths
constexpr auto k_APPDATA_SETUP_FOLDER = "app_config";
constexpr auto k_APPDATA_SETUP_INFO_FILE = "app_init.json";
constexpr auto k_APPDATA_SETUP_USER_SETTINGS_FILE = "user_settings.json";

// Backup-specific data paths
constexpr auto k_BACKUP_SETUP_FOLDER = "_MyDataBackupApp";
constexpr auto k_BACKUP_SETUP_INFO_FILE = "backup_init.json";

// Logging data paths
constexpr auto k_BACKUP_LOGS_FOLDER = "logs";
inline auto k_BACKUP_LOGS_FILE = QStringLiteral("log.json");
}

#endif // APP_CONFIG_H
