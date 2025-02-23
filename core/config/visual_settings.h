#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QString>
#include <QSize>

namespace UIConfig {
// Backup UI Labels
constexpr auto LABEL_BACKUP_FOUND = "Backup Found: ";
constexpr auto LABEL_BACKUP_LOCATION = "Backup Location: ";
constexpr auto LABEL_BACKUP_TOTAL_COUNT = "Backup Total Count: ";
constexpr auto LABEL_BACKUP_TOTAL_SIZE = "Backup Total Size: ";
constexpr auto LABEL_BACKUP_LOCATION_ACCESS = "Backup Location Access: ";

// Last Backup UI Labels
constexpr auto LABEL_LAST_BACKUP_NAME = "Last Backup Name: ";
constexpr auto LABEL_LAST_BACKUP_TIMESTAMP = "Last Backup Timestamp: ";
constexpr auto LABEL_LAST_BACKUP_DURATION = "Last Backup Duration: ";
constexpr auto LABEL_LAST_BACKUP_SIZE = "Last Backup Size: ";

// Toolbar UI Labels
constexpr auto MENU_SETTINGS_LABEL = "Settings";
constexpr auto MENU_EXIT_LABEL = "Exit";
constexpr auto MENU_HELP_LABEL = "Help";
constexpr auto MENU_ABOUT_LABEL = "About";

// Directory Write Status
constexpr auto DIRECTORY_STATUS_WRITABLE = "Writable";
constexpr auto DIRECTORY_STATUS_READ_ONLY = "Read-Only";
constexpr auto DIRECTORY_STATUS_UNKNOWN = "Unknown";

// Tree View UI Settings
constexpr auto STAGING_COLUMN_NAME = "Name";
constexpr int TREE_VIEW_START_HIDDEN_COLUMN = 1;
constexpr int TREE_VIEW_DEFAULT_COLUMN_COUNT = 4;

// Title Bar UI Settings
constexpr int TITLE_BAR_HEIGHT = 40;
}

namespace Styling {
// UI Component HTML Templates
constexpr auto STATUS_LIGHT_ICON_TEMPLATE = "<img src='data:image/png;base64,%1' style='%2'>";
constexpr auto STATUS_LABEL_HTML_TEMPLATE =
    "<div style='display:flex; align-items:center;'>"
    "<span>%1</span><span style='margin-left:4px;'>%2</span>"
    "</div>";

// Icon Styling
constexpr auto ICON_STYLE_TEMPLATE = "width:%1px; height:%1px;";
constexpr QSize TOOLBAR_ICON_SIZE(32, 32);

// General UI Colors
constexpr auto TOOLBAR_BACKGROUND_COLOR = "#222222";
constexpr auto TITLE_BAR_COLOR = "#222";
constexpr auto WINDOW_BACKGROUND_COLOR = "#2B2B2B";

// Title Bar Styling
constexpr auto TITLE_BAR_STYLESHEET = "background-color: #222; color: white; border: none;";
constexpr auto TITLE_LABEL_STYLESHEET = "color: white; font-size: 24px;";

// Window Buttons Labels
constexpr auto WINDOW_CLOSE_BUTTON_LABEL = "X";
constexpr auto WINDOW_MINIMIZE_BUTTON_LABEL = "-";

// Minimize Button Styling
constexpr auto MINIMIZE_BUTTON_STYLESHEET =
    "QPushButton { background: transparent; color: white; border: none; }"
    "QPushButton:hover { background: rgba(255, 255, 255, 0.2); }"
    "QPushButton:pressed { background: rgba(255, 255, 255, 0.3); }";

// Close Button Styling
constexpr auto CLOSE_BUTTON_STYLESHEET =
    "QPushButton { background: transparent; color: white; border: none; }"
    "QPushButton:hover { background: red; color: white; }"
    "QPushButton:pressed { background: darkred; color: white; }";

// Toolbar Styling
constexpr auto TOOLBAR_STYLESHEET = "background-color: #222222; border: none;";

// General Button Styling
constexpr auto BUTTON_STYLE =
    "QPushButton { background: #444; color: white; border-radius: 5px; padding: 5px; }"
    "QPushButton:hover { background: #666; }"
    "QPushButton:pressed { background: #888; }";
}

#endif // UI_CONFIG_H
