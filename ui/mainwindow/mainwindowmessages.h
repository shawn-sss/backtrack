#ifndef MAINWINDOWMESSAGES_H
#define MAINWINDOWMESSAGES_H

// Qt includes
#include <QString>

// General informational messages
namespace InfoMessages {
inline constexpr auto k_SELECT_BACKUP_DESTINATION_TITLE = "Select Backup Destination";
inline const QString k_DELETE_ALL_SUCCESS_TITLE = QStringLiteral("Deletion Complete");
inline const QString k_DELETE_ALL_SUCCESS_MESSAGE =
    QStringLiteral("All contents in the backup location have been successfully deleted.");
inline const QString k_NOTIFICATION_POPUP_TITLE = QStringLiteral("Notification");
}

// Warning prompts and confirmations
namespace WarningMessages {
inline constexpr auto k_WARNING_CONFIRM_BACKUP_DELETION  = "Confirm Backup Deletion";
inline constexpr auto k_MESSAGE_CONFIRM_BACKUP_DELETION  =
    "Are you sure you want to delete this backup?\nThis action cannot be undone.\n\nBackup Location: %1";
inline constexpr auto k_WARNING_OPERATION_STILL_RUNNING  =
    "A background operation is currently running.\nPlease wait for it to complete before proceeding.";
inline constexpr auto k_WARNING_BACKUP_OPERATION_RUNNING =
    "A backup operation is currently in progress.\nPlease wait until it finishes before starting a new one.";

inline const QString k_DELETE_ALL_BACKUPS_WARNING_TITLE = QStringLiteral("Confirm Deletion");
inline const QString k_DELETE_ALL_BACKUPS_WARNING_MESSAGE =
    QStringLiteral("⚠️ WARNING: This will permanently delete all files and folders inside:\n\n"
                   "%1\n\n"
                   "This action cannot be undone. All backups and data in this location "
                   "will be lost forever.\n\n"
                   "Are you sure you want to proceed?");
}

