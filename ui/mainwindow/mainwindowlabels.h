#ifndef MAINWINDOWLABELS_H
#define MAINWINDOWLABELS_H

// C++ includes
#include <QString>

namespace Labels {

namespace Backup {
// Button texts
inline constexpr auto k_ADD_TO_BACKUP_ORIGINAL_TEXT      = "➕ Add to Backup";
inline constexpr auto k_REMOVE_FROM_BACKUP_ORIGINAL_TEXT = "➖ Remove from Backup";
inline constexpr auto k_CREATE_BACKUP_ORIGINAL_TEXT      = "📦 Create Backup";
inline constexpr auto k_CHANGE_DESTINATION_ORIGINAL_TEXT = "📂 Change Destination";
inline constexpr auto k_DELETE_BACKUP_ORIGINAL_TEXT      = "🗑️ Delete Backup";
inline constexpr auto k_NOTIFICATION_ORIGINAL_TEXT       = "🔔 Notifications";
inline constexpr auto k_UNLOCK_DRIVE_ORIGINAL_TEXT       = "🔓 Unlock Drive";
inline constexpr auto k_TEMPLATE_ORIGINAL_TEXT           = "📑 Templates";
inline constexpr auto k_SCHEDULE_ORIGINAL_TEXT           = "⏰ Schedule";

// Status label
inline const QString k_STATUS_LABEL =
    QStringLiteral("<b>Backup Archive Status:</b> %1 %2");

// Status texts
inline constexpr auto k_READY_LABEL     = "Healthy";
inline constexpr auto k_WARNING_LABEL   = "Warning";
inline constexpr auto k_NOT_INITIALIZED = "Uninitialized";

// Metadata
inline constexpr auto k_TOTAL_COUNT     = "Total Backups: ";
inline constexpr auto k_TOTAL_SIZE      = "Total Size: ";
inline constexpr auto k_LOCATION_ACCESS = "Location Access: ";

// Staging
inline constexpr auto k_STAGING_TITLE = "Staging Area";

// Notification feedback
inline constexpr auto k_NOTIFICATION_BUTTON_TEXT = "✅ Notified!";

// Special states
inline constexpr auto k_WARNING_SUMMARY_LABEL      = "⚠ Issues detected";
inline constexpr auto k_APP_AND_BACKUP_ISSUE_LABEL = "App + Backup Issues";
inline constexpr auto k_BACKUP_ISSUE_LABEL         = "Backup Issue";
inline constexpr auto k_APP_ISSUE_LABEL            = "App Issue";
inline constexpr auto k_NO_BACKUPS_COUNT_LABEL     = "No backups yet";
inline constexpr auto k_SEE_NOTIFICATIONS_LABEL    = "See notifications for details";
}

namespace LastBackup {
// Labels
inline constexpr auto k_NAME      = "Name: ";
inline constexpr auto k_TIMESTAMP = "Timestamp: ";
inline constexpr auto k_DURATION  = "Duration: ";
inline constexpr auto k_SIZE      = "Size: ";

// Messages
inline const QString k_NO_BACKUPS_MESSAGE =
    QStringLiteral("No backups have been created yet.");
}

namespace DirectoryStatus {
inline constexpr auto k_WRITABLE  = "Writable";
inline constexpr auto k_READ_ONLY = "Read-only";
inline constexpr auto k_UNKNOWN   = "Unknown";
}

namespace ApplicationStatus {
// Labels
inline constexpr auto k_HEALTHY = "Healthy";
inline constexpr auto k_INVALID = "Invalid";

// Status label
inline const QString k_STATUS_LABEL =
    QStringLiteral("<b>App Config Status:</b> %1 %2");
}

namespace Emoji {
inline constexpr auto k_GREEN  = "🟢";
inline constexpr auto k_YELLOW = "🟡";
inline constexpr auto k_RED    = "🔴";
}

namespace General {
inline constexpr auto k_ELLIPSIS = "...";
}

namespace Notification {
inline const QString k_TIMESTAMP_TEMPLATE = "[%1]";
}

} // namespace Labels

#endif // MAINWINDOWLABELS_H
