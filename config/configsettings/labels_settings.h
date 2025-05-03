#ifndef LABELS_SETTINGS_H
#define LABELS_SETTINGS_H

// Qt includes
#include <QString>

// Labels for backup-related information
namespace Labels::Backup {
constexpr auto k_STATUS_LIGHT_IMAGE_FORMAT = "PNG";
constexpr auto k_FOUND = "Backup Found: ";
constexpr auto k_LOCATION = "Backup Location: ";
constexpr auto k_TOTAL_COUNT = "Backup Total Count: ";
constexpr auto k_TOTAL_SIZE = "Backup Total Size: ";
constexpr auto k_LOCATION_ACCESS = "Backup Location Access: ";

constexpr auto k_CREATE_BACKUP_BUTTON_TEXT = "Create Backup";
constexpr auto k_BACKING_UP_BUTTON_TEXT = "📦 Backing Up...";
}

// Labels for last backup display
namespace Labels::LastBackup {
constexpr auto k_NAME = "Last Backup Name: ";
constexpr auto k_TIMESTAMP = "Last Backup Timestamp: ";
constexpr auto k_DURATION = "Last Backup Duration: ";
constexpr auto k_SIZE = "Last Backup Size: ";
}

// Labels used in the toolbar
namespace Labels::Toolbar {
constexpr auto k_SETTINGS = "Settings";
constexpr auto k_EXIT = "Exit";
constexpr auto k_HELP = "Help";
constexpr auto k_ABOUT = "About";
}

// Labels for directory write status
namespace Labels::DirectoryStatus {
constexpr auto k_WRITABLE = "Writable";
constexpr auto k_READ_ONLY = "Read-Only";
constexpr auto k_UNKNOWN = "Unknown";
}

#endif // LABELS_SETTINGS_H
