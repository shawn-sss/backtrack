#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QDir>

// Application Metadata
namespace AppConfig {
constexpr auto WINDOW_TITLE = "MyDataBackupApp";
inline const QString ABOUT_DIALOG_TITLE = "About";
inline const QString ABOUT_DIALOG_MESSAGE = "%1\n%2\n\nMade by Shawn SSS";
inline const QString APP_VERSION = "Version 0.1";
}

// User-Configurable Settings
namespace UserSettings {
inline const QString DEFAULT_BACKUP_DESTINATION = "C:\\temp";
inline const QString BACKUP_FOLDER_PREFIX = "BKP_";
inline const QString BACKUP_SUMMARY_FILENAME = ".backup_summary.json";
}

// Backup-Related Constants
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

// UI Configurations and Labels
namespace UIConfig {
// Colors
inline const QString COLOR_GREEN = "green";
inline const QString COLOR_RED = "red";
inline const QString COLOR_YELLOW = "yellow";
inline const QString COLOR_TRANSPARENT = "transparent";
inline const QString COLOR_GRAY = "gray";
inline const QString COLOR_WARNING = "orange";
inline const QString COLOR_INFO = "blue";

// Backup Status Colors
inline const QString BACKUP_STATUS_COLOR_FOUND = COLOR_GREEN;
inline const QString BACKUP_STATUS_COLOR_NOT_FOUND = COLOR_RED;

// UI Labels
inline const QString LABEL_BACKUP_FOUND = "Backup Found: ";
inline const QString LABEL_BACKUP_STATUS = "Backup Status: ";
inline const QString LABEL_BACKUP_TOTAL_COUNT = "Backup Total Count: ";
inline const QString LABEL_BACKUP_TOTAL_SIZE = "Backup Total Size: ";
inline const QString LABEL_BACKUP_LOCATION = "Backup Location: ";
inline const QString LABEL_BACKUP_LOCATION_ACCESS = "Backup Location Access: ";
inline const QString LABEL_LAST_BACKUP_NAME = "Last Backup Name: ";
inline const QString LABEL_LAST_BACKUP_TIMESTAMP = "Last Backup Timestamp: ";
inline const QString LABEL_LAST_BACKUP_DURATION = "Last Backup Duration: ";
inline const QString LABEL_LAST_BACKUP_SIZE = "Last Backup Size: ";
inline const QString LABEL_BACKUP_PROGRESS = "Backup Progress: ";
inline const QString LABEL_TRANSFER_PROGRESS_COMPLETE = "Transfer Complete";
inline const QString STAGING_COLUMN_NAME = "Name";

// UI Messages
inline const QString MESSAGE_BACKUP_IN_PROGRESS = "A backup operation is currently in progress.";
inline const QString MESSAGE_OPERATION_IN_PROGRESS = "Operation In Progress";
inline const QString MESSAGE_OPERATION_WARNING =
    "A background operation is still running. Please wait for it to complete before proceeding.";

// Confirmation and Deletion Prompts
inline const QString SELECT_BACKUP_DESTINATION_TITLE = "Select Backup Destination";
inline const QString DELETE_BACKUP_CONFIRMATION_MESSAGE =
    "Are you sure you want to permanently delete this backup?\n\nLocation: %1";
inline const QString DELETE_BACKUP_WARNING_TITLE = "Confirm Backup Deletion";
inline const QString DELETE_BACKUP_WARNING_MESSAGE =
    "Are you sure you want to delete this backup?\nThis action cannot be undone.\n\nLocation: %1";

// Warnings
inline const QString EMPTY_STAGING_WARNING_MESSAGE =
    "No items are currently staged. Please add files or folders before creating a backup.";

// Errors
inline const QString ERROR_INVALID_SELECTION = "No items are selected. Please select at least one item and try again.";
inline const QString ERROR_TRANSFER_FAILED = "Unable to transfer the following item: %1";
inline const QString ERROR_FILE_ACCESS_DENIED = "File access was denied.";
inline const QString ERROR_EMPTY_PATH_SELECTION = "No path was selected. Please choose a valid directory.";
inline const QString ERROR_NO_ITEMS_IN_STAGING_TITLE = "No Items Staged";
inline const QString ERROR_NO_ITEMS_IN_STAGING_MESSAGE =
    "There are currently no items staged to remove. Please select items in the staging area first.";

// Styling
inline const QString ICON_STYLE_TEMPLATE = "width:%1px; height:%1px;";
inline const QString DEFAULT_VALUE_NOT_AVAILABLE = "N/A";

// Progress Bar Settings
constexpr int STATUS_LIGHT_SIZE = 8;
constexpr int PROGRESS_BAR_HEIGHT = 20;
constexpr int PROGRESS_BAR_MAX_VALUE = 100;
constexpr int PROGRESS_BAR_MIN_VALUE = 0;
constexpr bool PROGRESS_BAR_TEXT_VISIBLE = true;
constexpr bool PROGRESS_BAR_DEFAULT_VISIBILITY = false;
constexpr int PROGRESS_BAR_DEFAULT_VALUE = PROGRESS_BAR_MIN_VALUE;
constexpr int PROGRESS_BAR_DELAY_MS = 5000;

// Tree View Settings
constexpr int TREE_VIEW_START_HIDDEN_COLUMN = 1;
constexpr int TREE_VIEW_DEFAULT_COLUMN_COUNT = 4;

// Directory Status
inline const QString DIRECTORY_STATUS_WRITABLE = "Writable";
inline const QString DIRECTORY_STATUS_READ_ONLY = "Read-Only";
inline const QString DIRECTORY_STATUS_UNKNOWN = "Unknown";

// Error Titles
inline const QString ERROR_TITLE = "Error";
inline const QString WARNING_TITLE = "Warning";
inline const QString INFO_TITLE = "Information";
inline const QString INVALID_SELECTION_TITLE = "Invalid Selection";
inline const QString BACKUP_ERROR_TITLE = "Backup Failed";
inline const QString EMPTY_SELECTION_TITLE = "No Items Selected";
inline const QString PERMISSION_DENIED_TITLE = "Permission Denied";
inline const QString LOCATION_SELECTION_TITLE = "No Location Selected";
inline const QString NO_FILES_SELECTED_TITLE = "No Files Selected";
inline const QString NO_BACKUP_ITEMS_SELECTED_TITLE = "No Backup Items Selected";
inline const QString INITIALIZATION_ERROR_TITLE = "Initialization Error";
inline const QString INITIALIZATION_ERROR_MESSAGE = "Unable to create the default backup directory.";
}

// Progress Configuration
namespace ProgressConfig {
constexpr int MIN_VALUE = 0;
constexpr int MAX_VALUE = 100;
constexpr bool DEFAULT_VISIBILITY = false;
}

// Backup Metadata Keys
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

#endif // CONSTANTS_H