// Error messages related to backup operations
namespace ErrorMessages {
inline constexpr auto k_BACKUP_LOCATION_REQUIRED_TITLE           = "No Backup Location Selected";
inline constexpr auto k_ERROR_NO_BACKUP_LOCATION_PATH_SELECTED   =
    "No valid backup location was selected.\nPlease specify a directory.";

inline constexpr auto k_BACKUP_SELECTION_REQUIRED_TITLE          = "Backup Selection Required";
inline constexpr auto k_ERROR_NO_ITEMS_SELECTED_FOR_BACKUP       =
    "No items have been selected for backup.\nPlease choose at least one item and try again.";

inline constexpr auto k_REMOVE_SELECTION_REQUIRED_TITLE          = "Removal Selection Required";
inline constexpr auto k_ERROR_NO_ITEMS_SELECTED_FOR_REMOVAL      =
    "No items have been selected for removal.\nPlease choose at least one item and try again.";

inline constexpr auto k_NO_ITEMS_STAGED_FOR_BACKUP_TITLE         = "No Items Ready for Backup";
inline constexpr auto k_ERROR_NO_ITEMS_STAGED_FOR_BACKUP         =
    "No items are currently staged for backup.\nPlease add files or folders before proceeding.";

inline constexpr auto k_BACKUP_VALIDATION_FAILED_TITLE           = "Backup Validation Failed";
inline constexpr auto k_ERROR_INVALID_BACKUP_DIRECTORY           =
    "The selected folder does not contain a valid backup.\nPlease verify the location and try again.";

inline constexpr auto k_BACKUP_DIRECTORY_ERROR_TITLE             = "Backup Directory Error";
inline constexpr auto k_ERROR_CREATING_BACKUP_DIRECTORY          =
    "Unable to create the backup directory.\nPlease check permissions and try again.";
inline constexpr auto k_BACKUP_INITIALIZATION_FAILED_TITLE       = "Backup Initialization Failed";
inline constexpr auto k_ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY  =
    "Failed to create the default backup directory.\nPlease check your permissions and try again.";

inline constexpr auto k_BACKUP_DELETION_ERROR_TITLE              = "Backup Deletion Failed";
inline constexpr auto k_ERROR_BACKUP_DELETE_FAILED               =
    "The selected backup could not be deleted.\nPlease try again.";
inline constexpr auto k_ERROR_BACKUP_LOG_NOT_FOUND               =
    "The backup log file was not found.\nDeletion has been canceled.";
inline constexpr auto k_ERROR_DELETING_BACKUP_LOG                =
    "Unable to delete the backup log file.\nEnsure it is not in use and try again.";
inline constexpr auto k_ERROR_DELETING_BACKUP_DIRECTORY          =
    "Unable to remove the backup directory.\nEnsure all files inside are closed and try again.";

inline constexpr auto k_ERROR_BACKUP_ALREADY_IN_PROGRESS         = "Backup Already in Progress";
inline constexpr auto k_ERROR_OPERATION_IN_PROGRESS              = "Ongoing Operation";

inline constexpr auto k_ERROR_TRANSFER_FAILED                    = "Unable to transfer the following item: %1";
inline constexpr auto k_ERROR_FILE_ACCESS_DENIED                 = "File access was denied.";
inline constexpr auto k_ERROR_CREATE_BACKUP_DIRECTORY            = "Failed to create '%1' directory.";
inline constexpr auto k_ERROR_CREATE_BACKUP_FILE                 = "Failed to create '%1' file.";
inline constexpr auto k_ERROR_CREATE_BACKUP_FOLDER               = "Failed to create '%1' folder.";

inline constexpr auto k_BACKUP_ERROR_FOLDER_NOT_FOUND            =
    "The backup initialization folder could not be found.\nPlease try again.";
inline constexpr auto k_ERROR_DESTINATION_MODEL_NULL             =
    "The backup list could not be loaded due to an internal error.\nPlease try again.";
inline constexpr auto k_ERROR_MODEL_TYPE_INVALID                 =
    "An unexpected error occurred while accessing the backup list.\nPlease try again.";
inline constexpr auto k_ERROR_SELECTED_PATH_INVALID              =
    "The selected backup path could not be determined.\nPlease try again.";

inline const QString k_BACKUP_LOCATION_INVALID_TITLE = QStringLiteral("Delete All Backups");
inline const QString k_BACKUP_LOCATION_INVALID_MESSAGE =
    QStringLiteral("The backup location is invalid or not set.");
inline const QString k_DELETE_ALL_FAILED_TITLE = QStringLiteral("Deletion Failed");
inline const QString k_DELETE_ALL_FAILED_MESSAGE =
    QStringLiteral("Some files or folders could not be deleted. Please check permissions or try again.");
}

// Notification and backup integrity-related messages
namespace NotificationMessages {
inline const QString k_ORPHANED_LOGS_MESSAGE =
    QStringLiteral("Some backup logs exist without their corresponding backup folders. "
                   "They may have been deleted outside the application.");

inline const QString k_MISSING_LOGS_MESSAGE =
    QStringLiteral("Some backup folders exist without corresponding log files. "
                   "Log metadata may be missing or was deleted. This may affect size "
                   "and count reporting.");

inline const QString k_BROKEN_STRUCTURE_MESSAGE =
    QStringLiteral("The backup structure is partially broken. Please review and "
                   "consider cleaning or restoring backups.");
}

// About window labels and content
namespace AboutInfo {
inline constexpr auto k_ABOUT_WINDOW_TITLE   = "About";
inline constexpr auto k_AUTHOR_NAME          = "Shawn SSS";
inline constexpr auto k_ABOUT_WINDOW_MESSAGE = "%2\nVersion %1\n\nDeveloped by %3";
}

// Help window instructions and messages
namespace HelpInfo {
inline constexpr auto k_HELP_WINDOW_TITLE    = "Help";
inline constexpr auto k_HELP_WINDOW_MESSAGE  =
    "How to create a backup:\n"
    "1. Select one or more drives, directories, or files.\n"
    "2. Click the add to backup button.\n"
    "3. Confirm desired items are selected for backup.\n"
    "4. Click the create backup button.";

inline constexpr auto k_HELP_EXTENDED_MESSAGE = "\n\nYour settings are stored at:\n%1";
}

#endif // MAINWINDOWMESSAGES_H
