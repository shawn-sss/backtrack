#ifndef MESSAGES_SETTINGS_H
#define MESSAGES_SETTINGS_H

// Built-in Qt includes
#include <QString>

// Information messages
namespace InfoMessages {
constexpr auto SELECT_BACKUP_DESTINATION_TITLE = "Select Backup Destination";
}

// Warning messages
namespace WarningMessages {
constexpr auto WARNING_CONFIRM_BACKUP_DELETION = "Confirm Backup Deletion";
constexpr auto MESSAGE_CONFIRM_BACKUP_DELETION = "Are you sure you want to delete this backup?\nThis action cannot be undone.\n\nBackup Location: %1";

constexpr auto WARNING_OPERATION_STILL_RUNNING = "A background operation is currently running.\nPlease wait for it to complete before proceeding.";
constexpr auto WARNING_BACKUP_OPERATION_RUNNING = "A backup operation is currently in progress.\nPlease wait until it finishes before starting a new one.";
}

// Error messages
namespace ErrorMessages {
constexpr auto BACKUP_LOCATION_REQUIRED_TITLE = "No Backup Location Selected";
constexpr auto ERROR_NO_BACKUP_LOCATION_PATH_SELECTED = "No valid backup location was selected.\nPlease specify a directory.";

constexpr auto BACKUP_SELECTION_REQUIRED_TITLE = "Backup Selection Required";
constexpr auto ERROR_NO_ITEMS_SELECTED_FOR_BACKUP = "No items have been selected for backup.\nPlease choose at least one item and try again.";
constexpr auto REMOVE_SELECTION_REQUIRED_TITLE = "Removal Selection Required";
constexpr auto ERROR_NO_ITEMS_SELECTED_FOR_REMOVAL = "No items have been selected for removal.\nPlease choose at least one item and try again.";
constexpr auto NO_ITEMS_STAGED_FOR_BACKUP_TITLE = "No Items Ready for Backup";
constexpr auto ERROR_NO_ITEMS_STAGED_FOR_BACKUP = "No items are currently staged for backup.\nPlease add files or folders before proceeding.";

constexpr auto BACKUP_DELETION_ERROR_TITLE = "Backup Deletion Failed";
constexpr auto ERROR_BACKUP_DELETE_FAILED = "The selected backup could not be deleted.\nPlease try again.";

constexpr auto BACKUP_VALIDATION_FAILED_TITLE = "Backup Validation Failed";
constexpr auto ERROR_INVALID_BACKUP_DIRECTORY = "The selected folder does not contain a valid backup.\nPlease verify the location and try again.";

constexpr auto BACKUP_DIRECTORY_ERROR_TITLE = "Backup Directory Error";
constexpr auto ERROR_CREATING_BACKUP_DIRECTORY = "Unable to create the backup directory.\nPlease check permissions and try again.";
constexpr auto BACKUP_INITIALIZATION_FAILED_TITLE = "Backup Initialization Failed";
constexpr auto ERROR_CREATING_DEFAULT_BACKUP_DIRECTORY = "Failed to create the default backup directory.\nPlease check your permissions and try again.";

constexpr auto ERROR_BACKUP_ALREADY_IN_PROGRESS = "Backup Already in Progress";
constexpr auto ERROR_OPERATION_IN_PROGRESS = "Ongoing Operation";

constexpr auto ERROR_BACKUP_LOG_NOT_FOUND = "The backup log file was not found.\nDeletion has been canceled.";
constexpr auto ERROR_DELETING_BACKUP_LOG = "Unable to delete the backup log file.\nEnsure it is not in use and try again.";
constexpr auto ERROR_DELETING_BACKUP_DIRECTORY = "Unable to remove the backup directory.\nEnsure all files inside are closed and try again.";
constexpr auto ERROR_CREATING_BACKUP_FOLDER = "Failed to create the backup folder.\nPlease check permissions and try again.";
constexpr auto ERROR_TRANSFER_FAILED = "Unable to transfer the following item: %1";
constexpr auto ERROR_FILE_ACCESS_DENIED = "File access was denied.";

constexpr auto ERROR_CREATE_BACKUP_DIRECTORY = "Failed to create '%1' directory.";
constexpr auto ERROR_CREATE_BACKUP_FILE = "Failed to create '%1' file.";
constexpr auto ERROR_CREATE_BACKUP_FOLDER = "Failed to create '%1' folder.";
}

// About window information
namespace AboutInfo {
constexpr auto ABOUT_WINDOW_TITLE = "About";
constexpr auto AUTHOR_NAME = "Shawn SSS";
constexpr auto ABOUT_WINDOW_MESSAGE = "%2\nVersion %1\n\nDeveloped by %3";
}

// Help window information
namespace HelpInfo {
constexpr auto HELP_WINDOW_TITLE = "Help";
constexpr auto HELP_WINDOW_MESSAGE = "How to create a backup:\n1. Select one or more drives, directories, or files.\n2. Click the add to backup button.\n3. Confirm desired items are selected for backup.\n4. Click the create backup button.";
constexpr auto HELP_EXTENDED_MESSAGE = "\n\nYour settings are stored at:\n%1";
}

#endif // MESSAGES_SETTINGS_H
