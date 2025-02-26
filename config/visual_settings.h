#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QString>
#include <QSize>

// UI Labels
namespace Labels {
// Backup UI Labels
namespace Backup {
constexpr auto STATUS_LIGHT_IMAGE_FORMAT = "PNG";
constexpr auto FOUND = "Backup Found: ";
constexpr auto LOCATION = "Backup Location: ";
constexpr auto TOTAL_COUNT = "Backup Total Count: ";
constexpr auto TOTAL_SIZE = "Backup Total Size: ";
constexpr auto LOCATION_ACCESS = "Backup Location Access: ";
}

// Last Backup UI Labels
namespace LastBackup {
constexpr auto NAME = "Last Backup Name: ";
constexpr auto TIMESTAMP = "Last Backup Timestamp: ";
constexpr auto DURATION = "Last Backup Duration: ";
constexpr auto SIZE = "Last Backup Size: ";
}

// Toolbar UI Labels
namespace Toolbar {
constexpr auto SETTINGS = "Settings";
constexpr auto EXIT = "Exit";
constexpr auto HELP = "Help";
constexpr auto ABOUT = "About";
}

// Window Buttons Labels
namespace WindowButtons {
constexpr auto CLOSE = "X";
constexpr auto MINIMIZE = "_";
}
}

// Directory Write Status Labels
namespace DirectoryStatus {
constexpr auto WRITABLE = "Writable";
constexpr auto READ_ONLY = "Read-Only";
constexpr auto UNKNOWN = "Unknown";
}

// UI Settings
namespace UISettings {
// Tree View UI Settings
namespace TreeView {
constexpr auto STAGING_COLUMN_NAME = "Name";
constexpr int START_HIDDEN_COLUMN = 1;
constexpr int DEFAULT_COLUMN_COUNT = 4;
}

// Title Bar UI Settings
namespace TitleBar {
constexpr int HEIGHT = 40;
}
}

// UI Component Tooltips
namespace Tooltips {
constexpr auto WINDOW_MINIMIZE = "Minimize Window";
constexpr auto WINDOW_CLOSE = "Close Application";
}

// UI Component HTML Templates
namespace HTMLTemplates {
constexpr auto STATUS_LIGHT_ICON = "<img src='data:image/png;base64,%1' style='%2'>";
constexpr auto STATUS_LABEL_HTML =
    "<div style='display:flex; align-items:center;'>"
    "<span>%1</span><span style='margin-left:4px;'>%2</span>"
    "</div>";
}

// Icon Styling
namespace IconStyles {
constexpr auto STYLE_TEMPLATE = "width:%1px; height:%1px;";
constexpr QSize TOOLBAR_SIZE(24, 24);
}

// General UI Colors
namespace Colors {
constexpr auto TOOLBAR_BACKGROUND = "#222222";
constexpr auto TITLE_BAR = "#222";
constexpr auto WINDOW_BACKGROUND = "#2B2B2B";
}

// UI Styling
namespace Styles {
// Title Bar Styling
namespace TitleBar {
constexpr auto STYLESHEET = "background-color: #222; color: white; border: none;";
constexpr auto LABEL_STYLESHEET = "color: white; font-size: 24px;";
}

// Button Styling
namespace Button {
constexpr auto MINIMIZE_STYLESHEET =
    "QPushButton { background: transparent; color: white; border: none; }"
    "QPushButton:hover { background: rgba(255, 255, 255, 0.2); }"
    "QPushButton:pressed { background: rgba(255, 255, 255, 0.3); }";

constexpr auto CLOSE_STYLESHEET =
    "QPushButton { background: transparent; color: white; border: none; }"
    "QPushButton:hover { background: red; color: white; }"
    "QPushButton:pressed { background: darkred; color: white; }";
}

// Toolbar Styling
namespace Toolbar {
constexpr auto GENERAL = "background-color: transparent; border: none; padding: 5px;";
constexpr auto MINIMAL = "background-color: transparent; border: none;";
constexpr auto STYLESHEET = "background-color: #222222; border: none;";
}

// General Button Styling
namespace GeneralButton {
constexpr auto STYLE =
    "QPushButton { background: #444; color: white; border-radius: 5px; padding: 5px; }"
    "QPushButton:hover { background: #666; }"
    "QPushButton:pressed { background: #888; }";
}
}

#endif // UI_CONFIG_H
