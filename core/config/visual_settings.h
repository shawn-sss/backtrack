#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QString>

namespace UIConfig {

// Backup Directory UI Labels
inline const QString LABEL_BACKUP_FOUND = "Backup Found: ";
inline const QString LABEL_BACKUP_LOCATION = "Backup Location: ";
inline const QString LABEL_BACKUP_TOTAL_COUNT = "Backup Total Count: ";
inline const QString LABEL_BACKUP_TOTAL_SIZE = "Backup Total Size: ";
inline const QString LABEL_BACKUP_LOCATION_ACCESS = "Backup Location Access: ";

// Toolbar UI Labels
inline const QString MENU_SETTINGS_LABEL = "Settings";
inline const QString MENU_EXIT_LABEL = "Exit";
inline const QString MENU_HELP_LABEL = "Help";
inline const QString MENU_ABOUT_LABEL = "About";

// Directory Write Status
inline const QString DIRECTORY_STATUS_WRITABLE = "Writable";
inline const QString DIRECTORY_STATUS_READ_ONLY = "Read-Only";
inline const QString DIRECTORY_STATUS_UNKNOWN = "Unknown";

// Last Backup UI Labels
inline const QString LABEL_LAST_BACKUP_NAME = "Last Backup Name: ";
inline const QString LABEL_LAST_BACKUP_TIMESTAMP = "Last Backup Timestamp: ";
inline const QString LABEL_LAST_BACKUP_DURATION = "Last Backup Duration: ";
inline const QString LABEL_LAST_BACKUP_SIZE = "Last Backup Size: ";

// Tree View UI Settings
inline const QString STAGING_COLUMN_NAME = "Name";
constexpr int TREE_VIEW_START_HIDDEN_COLUMN = 1;
constexpr int TREE_VIEW_DEFAULT_COLUMN_COUNT = 4;

// Title Bar UI Settings
constexpr int TITLE_BAR_HEIGHT = 40;

} // namespace UIConfig

namespace Styling {
// HTML Template for Embedding Status Light Icon
inline const QString STATUS_LIGHT_ICON_TEMPLATE =
    "<img src='data:image/png;base64,%1' style='%2'>";

// HTML Template for Combining the Status Label and the Icon
inline const QString STATUS_LABEL_HTML_TEMPLATE =
    "<div style='display:flex; align-items:center;'>"
    "<span>%1</span><span style='margin-left:4px;'>%2</span>"
    "</div>";

// Icon Styling Template
inline const QString ICON_STYLE_TEMPLATE = "width:%1px; height:%1px;";
constexpr auto TOOLBAR_ICON_SIZE = QSize(32, 32);
constexpr auto TOOLBAR_BACKGROUND_COLOR = "#222222";
constexpr auto TITLE_BAR_COLOR = "#222";
constexpr auto WINDOW_CLOSE_BUTTON_LABEL = "X";
constexpr auto WINDOW_MINIMIZE_BUTTON_LABEL = "-";
constexpr auto WINDOW_BACKGROUND_COLOR = "#2B2B2B";
constexpr auto WINDOW_STYLESHEET = "background-color: #2B2B2B; border: none;";
constexpr auto TITLE_BAR_STYLESHEET = "background-color: #222; color: white; border: none;";
constexpr auto TOOLBAR_STYLESHEET = "background-color: #222222; border: none;";

inline const QString BUTTON_STYLE =
    "QPushButton { background: #444; color: white; border-radius: 5px; padding: 5px; }"
    "QPushButton:hover { background: #666; }"
    "QPushButton:pressed { background: #888; }";

}

#endif // UI_CONFIG_H
