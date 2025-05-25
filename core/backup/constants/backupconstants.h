#ifndef BACKUPCONSTANTS_H
#define BACKUPCONSTANTS_H

// Qt includes
#include <QDir>
#include <QString>

// Drive label constants for backup item selection
namespace Backup::Drive {
inline constexpr auto k_DEFAULT_DRIVE_LABEL = "Local Disk";
inline constexpr auto k_DRIVE_LABEL_SUFFIX  = "Drive";
}

// Timestamp formats for backup folders and display
namespace Backup::Timestamps {
inline constexpr auto k_BACKUP_FOLDER_TIMESTAMP_FORMAT     = "_yyyyMMdd_HHmmss";
inline constexpr auto k_BACKUP_TIMESTAMP_DISPLAY_FORMAT    = "MM/dd/yyyy hh:mm AP";
inline constexpr auto k_NOTIFICATION_TIMESTAMP_DISPLAY_FORMAT = "yyyy-MM-dd HH:mm:ss";
}

// Keys for backup log metadata
namespace BackupMetadata {
inline constexpr auto k_NAME                        = "backup_name";
inline constexpr auto k_TIMESTAMP                   = "backup_timestamp";
inline constexpr auto k_DURATION                    = "backup_duration";
inline constexpr auto k_DURATION_READABLE           = "backup_duration_readable";
inline constexpr auto k_SIZE_BYTES                  = "total_size_bytes";
inline constexpr auto k_SIZE_READABLE               = "total_size_readable";
inline constexpr auto k_FILE_COUNT                  = "file_count";
inline constexpr auto k_FOLDER_COUNT                = "folder_count";
inline constexpr auto k_USER_SELECTED_ITEMS         = "user_selected_items";
inline constexpr auto k_USER_SELECTED_ITEM_COUNT    = "user_selected_item_count";
inline constexpr auto k_BACKUP_FILES                = "backup_files";
inline constexpr auto k_BACKUP_FOLDERS              = "backup_folders";
}

#endif // BACKUPCONSTANTS_H
