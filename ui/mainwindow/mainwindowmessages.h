#ifndef MAINWINDOWMESSAGES_H
#define MAINWINDOWMESSAGES_H

// Qt includes
#include <QString>

// General informational messages
namespace InfoMessages {
// Titles and status labels
inline constexpr auto k_SELECT_BACKUP_DESTINATION_TITLE = "Select Backup Destination";
inline const QString k_NOTIFICATION_POPUP_TITLE = QStringLiteral("Notification");
inline constexpr auto k_INSTALL_OK = "ok";
inline constexpr auto k_INSTALL_PARTIAL = "partial";
inline constexpr auto k_INSTALL_BROKEN = "broken";
}

// Warning prompts and confirmations
namespace WarningMessages {
inline constexpr auto k_WARNING_CONFIRM_BACKUP_DELETION = "Confirm Backup Deletion";
inline constexpr auto k_MESSAGE_CONFIRM_BACKUP_DELETION =
    "Are you sure you want to delete this backup?\nThis action cannot be undone.\n\nBackup Location: %1";

inline constexpr auto k_WARNING_OPERATION_STILL_RUNNING =
    "A background operation is currently running.\nPlease wait for it to complete before proceeding.";

inline constexpr auto k_WARNING_BACKUP_OPERATION_RUNNING =
    "A backup operation is currently in progress.\nPlease wait until it finishes before starting a new one.";
}

// Error messages related to backup operations
namespace ErrorMessages {
inline constexpr auto k_BACKUP_LOCATION_REQUIRED_TITLE = "No Backup Location Selected";
inline constexpr auto k_ERROR_NO_BACKUP_LOCATION_PATH_SELECTED =
    "No valid backup location was selected.\nPlease specify a directory.";

inline constexpr auto k_BACKUP_SELECTION_REQUIRED_TITLE = "Backup Selection Required";
inline constexpr auto k_ERROR_NO_ITEMS_SELECTED_FOR_BACKUP =
    "No items have been selected for backup.\nPlease choose at least one item and try again.";

inline constexpr auto k_REMOVE_SELECTION_REQUIRED_TITLE = "Removal Selection Required";
inline constexpr auto k_ERROR_NO_ITEMS_SELECTED_FOR_REMOVAL =
    "No items have been selected for removal.\nPlease choose at least one item and try again.";

inline constexpr auto k_NO_ITEMS_STAGED_FOR_BACKUP_TITLE = "No Items Ready for Backup";
inline constexpr auto k_ERROR_NO_ITEMS_STAGED_FOR_BACKUP =
    "No items are currently staged for backup.\nPlease add files or folders before proceeding.";

inline constexpr auto k_BACKUP_DIRECTORY_ERROR_TITLE = "Backup Directory Error";
inline constexpr auto k_ERROR_CREATING_BACKUP_DIRECTORY =
    "Unable to create the backup directory.\nPlease check permissions and try again.";

inline constexpr auto k_BACKUP_INITIALIZATION_FAILED_TITLE = "Backup Initialization Failed";
inline constexpr auto k_ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY =
    "Failed to create the default backup directory.\nPlease check your permissions and try again.";

inline constexpr auto k_BACKUP_DELETION_ERROR_TITLE = "Backup Deletion Failed";
inline constexpr auto k_ERROR_BACKUP_DELETE_FAILED =
    "The selected backup could not be deleted.\nPlease try again.";

inline constexpr auto k_ERROR_BACKUP_LOG_NOT_FOUND =
    "The backup log file was not found.\nDeletion has been canceled.";

inline constexpr auto k_ERROR_DELETING_BACKUP_LOG =
    "Unable to delete the backup log file.\nEnsure it is not in use and try again.";

inline constexpr auto k_ERROR_DELETING_BACKUP_DIRECTORY =
    "Unable to remove the backup directory.\nEnsure all files inside are closed and try again.";

inline constexpr auto k_ERROR_BACKUP_ALREADY_IN_PROGRESS = "Backup Already in Progress";
inline constexpr auto k_ERROR_OPERATION_IN_PROGRESS = "Ongoing Operation";

inline constexpr auto k_ERROR_TRANSFER_FAILED = "Unable to transfer the following item: %1";
inline constexpr auto k_ERROR_FILE_ACCESS_DENIED = "File access was denied.";
inline constexpr auto k_ERROR_CREATE_BACKUP_FOLDER = "Failed to create '%1' folder.";

inline constexpr auto k_ERROR_DESTINATION_MODEL_NULL =
    "The backup list could not be loaded due to an internal error.\nPlease try again.";

inline constexpr auto k_ERROR_MODEL_TYPE_INVALID =
    "An unexpected error occurred while accessing the backup list.\nPlease try again.";

inline constexpr auto k_ERROR_SELECTED_PATH_INVALID =
    "The selected backup path could not be determined.\nPlease try again.";

inline constexpr auto k_ALREADY_STAGED_TITLE = "Item(s) Already Staged";
inline const QString k_ERROR_ALREADY_IN_STAGING =
    QStringLiteral("Some items are already in the staging list:\n\n%1\n\n"
                   "Please select different items that are not yet staged.");

inline constexpr auto k_READ_ACCESS_DENIED_TITLE = "Read Access Denied";
inline const QString k_READ_ACCESS_DENIED_BODY =
    QStringLiteral("The following items could not be added due to lack of read access:\n%1");
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

inline const QString k_BROKEN_BACKUP_STRUCTURE_MESSAGE =
    QStringLiteral("The backup structure is partially broken. Please review and "
                   "consider cleaning or restoring backups.");

inline const QString k_BROKEN_APP_STRUCTURE_MESSAGE =
    QStringLiteral("The application structure appears to be incomplete or damaged. "
                   "Consider restarting the app or resetting its data to resolve the issue.");
}

// About window labels and content
namespace AboutInfo {
inline constexpr auto k_ABOUT_WINDOW_TITLE = "About";
inline constexpr auto k_AUTHOR_NAME = "Shawn SSS";
inline constexpr auto k_ABOUT_WINDOW_MESSAGE = "%2\nVersion %1\n\nDeveloped by %3";
}

// Help window instructions and messages
namespace HelpInfo {
inline constexpr auto k_HELP_WINDOW_TITLE = "Help";
inline constexpr auto k_HELP_WINDOW_MESSAGE =
    "How to create a backup:\n"
    "1. Select one or more drives, directories, or files.\n"
    "2. Click the add to backup button.\n"
    "3. Confirm desired items are selected for backup.\n"
    "4. Click the create backup button.";

inline constexpr auto k_HELP_EXTENDED_MESSAGE = "\n\nYour settings are stored at:\n%1";
}

// BitLocker unlock messages
namespace BitLockerMessages {
inline constexpr auto k_NO_DRIVE_SELECTED_TITLE = "No Drive Selected";
inline const QString k_NO_DRIVE_SELECTED_MESSAGE =
    QStringLiteral("Please select a drive from the list first.");

inline constexpr auto k_UNLOCK_FAILED_TITLE = "Unlock Failed";
inline const QString k_UNLOCK_FAILED_MESSAGE =
    QStringLiteral("Could not launch BitLocker password prompt.\n"
                   "Ensure PowerShell is available and you have permission to elevate.");
}

#endif // MAINWINDOWMESSAGES_H
