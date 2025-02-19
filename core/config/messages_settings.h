#ifndef MESSAGES_SETTINGS_H
#define MESSAGES_SETTINGS_H

#include <QString>

namespace InfoMessages {
// title for selecting destination
inline const QString SELECT_BACKUP_DESTINATION_TITLE = "Select Backup Destination";
}

namespace WarningMessages {
// backup delete warning
inline const QString WARNING_CONFIRM_BACKUP_DELETION = "Confirm Backup Deletion";
inline const QString MESSAGE_CONFIRM_BACKUP_DELETION = "Are you sure you want to delete this backup?\nThis action cannot be undone.\n\nBackup Location: %1";
}

namespace ErrorMessages {
// change location
inline const QString BACKUP_LOCATION_REQUIRED_TITLE = "No Backup Location Selected";
inline const QString ERROR_NO_BACKUP_LOCATION_PATH_SELECTED = "No valid backup location was selected.\nPlease specify a directory.";

// add to backup
inline const QString BACKUP_SELECTION_REQUIRED_TITLE = "Backup Selection Required";
inline const QString ERROR_NO_ITEMS_SELECTED_FOR_BACKUP = "No items have been selected for backup.\nPlease choose at least one item and try again.";

// remove from backup
inline const QString REMOVE_SELECTION_REQUIRED_TITLE = "Removal Selection Required";
inline const QString ERROR_NO_ITEMS_SELECTED_FOR_REMOVAL = "No items have been selected for removal.\nPlease choose at least one item and try again.";

// create backup
inline const QString NO_ITEMS_STAGED_FOR_BACKUP_TITLE = "No Items Ready for Backup";
inline const QString ERROR_NO_ITEMS_STAGED_FOR_BACKUP = "No items are currently staged for backup.\nPlease add files or folders before proceeding.";

// delete backup
inline const QString BACKUP_DELETION_ERROR_TITLE = "Backup Deletion Failed";
inline const QString ERROR_BACKUP_DELETE_FAILED = "The selected backup could not be deleted.\nPlease try again.";

// validation backup
inline const QString BACKUP_VALIDATION_FAILED_TITLE = "Backup Validation Failed";
inline const QString ERROR_INVALID_BACKUP_DIRECTORY = "The selected folder does not contain a valid backup.\nPlease verify the location and try again.";

// create backup directory
inline const QString BACKUP_DIRECTORY_ERROR_TITLE = "Backup Directory Error";
inline const QString ERROR_CREATING_BACKUP_DIRECTORY = "Unable to create the backup directory.\nPlease check permissions and try again.";

// ensure backup directory exists (not sure this is actually in use)
inline const QString BACKUP_INITIALIZATION_FAILED_TITLE = "Backup Initialization Failed";
inline const QString ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY = "Failed to create the default backup directory.\nPlease check your permissions and try again.";

// Backup Errors - Signals Only

// backup validation failure
inline const QString ERROR_BACKUP_LOG_NOT_FOUND = "The backup log file was not found.\nDeletion has been canceled.";

// backup deletion failure (log file)
inline const QString ERROR_DELETING_BACKUP_LOG = "Unable to delete the backup log file.\nEnsure it is not in use and try again.";

// backup deletion failure (backup directory)
inline const QString ERROR_DELETING_BACKUP_DIRECTORY = "Unable to remove the backup directory.\nEnsure all files inside are closed and try again.";

// backup creation failure
inline const QString ERROR_CREATING_BACKUP_FOLDER = "Failed to create the backup folder.\nPlease check permissions and try again.";

// backup transfer failure
inline const QString ERROR_TRANSFER_FAILED = "Unable to transfer the following item: %1";

//backup transfer access failure
inline const QString ERROR_FILE_ACCESS_DENIED = "File access was denied.";

// Backup Errors - Error Handling

// infrastructure return messages
inline const QString ERROR_CREATE_BACKUP_DIRECTORY = "Failed to create '%1' directory.";
inline const QString ERROR_CREATE_BACKUP_FILE = "Failed to create '%1' file.";
inline const QString ERROR_CREATE_BACKUP_FOLDER = "Failed to create '%1' folder.";

// Backup Errors - Edge Cases

// exiting while a backup is running
inline const QString ERROR_OPERATION_IN_PROGRESS = "Ongoing Operation";
inline const QString WARNING_OPERATION_STILL_RUNNING = "A background operation is currently running.\nPlease wait for it to complete before proceeding.";

// attempting to create a backup while one is already in progress
inline const QString ERROR_BACKUP_ALREADY_IN_PROGRESS = "Backup Already in Progress";
inline const QString WARNING_BACKUP_OPERATION_RUNNING = "A backup operation is currently in progress.\nPlease wait until it finishes before starting a new one.";

}

#endif // MESSAGES_SETTINGS_H
