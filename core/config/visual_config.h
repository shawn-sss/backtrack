#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QString>

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
inline const QString BACKUP_STATUS_COLOR_WARNING = COLOR_YELLOW;


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

// HTML template for embedding status light icon
inline const QString STATUS_LIGHT_ICON_TEMPLATE =
    "<img src='data:image/png;base64,%1' style='%2'>";

// HTML template for combining the status label and the icon
inline const QString STATUS_LABEL_HTML_TEMPLATE =
    "<div style='display:flex; align-items:center;'>"
    "<span>%1</span><span style='margin-left:4px;'>%2</span>"
    "</div>";


inline const QString SIZE_UNIT_MILLISECONDS = " milliseconds";
inline const QString SIZE_UNIT_SECONDS = " seconds";
inline const QString SIZE_UNIT_MINUTES = " minutes";
inline const QString SIZE_UNIT_HOURS = " hours";
inline const QString SIZE_UNIT_DAYS = " days";



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

#endif // UI_CONFIG_H
