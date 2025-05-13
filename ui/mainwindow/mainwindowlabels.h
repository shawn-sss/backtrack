#ifndef MAINWINDOWLABELS_H
#define MAINWINDOWLABELS_H

// Qt includes
#include <QString>

// Backup-related labels and buttons
namespace Labels::Backup {
inline constexpr auto k_STATUS_LIGHT_IMAGE_FORMAT        = "PNG";
inline constexpr auto k_FOUND                            = "Backup Found: ";
inline constexpr auto k_LOCATION                         = "Backup Location: ";
inline constexpr auto k_TOTAL_COUNT                      = "Backup Total Count: ";
inline constexpr auto k_TOTAL_SIZE                       = "Backup Total Size: ";
inline constexpr auto k_LOCATION_ACCESS                  = "Backup Location Access: ";

inline constexpr auto k_NOTIFICATION_BUTTON_TEXT         = "Notifications";
inline constexpr auto k_NOTIFICATION_FEEDBACK_TEXT       = "🔔 Opened!";
inline constexpr auto k_BACKING_UP_BUTTON_TEXT           = "📦 Backing Up...";
inline constexpr auto k_CREATE_BACKUP_BUTTON_TEXT        = "Create Backup";

inline constexpr auto k_ADD_TO_BACKUP_ORIGINAL_TEXT      = "Add to Backup";
inline constexpr auto k_REMOVE_FROM_BACKUP_ORIGINAL_TEXT = "Remove from Backup";
inline constexpr auto k_DELETE_BACKUP_ORIGINAL_TEXT      = "Delete Backup";
inline constexpr auto k_CHANGE_DESTINATION_ORIGINAL_TEXT = "Change Location";

inline constexpr auto k_ADD_TO_BACKUP_BUTTON_TEXT        = "➕ Added!";
inline constexpr auto k_REMOVE_FROM_BACKUP_BUTTON_TEXT   = "➖ Removed!";
inline constexpr auto k_DELETE_BACKUP_BUTTON_TEXT        = "🗑️ Deleted!";
inline constexpr auto k_CHANGE_DESTINATION_BUTTON_TEXT   = "📁 Changed!";
}

// Last backup information labels
namespace Labels::LastBackup {
inline constexpr auto k_NAME      = "Last Backup Name: ";
inline constexpr auto k_TIMESTAMP = "Last Backup Timestamp: ";
inline constexpr auto k_DURATION  = "Last Backup Duration: ";
inline constexpr auto k_SIZE      = "Last Backup Size: ";
}

// Toolbar action labels
namespace Labels::Toolbar {
inline constexpr auto k_SETTINGS = "Settings";
inline constexpr auto k_HELP     = "Help";
inline constexpr auto k_ABOUT    = "About";
inline constexpr auto k_EXIT     = "Exit";
}

// Directory status labels
namespace Labels::DirectoryStatus {
inline constexpr auto k_WRITABLE  = "Writable";
inline constexpr auto k_READ_ONLY = "Read-Only";
inline constexpr auto k_UNKNOWN   = "Unknown";
}

#endif // MAINWINDOWLABELS_H
