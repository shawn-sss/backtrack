#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <QString>

namespace AppInfo {
// Application Information
constexpr auto APP_DISPLAY_TITLE = "MyDataBackup";
inline const QString APP_VERSION = "v0.3";
}

namespace AboutInfo {
// About Dialog Information
inline const QString ABOUT_WINDOW_TITLE = "About MyDataBackupApp";
inline const QString ABOUT_WINDOW_MESSAGE = "%2\nVersion %1\n\nDeveloped by Shawn SSS";
}

namespace HelpInfo {
// Help Dialog Information
inline const QString HELP_WINDOW_TITLE = "Help";
inline const QString HELP_WINDOW_MESSAGE = "To be added later.";
}

namespace AppConfig {
// Application Configuration
inline const QString BACKUP_CONFIG_FOLDER = "_MyDataBackupApp";
inline const QString CONFIG_FILE_NAME = "config.json";
inline const QString BACKUP_LOGS_DIRECTORY = "Backup Logs";
inline const QString BACKUP_LOG_FILE_SUFFIX = "_log.json";
}

namespace UserConfig {
// User Configurable Configuration
inline const QString DEFAULT_BACKUP_DIRECTORY = "C:\\temp";
inline const QString BACKUP_FOLDER_PREFIX = "Backup_";
inline const QString BACKUP_SUMMARY_FILE = "backup_summary.json";
}

#endif // APP_CONFIG_H
