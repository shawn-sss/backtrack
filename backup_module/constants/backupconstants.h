#ifndef BACKUPCONSTANTS_H
#define BACKUPCONSTANTS_H

// Project includes
#include "../../constants/app_info.h"

// Qt includes
#include <QDir>
#include <QString>
#include <QRegularExpression>
#include <QMap>

namespace Backup {

// Drive label constants
namespace Drive {
inline const QString DefaultLabel = "Local Disk";
inline const QString LabelSuffix  = "Drive";
inline const QString RootSuffix   = ":/";
inline const QString FolderFormat = "%1 (%2 %3)";
}

// Timestamp formats and regex
namespace Timestamps {
inline const QString FolderFormat          = "_yyyyMMdd_HHmmss";
inline const QString DisplayFormat         = "MM/dd/yyyy hh:mm AP";
inline const QString NotificationFormat    = "yyyy-MM-dd HH:mm:ss";
inline const QString IsoFormat             = "yyyy-MM-ddTHH:mm:ssZ";
inline const QRegularExpression FolderRegex {R"((\d{8}_\d{6}))"};
}

// Backup log metadata keys
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
inline const QString StagingColumnName   = "Name";
}

// Initial backup config keys
namespace InitConfig {
inline const QString Location   = "location";
inline const QString Timestamp  = "timestamp";
inline const QString AppName    = "app_name";
inline const QString AppAuthor  = "app_author";
inline const QString AppVersion = "app_version";
inline const QString Backup     = "backup";
}

// Log constants
namespace Logs {
inline const QString FileSuffix = App::Items::k_BACKUP_SETUP_CONFIG_LOGS_SUFFIX;
}

// Directory entry filters
namespace Filters {
inline constexpr auto All        = QDir::NoDotAndDotDot | QDir::AllEntries;
inline constexpr auto Files      = QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs;
inline constexpr auto Dirs       = QDir::NoDotAndDotDot | QDir::Dirs;
inline constexpr auto Logs       = QDir::Files;
inline constexpr auto LogsByTime = QDir::Time;
}

// Backup scanning
namespace Scan {
inline constexpr int MinFolderAgeSecs = 5;
}

// Transfer worker constants
namespace Transfer {
inline constexpr auto EntryFilter      = QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files;
}

// Error codes
enum class ErrorCode {
    FileAccessDenied,
    TransferFailed,
    CreateBackupFolder,
    DeletingBackupLog,
    DeletingBackupDirectory,
    BackupLogNotFound,
    BackupAlreadyRunning,
    NoItemsSelected,
    OperationStillRunning,
    CreateAppFolder,
    CreateLogsFolder
};

// Centralized error messages
namespace Errors {
inline const QMap<ErrorCode, QString> Messages {
    {ErrorCode::FileAccessDenied,        "Access denied to file or directory: %1"},
    {ErrorCode::TransferFailed,          "Transfer failed for: %1"},
    {ErrorCode::CreateBackupFolder,      "Failed to create backup folder"},
    {ErrorCode::DeletingBackupLog,       "Error deleting backup log file"},
    {ErrorCode::DeletingBackupDirectory, "Error deleting backup directory"},
    {ErrorCode::BackupLogNotFound,       "Backup log file not found"},
    {ErrorCode::BackupAlreadyRunning,    "A backup operation is already in progress"},
    {ErrorCode::NoItemsSelected,         "No items selected for backup: %1"},
    {ErrorCode::OperationStillRunning,   "Another operation is still running"},
    {ErrorCode::CreateAppFolder,         "Failed to create directory: %1"},
    {ErrorCode::CreateLogsFolder,        "Failed to create logs directory: %1"}
};
}

} // namespace Backup

#endif // BACKUPCONSTANTS_H
