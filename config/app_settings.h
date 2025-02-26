#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <QString>
#include <QDir>
#include <QSize>

namespace AppInfo {
// Application Information
constexpr auto AUTHOR_NAME = "Shawn SSS";
constexpr auto APP_DISPLAY_TITLE = "MyDataBackupApp";
constexpr auto APP_VERSION = "v0.3";
}

namespace AppConfig {
// Application Configuration
constexpr auto BACKUP_CONFIG_FOLDER = "_MyDataBackupApp";
constexpr auto CONFIG_FILE_NAME = "config.json";
constexpr auto BACKUP_LOGS_DIRECTORY = "logs";
const QString BACKUP_LOG_FILE_SUFFIX = "log.json";
}

// -- See ConfigManager for User Settings -- \\

#endif // APP_CONFIG_H
