#ifndef BACKUP_SETTINGS_H
#define BACKUP_SETTINGS_H

#include <QString>
#include <QDir>

namespace BackupInfo {
// Default Paths & File System Settings
inline const QString DEFAULT_ROOT_PATH = "";
inline const QString DEFAULT_FILE_DIALOG_ROOT = QDir::rootPath();
inline const QDir::Filters FILE_SYSTEM_FILTER = QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files;

// Backup Configuration Variables
inline const QString DEFAULT_DRIVE_LABEL = "Local Disk";
inline const QString DRIVE_LABEL_SUFFIX = "Drive";

// Timestamp Formats
inline const QString BACKUP_FOLDER_TIMESTAMP_FORMAT = "yyyyMMdd_HHmmss";
inline const QString BACKUP_TIMESTAMP_DISPLAY_FORMAT = "MM/dd/yyyy hh:mm AP";
}

namespace BackupMetadataKeys {
// Key-Value Pair Keys Stored in Backup Log Files
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

#endif // BACKUP_SETTINGS_H
