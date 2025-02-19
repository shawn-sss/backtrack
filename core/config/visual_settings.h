#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QString>
#include <QSize>

namespace UIConfig {

// Backup UI Labels
inline const QString LABEL_BACKUP_FOUND = "Backup Found: ";
inline const QString LABEL_BACKUP_LOCATION = "Backup Location: ";
inline const QString LABEL_BACKUP_TOTAL_COUNT = "Backup Total Count: ";
inline const QString LABEL_BACKUP_TOTAL_SIZE = "Backup Total Size: ";
inline const QString LABEL_BACKUP_LOCATION_ACCESS = "Backup Location Access: ";

// Last Backup UI Labels
inline const QString LABEL_LAST_BACKUP_NAME = "Last Backup Name: ";
inline const QString LABEL_LAST_BACKUP_TIMESTAMP = "Last Backup Timestamp: ";
inline const QString LABEL_LAST_BACKUP_DURATION = "Last Backup Duration: ";
inline const QString LABEL_LAST_BACKUP_SIZE = "Last Backup Size: ";

// Toolbar UI Labels
inline const QString MENU_SETTINGS_LABEL = "Settings";
inline const QString MENU_EXIT_LABEL = "Exit";
inline const QString MENU_HELP_LABEL = "Help";
inline const QString MENU_ABOUT_LABEL = "About";

// Directory Write Status
inline const QString DIRECTORY_STATUS_WRITABLE = "Writable";
inline const QString DIRECTORY_STATUS_READ_ONLY = "Read-Only";
inline const QString DIRECTORY_STATUS_UNKNOWN = "Unknown";

// Tree View UI Settings
inline const QString STAGING_COLUMN_NAME = "Name";
constexpr int TREE_VIEW_START_HIDDEN_COLUMN = 1;
constexpr int TREE_VIEW_DEFAULT_COLUMN_COUNT = 4;

// Title Bar UI Settings
constexpr int TITLE_BAR_HEIGHT = 40;

} // namespace UIConfig

namespace Styling {

// UI Component HTML Templates
inline const QString STATUS_LIGHT_ICON_TEMPLATE = "<img src='data:image/png;base64,%1' style='%2'>";
inline const QString STATUS_LABEL_HTML_TEMPLATE =
    "<div style='display:flex; align-items:center;'>"
    "<span>%1</span><span style='margin-left:4px;'>%2</span>"
    "</div>";

// Icon Styling
inline const QString ICON_STYLE_TEMPLATE = "width:%1px; height:%1px;";
constexpr auto TOOLBAR_ICON_SIZE = QSize(32, 32);

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
inline const QString MINIMIZE_BUTTON_STYLESHEET =
    "QPushButton { background: transparent; color: white; border: none; }"
    "QPushButton:hover { background: rgba(255, 255, 255, 0.2); }"
    "QPushButton:pressed { background: rgba(255, 255, 255, 0.3); }";

// Close Button Styling
inline const QString CLOSE_BUTTON_STYLESHEET =
    "QPushButton { background: transparent; color: white; border: none; }"
    "QPushButton:hover { background: red; color: white; }"
    "QPushButton:pressed { background: darkred; color: white; }";

// Toolbar Styling
constexpr auto TOOLBAR_STYLESHEET = "background-color: #222222; border: none;";

// General Button Styling
inline const QString BUTTON_STYLE =
    "QPushButton { background: #444; color: white; border-radius: 5px; padding: 5px; }"
    "QPushButton:hover { background: #666; }"
    "QPushButton:pressed { background: #888; }";

} // namespace Styling

#endif // UI_CONFIG_H
