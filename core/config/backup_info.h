#ifndef BACKUP_INFO_H
#define BACKUP_INFO_H

#include <QString>
#include <QDir>

namespace BackupInfo {
// Default Paths
constexpr auto ICON_PATH = ":/resources/app_icon.png";
constexpr auto ERROR_ICON_PATH = ":/resources/error_icon.png";
constexpr auto SUCCESS_ICON_PATH = ":/resources/success_icon.png";
constexpr auto BACKUP_STATUS_ICON_FOUND = ":/resources/backup_found_icon.png";
constexpr auto BACKUP_STATUS_ICON_NOT_FOUND = ":/resources/backup_not_found_icon.png";

// File Selection Defaults
inline const QString DEFAULT_ROOT_PATH = "";
inline const QString DEFAULT_DRIVE_LABEL = "Local Disk";
inline const QString DEFAULT_FILE_DIALOG_ROOT = QDir::rootPath();
inline const QString DRIVE_LABEL_SUFFIX = "Drive";

// File System Settings
inline const QDir::Filters FILE_SYSTEM_FILTER = QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files;

// Backup Naming Formats
inline const QString BACKUP_FOLDER_FORMAT = "%1%2";
inline const QString BACKUP_FOLDER_TIMESTAMP_FORMAT = "yyyyMMdd_HHmmss";

// Status Messages
inline const QString BACKUP_COMPLETE_MESSAGE = "The backup completed successfully.";
inline const QString BACKUP_DELETE_SUCCESS = "The backup was successfully deleted.";

// Error Messages
inline const QString ERROR_BACKUP_CREATION_FAILED = "Unable to create the backup.";
inline const QString ERROR_INVALID_BACKUP_LOCATION = "The selected backup location is invalid.";
inline const QString ERROR_BACKUP_DELETION_FAILED = "Unable to delete the selected backup.";
inline const QString ERROR_BACKUP_FOLDER_CREATION_FAILED = "Unable to create the backup folder.";
inline const QString ERROR_INVALID_BACKUP_DESTINATION = "The backup destination is invalid.";
inline const QString ERROR_SET_BACKUP_DESTINATION = "Unable to set the backup destination.";
inline const QString ERROR_BACKUP_OPERATION_RUNNING = "You cannot close the application while a backup is in progress.";

// Validation Messages
inline const QString INVALID_BACKUP_TITLE = "Invalid Backup";
inline const QString INVALID_BACKUP_MESSAGE =
    "The folder you selected does not contain a valid backup. The deletion process has been canceled.";

// Notifications
inline const QString NOTIFY_BACKUP_STARTED = "The backup operation has started.";
inline const QString NOTIFY_BACKUP_COMPLETED = "The backup operation has completed.";
inline const QString NOTIFY_BACKUP_FAILED = "The backup operation has failed.";

// Success Messages
inline const QString SUCCESS_BACKUP_CREATED = "The backup was created successfully.";
inline const QString SUCCESS_BACKUP_DELETED = "The backup was deleted successfully.";

// Timestamp Formats
inline const QString DISPLAY_TIMESTAMP_FORMAT = "dd-MM-yyyy hh:mm:ss";
inline const QString BACKUP_TIMESTAMP_DISPLAY_FORMAT = "MM/dd/yyyy hh:mm AP";

// File Size Units
inline const QString SIZE_UNIT_BYTES = " B";
inline const QString SIZE_UNIT_KILOBYTES = " KB";
inline const QString SIZE_UNIT_MEGABYTES = " MB";
inline const QString SIZE_UNIT_GIGABYTES = " GB";
constexpr double SIZE_CONVERSION_FACTOR = 1024.0;
}

#endif // BACKUP_INFO_H
