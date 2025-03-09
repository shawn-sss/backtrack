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
constexpr auto BACKUP_CONFIG_FOLDER = "_MyDataBackupApp";
constexpr auto RUNTIME_STORAGE_FOLDER = "artifacts";
constexpr auto INSTALL_METADATA_FILE_NAME = "install_metadata.json";
constexpr auto USER_CONFIG_FILE_NAME = "user_config.json";
constexpr auto BACKUP_CONFIG_FILE_NAME = "backup_config.json";
constexpr auto BACKUP_LOGS_DIRECTORY = "logs";
inline auto BACKUP_LOG_FILE_SUFFIX = QStringLiteral("log.json");
}

#endif // APP_CONFIG_H
