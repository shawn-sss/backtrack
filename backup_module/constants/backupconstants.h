#ifndef BACKUPCONSTANTS_H
#define BACKUPCONSTANTS_H

// Qt includes
#include <QDir>
#include <QString>
#include <QRegularExpression>

namespace Backup {

// ------------------------------------------------------------
// Drive label constants
// ------------------------------------------------------------
namespace Drive {
inline const QString DefaultLabel = "Local Disk";
inline const QString LabelSuffix  = "Drive";
}

// ------------------------------------------------------------
// Timestamp formats and regex
// ------------------------------------------------------------
namespace Timestamps {
inline const QString FolderFormat       = "_yyyyMMdd_HHmmss";
inline const QString DisplayFormat      = "MM/dd/yyyy hh:mm AP";
inline const QString NotificationFormat = "yyyy-MM-dd HH:mm:ss";
inline const QString IsoFormat          = "yyyy-MM-ddTHH:mm:ssZ"; // ISO format
inline const QRegularExpression FolderRegex{R"((\d{8}_\d{6}))"};
}

// ------------------------------------------------------------
// Backup log metadata keys
// ------------------------------------------------------------
namespace Metadata {
inline const QString Name                = "backup_name";
inline const QString Timestamp           = "backup_timestamp";
inline const QString Duration            = "backup_duration";
inline const QString DurationReadable    = "backup_duration_readable";
inline const QString SizeBytes           = "total_size_bytes";
inline const QString SizeReadable        = "total_size_readable";
inline const QString FileCount           = "file_count";
inline const QString FolderCount         = "folder_count";
inline const QString UserSelectedItems   = "user_selected_items";
inline const QString UserSelectedItemCnt = "user_selected_item_count";
inline const QString Files               = "backup_files";
inline const QString Folders             = "backup_folders";
}

// ------------------------------------------------------------
// Initial backup config keys
// ------------------------------------------------------------
namespace InitConfig {
inline const QString Location   = "location";
inline const QString Timestamp  = "timestamp";
inline const QString AppName    = "app_name";
inline const QString AppAuthor  = "app_author";
inline const QString AppVersion = "app_version";
inline const QString Backup     = "backup";
}

// ------------------------------------------------------------
// Log constants
// ------------------------------------------------------------
namespace Logs {
inline const QString FileSuffix = "_backup_logs.json";
}

// ------------------------------------------------------------
// Directory entry filters
// ------------------------------------------------------------
namespace Filters {
inline constexpr auto All   = QDir::NoDotAndDotDot | QDir::AllEntries;
inline constexpr auto Files = QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs;
inline constexpr auto Dirs  = QDir::NoDotAndDotDot | QDir::Dirs;
inline constexpr auto Logs  = QDir::Files;
inline constexpr auto LogsByTime = QDir::Time;
}

// ------------------------------------------------------------
// Backup scanning
// ------------------------------------------------------------
namespace Scan {
inline constexpr int MinFolderAgeSecs = 5; // ignore very recent folders
}

// ------------------------------------------------------------
// Transfer worker constants
// ------------------------------------------------------------
namespace Transfer {
inline const QString DriveRootSuffix   = ":/";
inline const QString DriveFolderFormat = "%1 (%2 %3)";
inline const QString MkpathCurrentDir  = ".";
inline constexpr auto EntryFilter      = QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files;
}

} // namespace Backup

#endif // BACKUPCONSTANTS_H
