#ifndef MESSAGES_SETTINGS_H
#define MESSAGES_SETTINGS_H

#include <QString>

namespace InfoMessages {
// Title for selecting destination
constexpr auto SELECT_BACKUP_DESTINATION_TITLE = "Select Backup Destination";
}

namespace WarningMessages {
// Backup delete warning
constexpr auto WARNING_CONFIRM_BACKUP_DELETION = "Confirm Backup Deletion";
constexpr auto MESSAGE_CONFIRM_BACKUP_DELETION = "Are you sure you want to delete this backup?\nThis action cannot be undone.\n\nBackup Location: %1";
}

namespace ErrorMessages {
// Change location
constexpr auto BACKUP_LOCATION_REQUIRED_TITLE = "No Backup Location Selected";
constexpr auto ERROR_NO_BACKUP_LOCATION_PATH_SELECTED = "No valid backup location was selected.\nPlease specify a directory.";

// Add to backup
constexpr auto BACKUP_SELECTION_REQUIRED_TITLE = "Backup Selection Required";
constexpr auto ERROR_NO_ITEMS_SELECTED_FOR_BACKUP = "No items have been selected for backup.\nPlease choose at least one item and try again.";

// Remove from backup
constexpr auto REMOVE_SELECTION_REQUIRED_TITLE = "Removal Selection Required";
constexpr auto ERROR_NO_ITEMS_SELECTED_FOR_REMOVAL = "No items have been selected for removal.\nPlease choose at least one item and try again.";

// Create backup
constexpr auto NO_ITEMS_STAGED_FOR_BACKUP_TITLE = "No Items Ready for Backup";
constexpr auto ERROR_NO_ITEMS_STAGED_FOR_BACKUP = "No items are currently staged for backup.\nPlease add files or folders before proceeding.";

// Delete backup
constexpr auto BACKUP_DELETION_ERROR_TITLE = "Backup Deletion Failed";
constexpr auto ERROR_BACKUP_DELETE_FAILED = "The selected backup could not be deleted.\nPlease try again.";

// Validation backup
constexpr auto BACKUP_VALIDATION_FAILED_TITLE = "Backup Validation Failed";
constexpr auto ERROR_INVALID_BACKUP_DIRECTORY = "The selected folder does not contain a valid backup.\nPlease verify the location and try again.";

// Create backup directory
constexpr auto BACKUP_DIRECTORY_ERROR_TITLE = "Backup Directory Error";
constexpr auto ERROR_CREATING_BACKUP_DIRECTORY = "Unable to create the backup directory.\nPlease check permissions and try again.";

// Ensure backup directory exists
constexpr auto BACKUP_INITIALIZATION_FAILED_TITLE = "Backup Initialization Failed";
constexpr auto ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY = "Failed to create the default backup directory.\nPlease check your permissions and try again.";

// Backup Errors - Signals Only
constexpr auto ERROR_BACKUP_LOG_NOT_FOUND = "The backup log file was not found.\nDeletion has been canceled.";
constexpr auto ERROR_DELETING_BACKUP_LOG = "Unable to delete the backup log file.\nEnsure it is not in use and try again.";
constexpr auto ERROR_DELETING_BACKUP_DIRECTORY = "Unable to remove the backup directory.\nEnsure all files inside are closed and try again.";
constexpr auto ERROR_CREATING_BACKUP_FOLDER = "Failed to create the backup folder.\nPlease check permissions and try again.";
constexpr auto ERROR_TRANSFER_FAILED = "Unable to transfer the following item: %1";
constexpr auto ERROR_FILE_ACCESS_DENIED = "File access was denied.";

// Backup Errors - Error Handling
constexpr auto ERROR_CREATE_BACKUP_DIRECTORY = "Failed to create '%1' directory.";
constexpr auto ERROR_CREATE_BACKUP_FILE = "Failed to create '%1' file.";
constexpr auto ERROR_CREATE_BACKUP_FOLDER = "Failed to create '%1' folder.";

// Backup Errors - Edge Cases
constexpr auto ERROR_OPERATION_IN_PROGRESS = "Ongoing Operation";
constexpr auto WARNING_OPERATION_STILL_RUNNING = "A background operation is currently running.\nPlease wait for it to complete before proceeding.";
constexpr auto ERROR_BACKUP_ALREADY_IN_PROGRESS = "Backup Already in Progress";
constexpr auto WARNING_BACKUP_OPERATION_RUNNING = "A backup operation is currently in progress.\nPlease wait until it finishes before starting a new one.";
}

#endif // MESSAGES_SETTINGS_H
