#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <QString>
#include <QDir>

namespace AppInfo {
// Application Information
constexpr auto APP_DISPLAY_TITLE = "MyDataBackupApp";
constexpr auto APP_VERSION = "v0.3";
}

namespace AboutInfo {
// About Dialog Information
constexpr auto ABOUT_WINDOW_TITLE = "About MyDataBackupApp";
constexpr auto ABOUT_WINDOW_MESSAGE = "%2\nVersion %1\n\nDeveloped by Shawn SSS";
}

namespace HelpInfo {
// Help Dialog Information
constexpr auto HELP_WINDOW_TITLE = "Help";
constexpr auto HELP_WINDOW_MESSAGE = "How to create a backup:\n1. Select one or more drives, directories, or files.\n2. Click the add to backup button.\n3. Confirm desired items are selected for backup.\n4. Click the create backup button.";
}

namespace AppConfig {
// Application Configuration
constexpr auto BACKUP_CONFIG_FOLDER = "_MyDataBackupApp";
constexpr auto CONFIG_FILE_NAME = "config.json";
constexpr auto BACKUP_LOGS_DIRECTORY = "Backup Logs";
const QString BACKUP_LOG_FILE_SUFFIX = "_log.json";
constexpr auto CACHE_FILE_NAME = "TBA";
const QString DEFAULT_BACKUP_LOCATION = QDir::homePath();
}

#endif // APP_CONFIG_H
