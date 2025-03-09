#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// Built-in Qt includes
#include <QDir>
#include <QSize>
#include <QString>
#include <QStringLiteral>

// Application information
namespace AppInfo {
constexpr auto AUTHOR_NAME = "Shawn SSS";
constexpr auto APP_DISPLAY_TITLE = "MyDataBackupApp";
constexpr auto APP_VERSION = "0.4";
}

// Application configuration
namespace AppConfig {
// Directory for application-specific data
constexpr auto APPDATA_SETUP_FOLDER = "app_config";
constexpr auto APPDATA_SETUP_INFO_FILE = "app_init.json";
constexpr auto APPDATA_SETUP_USER_SETTINGS_FILE = "user_settings.json";

// Directory for backup-specific data
constexpr auto BACKUP_SETUP_FOLDER = "_MyDataBackupApp";
constexpr auto BACKUP_SETUP_INFO_FILE = "backup_init.json";
constexpr auto BACKUP_SETUP_LOGS_FOLDER = "logs";
inline auto BACKUP_SETUP_LOGS_FILE = QStringLiteral("log.json");
}

#endif // APP_CONFIG_H
