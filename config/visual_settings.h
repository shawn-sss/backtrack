#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QSize>
#include <QString>

// Backup UI labels
namespace Labels::Backup {
constexpr auto k_STATUS_LIGHT_IMAGE_FORMAT = "PNG";
constexpr auto k_FOUND = "Backup Found: ";
constexpr auto k_LOCATION = "Backup Location: ";
constexpr auto k_TOTAL_COUNT = "Backup Total Count: ";
constexpr auto k_TOTAL_SIZE = "Backup Total Size: ";
constexpr auto k_LOCATION_ACCESS = "Backup Location Access: ";
}

// Last backup UI labels
namespace Labels::LastBackup {
constexpr auto k_NAME = "Last Backup Name: ";
constexpr auto k_TIMESTAMP = "Last Backup Timestamp: ";
constexpr auto k_DURATION = "Last Backup Duration: ";
constexpr auto k_SIZE = "Last Backup Size: ";
}

// Toolbar UI labels
namespace Labels::Toolbar {
constexpr auto k_SETTINGS = "Settings";
constexpr auto k_EXIT = "Exit";
constexpr auto k_HELP = "Help";
constexpr auto k_ABOUT = "About";
}

// Directory write status labels
namespace DirectoryStatus {
constexpr auto k_WRITABLE = "Writable";
constexpr auto k_READ_ONLY = "Read-Only";
constexpr auto k_UNKNOWN = "Unknown";
}

// Tree view UI settings
namespace UISettings::TreeView {
constexpr auto k_STAGING_COLUMN_NAME = "Name";
constexpr int k_START_HIDDEN_COLUMN = 1;
constexpr int k_DEFAULT_COLUMN_COUNT = 4;
}

// HTML templates for UI components
namespace HTMLTemplates {
constexpr auto k_STATUS_LIGHT_ICON = "<img src='data:image/png;base64,%1' style='%2'>";
constexpr auto k_STATUS_LABEL_HTML =
    "<div style='display:flex; align-items:center;'>"
    "<span>%1</span><span style='margin-left:4px;'>%2</span>"
    "</div>";
}

// General UI colors
namespace Colors {
constexpr auto k_TITLE_BAR = "#222";
constexpr auto k_WINDOW_BACKGROUND = "#2B2B2B";
}

#endif // UI_CONFIG_H
