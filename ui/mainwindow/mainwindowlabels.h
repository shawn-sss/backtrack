#ifndef MAINWINDOWLABELS_H
#define MAINWINDOWLABELS_H

// Qt includes
#include <QString>

// Backup-related labels and button texts
namespace Labels::Backup {
inline constexpr auto k_FOUND                            = "Backup Found: ";
inline constexpr auto k_TOTAL_COUNT                      = "Backup Total Count: ";
inline constexpr auto k_TOTAL_SIZE                       = "Backup Total Size: ";
inline constexpr auto k_LOCATION_ACCESS                  = "Backup Location Access: ";

inline constexpr auto k_ADD_TO_BACKUP_ORIGINAL_TEXT      = "➕ Add to Backup";
inline constexpr auto k_REMOVE_FROM_BACKUP_ORIGINAL_TEXT = "➖ Remove from Backup";
inline constexpr auto k_DELETE_BACKUP_ORIGINAL_TEXT      = "🗑️ Delete Backup";
inline constexpr auto k_CHANGE_DESTINATION_ORIGINAL_TEXT = "📁 Change Location";
inline constexpr auto k_CREATE_BACKUP_ORIGINAL_TEXT      = "📦 Create Backup";
inline constexpr auto k_NOTIFICATION_ORIGINAL_TEXT       = "🔔 Notifications";
inline constexpr auto k_UNLOCK_DRIVE_ORIGINAL_TEXT       = "🔓 Unlock Drive";
inline constexpr auto k_SNAP_LIST_ORIGINAL_TEXT          = "📋 SnapList";
inline constexpr auto k_SCHEDULE_ORIGINAL_TEXT           = "📅 Schedule";

inline constexpr auto k_ADD_TO_BACKUP_BUTTON_TEXT        = "✅ Added!";
inline constexpr auto k_REMOVE_FROM_BACKUP_BUTTON_TEXT   = "✅ Removed!";
inline constexpr auto k_DELETE_BACKUP_BUTTON_TEXT        = "✅ Deleted!";
inline constexpr auto k_CHANGE_DESTINATION_BUTTON_TEXT   = "✅ Changed!";
inline constexpr auto k_CREATE_BACKUP_BUTTON_TEXT        = "✅ Created!";
inline constexpr auto k_NOTIFICATION_BUTTON_TEXT         = "✅ Opened!";
inline constexpr auto k_UNLOCK_DRIVE_BUTTON_TEXT         = "✅ Unlocked!";
inline constexpr auto k_SNAP_LIST_BUTTON_TEXT            = "✅ Loaded!";

inline constexpr auto k_BACKING_UP_BUTTON_TEXT           = "📦 Backing Up...";
inline constexpr auto k_UNLOCKING_FEEDBACK_TEXT          = "🔒 Unlocking...";

inline constexpr auto k_READY_LABEL                      = "OK";
inline constexpr auto k_WARNING_LABEL                    = "Warning";
inline constexpr auto k_NOT_INITIALIZED                  = "Uninitialized";

inline constexpr auto k_WARNING_SUMMARY_LABEL            = "WARNING!";
inline constexpr auto k_APP_AND_BACKUP_ISSUE_LABEL       = "App and Backup issues detected.";
inline constexpr auto k_BACKUP_ISSUE_LABEL               = "Backup issue detected.";
inline constexpr auto k_APP_ISSUE_LABEL                  = "App issue detected.";
inline constexpr auto k_NO_BACKUPS_COUNT_LABEL           = "Backup archive location uninitialized.";
inline constexpr auto k_SEE_NOTIFICATIONS_LABEL          = "See Notifications for more details.";

inline constexpr auto k_STAGING_TITLE                    = "Backup Staging";
inline constexpr auto k_SNAPLIST_RESET_BUTTON_TXT        = "✕";

inline const QString k_STATUS_LABEL = QStringLiteral("<b>Backup Archive Status:</b> %1 %2");
}

// Last backup information labels
namespace Labels::LastBackup {
inline constexpr auto k_NAME      = "Last Backup Name: ";
inline constexpr auto k_TIMESTAMP = "Last Backup Timestamp: ";
inline constexpr auto k_DURATION  = "Last Backup Duration: ";
inline constexpr auto k_SIZE      = "Last Backup Size: ";

inline const QString k_NO_BACKUPS_MESSAGE = QStringLiteral("No backups yet. Create one to see details here.");
}

// Directory status labels
namespace Labels::DirectoryStatus {
inline constexpr auto k_WRITABLE  = "Writable";
inline constexpr auto k_READ_ONLY = "Read-Only";
inline constexpr auto k_UNKNOWN   = "Unknown";
}

// Application status labels
namespace Labels::ApplicationStatus {
inline constexpr auto k_HEALTHY = "Healthy";
inline constexpr auto k_INVALID = "Invalid";

inline const QString k_STATUS_LABEL = QStringLiteral("<b>App Config Status:</b> %1 %2");
}

// Emoji symbols for status visuals
namespace Labels::Emoji {
inline constexpr auto k_GREEN  = "🟢";
inline constexpr auto k_YELLOW = "🟡";
inline constexpr auto k_RED    = "🔴";
}

// General-purpose literals
namespace Labels::General {
inline constexpr auto k_ELLIPSIS = "...";
}

// Notification-related
namespace Labels::Notification {
inline const QString k_TIMESTAMP_TEMPLATE = QStringLiteral("[%1]");
}

#endif // MAINWINDOWLABELS_H
