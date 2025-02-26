#ifndef BACKUP_SETTINGS_H
#define BACKUP_SETTINGS_H

#include <QString>
#include <QDir>

namespace BackupPaths {
// Default Root Paths
const QString DEFAULT_ROOT_PATH = "";
const QString DEFAULT_FILE_DIALOG_ROOT = QDir::rootPath();
}

namespace FileSystemSettings {
// File System Filters
constexpr QDir::Filters FILE_SYSTEM_FILTER = QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files;
}

namespace BackupConfiguration {
// Backup Drive Label Settings
constexpr auto DEFAULT_DRIVE_LABEL = "Local Disk";
constexpr auto DRIVE_LABEL_SUFFIX = "Drive";
}

namespace TimestampFormats {
// Timestamp Formats for Backup Folder Naming and Display
constexpr auto BACKUP_FOLDER_TIMESTAMP_FORMAT = "yyyyMMdd_HHmmss";
constexpr auto BACKUP_TIMESTAMP_DISPLAY_FORMAT = "MM/dd/yyyy hh:mm AP";
}

namespace BackupMetadataKeys {
// Metadata Keys Stored in Backup Log Files
constexpr auto NAME = "backup_name";
constexpr auto TIMESTAMP = "backup_timestamp";
constexpr auto DURATION = "backup_duration";
constexpr auto DURATION_READABLE = "backup_duration_readable";
constexpr auto SIZE_BYTES = "total_size_bytes";
constexpr auto SIZE_READABLE = "total_size_readable";
constexpr auto FILE_COUNT = "file_count";
constexpr auto FOLDER_COUNT = "folder_count";
constexpr auto USER_SELECTED_ITEMS = "user_selected_items";
constexpr auto USER_SELECTED_ITEM_COUNT = "user_selected_item_count";
constexpr auto BACKUP_FILES = "backup_files";
constexpr auto BACKUP_FOLDERS = "backup_folders";
}

#endif // BACKUP_SETTINGS_H
