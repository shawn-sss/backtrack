#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <QString>

namespace AppConfig {
constexpr auto WINDOW_TITLE = "MyDataBackupApp";
inline const QString ABOUT_DIALOG_TITLE = "About";
inline const QString ABOUT_DIALOG_MESSAGE = "%1\n%2\n\nMade by Shawn SSS";
inline const QString APP_VERSION = "Version 0.3";

// Folder and File Names for Backup Infrastructure
inline const QString BACKUP_SETTINGS_FOLDER = "_backup_settings";
inline const QString SETTINGS_FILE_NAME = "settings.json";
inline const QString BACKUP_LOGS_FOLDER = "backup_logs";
inline const QString BACKUP_LOG_SUFFIX = "_backup_log.json";

inline const QString NAME = "backup_name";
inline const QString TIMESTAMP = "backup_timestamp";
inline const QString DURATION = "backup_duration";
inline const QString DURATION_READABLE = "backup_duration_readable";
inline const QString SIZE_BYTES = "total_size_bytes";
inline const QString SIZE_READABLE = "total_size_readable";
inline const QString FILE_COUNT = "file_count";
inline const QString FOLDER_COUNT = "folder_count";
inline const QString USER_SELECTED_ITEMS = "user_selected_items";
inline const QString USER_SELECTED_ITEM_COUNT = "user_selected_item_count";
inline const QString BACKUP_FILES = "backup_files";
inline const QString BACKUP_FOLDERS = "backup_folders";

// Error Message Templates
inline const QString ERROR_CREATE_DIR = "Failed to create '%1' directory.";
inline const QString ERROR_CREATE_FILE = "Failed to create '%1' file.";
inline const QString ERROR_CREATE_FOLDER = "Failed to create '%1' folder.";
}

namespace BackupMetadataKeys {
inline const QString NAME = "backup_name";
inline const QString TIMESTAMP = "backup_timestamp";
inline const QString DURATION = "backup_duration";
inline const QString DURATION_READABLE = "backup_duration_readable";
inline const QString SIZE_BYTES = "total_size_bytes";
inline const QString SIZE_READABLE = "total_size_readable";
inline const QString FILE_COUNT = "file_count";
inline const QString FOLDER_COUNT = "folder_count";
inline const QString USER_SELECTED_ITEMS = "user_selected_items";
inline const QString USER_SELECTED_ITEM_COUNT = "user_selected_item_count";
inline const QString BACKUP_FILES = "backup_files";
inline const QString BACKUP_FOLDERS = "backup_folders";
}

#endif // APP_CONFIG_H
