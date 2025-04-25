#ifndef BACKUP_SETTINGS_H
#define BACKUP_SETTINGS_H

// Qt includes
#include <QDir>
#include <QString>
#include <QStringLiteral>

// Root path settings for backups
namespace Backup::Paths {
inline auto k_DEFAULT_ROOT_PATH = QLatin1String("");
const QString k_DEFAULT_FILE_DIALOG_ROOT = QDir::rootPath();
}

// Filters for file system operations
namespace Backup::FileSystem {
constexpr QDir::Filters k_FILE_SYSTEM_FILTER = QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files;
}

// Backup drive label configuration
namespace Backup::DriveConfig {
constexpr auto k_DEFAULT_DRIVE_LABEL = "Local Disk";
constexpr auto k_DRIVE_LABEL_SUFFIX = "Drive";
}

// Formats used for timestamps
namespace Backup::Timestamps {
constexpr auto k_BACKUP_FOLDER_TIMESTAMP_FORMAT = "_yyyyMMdd_HHmmss";
constexpr auto k_BACKUP_TIMESTAMP_DISPLAY_FORMAT = "MM/dd/yyyy hh:mm AP";
}

// Metadata keys for storing backup details
namespace Backup::MetadataKeys {
constexpr auto k_NAME = "backup_name";
constexpr auto k_TIMESTAMP = "backup_timestamp";
constexpr auto k_DURATION = "backup_duration";
constexpr auto k_DURATION_READABLE = "backup_duration_readable";
constexpr auto k_SIZE_BYTES = "total_size_bytes";
constexpr auto k_SIZE_READABLE = "total_size_readable";
constexpr auto k_FILE_COUNT = "file_count";
constexpr auto k_FOLDER_COUNT = "folder_count";
constexpr auto k_USER_SELECTED_ITEMS = "user_selected_items";
constexpr auto k_USER_SELECTED_ITEM_COUNT = "user_selected_item_count";
constexpr auto k_BACKUP_FILES = "backup_files";
constexpr auto k_BACKUP_FOLDERS = "backup_folders";
}

// Backup infrastructure setup paths
namespace Backup::Infrastructure {
constexpr auto k_BACKUP_SETUP_FOLDER = "_MyDataBackupApp";
constexpr auto k_BACKUP_SETUP_INFO_FILE = "backup_init.json";
constexpr auto k_BACKUP_LOGS_FOLDER = "logs";
inline auto k_BACKUP_LOGS_FILE = QStringLiteral("log.json");
}

#endif // BACKUP_SETTINGS_H
