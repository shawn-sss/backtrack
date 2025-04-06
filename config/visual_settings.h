#ifndef UI_CONFIG_H
#define UI_CONFIG_H

// Built-in Qt includes
#include <QSize>
#include <QString>

// Backup UI labels
namespace Labels {
namespace Backup {
constexpr auto STATUS_LIGHT_IMAGE_FORMAT = "PNG";
constexpr auto FOUND = "Backup Found: ";
constexpr auto LOCATION = "Backup Location: ";
constexpr auto TOTAL_COUNT = "Backup Total Count: ";
constexpr auto TOTAL_SIZE = "Backup Total Size: ";
constexpr auto LOCATION_ACCESS = "Backup Location Access: ";
}

// Last backup UI labels
namespace LastBackup {
constexpr auto NAME = "Last Backup Name: ";
constexpr auto TIMESTAMP = "Last Backup Timestamp: ";
constexpr auto DURATION = "Last Backup Duration: ";
constexpr auto SIZE = "Last Backup Size: ";
}

// Toolbar UI labels
namespace Toolbar {
constexpr auto SETTINGS = "Settings";
constexpr auto EXIT = "Exit";
constexpr auto HELP = "Help";
constexpr auto ABOUT = "About";
}
}

// Directory write status labels
namespace DirectoryStatus {
constexpr auto WRITABLE = "Writable";
constexpr auto READ_ONLY = "Read-Only";
constexpr auto UNKNOWN = "Unknown";
}

// Tree view UI settings
namespace UISettings {
namespace TreeView {
constexpr auto STAGING_COLUMN_NAME = "Name";
constexpr int START_HIDDEN_COLUMN = 1;
constexpr int DEFAULT_COLUMN_COUNT = 4;
}
}

// HTML templates for UI components
namespace HTMLTemplates {
constexpr auto STATUS_LIGHT_ICON = "<img src='data:image/png;base64,%1' style='%2'>";
constexpr auto STATUS_LABEL_HTML =
    "<div style='display:flex; align-items:center;'>"
    "<span>%1</span><span style='margin-left:4px;'>%2</span>"
    "</div>";
}

// General UI colors
namespace Colors {
constexpr auto TITLE_BAR = "#222";
constexpr auto WINDOW_BACKGROUND = "#2B2B2B";
}

#endif // UI_CONFIG_H
