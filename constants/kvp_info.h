#ifndef KVP_INFO_H
#define KVP_INFO_H

// Qt includes
#include <QString>

// Key-value pairs for user settings and backup metadata
namespace App::KVP {

// Keys for the user settings file
namespace UserServiceKeys {
inline constexpr auto k_BACKUP_SERVICE_GROUP = "backup_service";
inline constexpr auto k_BACKUP_DIRECTORY_KEY = "backup_directory";
inline constexpr auto k_BACKUP_PREFIX_KEY = "backup_prefix";
inline constexpr auto k_THEME_PREFERENCE_KEY = "theme_preference";
}

// Keys for backup log metadata
namespace BackupMetadata {
inline constexpr auto k_NAME = "backup_name";
inline constexpr auto k_TIMESTAMP = "backup_timestamp";
inline constexpr auto k_DURATION = "backup_duration";
inline constexpr auto k_DURATION_READABLE = "backup_duration_readable";
inline constexpr auto k_SIZE_BYTES = "total_size_bytes";
inline constexpr auto k_SIZE_READABLE = "total_size_readable";
inline constexpr auto k_FILE_COUNT = "file_count";
inline constexpr auto k_FOLDER_COUNT = "folder_count";
inline constexpr auto k_USER_SELECTED_ITEMS = "user_selected_items";
inline constexpr auto k_USER_SELECTED_ITEM_COUNT = "user_selected_item_count";
inline constexpr auto k_BACKUP_FILES = "backup_files";
inline constexpr auto k_BACKUP_FOLDERS = "backup_folders";
}

}

#endif // KVP_INFO_H
