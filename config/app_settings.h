#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <QString>
#include <QDir>
#include <QSize>

// Application information
namespace AppInfo {
constexpr auto AUTHOR_NAME = "Shawn SSS";
constexpr auto APP_DISPLAY_TITLE = "MyDataBackupApp";
constexpr auto APP_VERSION = "v0.4";
}

// Application configuration
namespace AppConfig {
constexpr auto BACKUP_CONFIG_FOLDER = "_MyDataBackupApp";
constexpr auto RUNTIME_STORAGE_FOLDER = "artifacts";
constexpr auto INSTALL_METADATA_FILE_NAME = "install_metadata.json";
constexpr auto USER_CONFIG_FILE_NAME = "user_config.json";
constexpr auto BACKUP_CONFIG_FILE_NAME = "backup_config.json";
constexpr auto BACKUP_LOGS_DIRECTORY = "logs";
const QString BACKUP_LOG_FILE_SUFFIX = "log.json";
}



#endif // APP_CONFIG_H
