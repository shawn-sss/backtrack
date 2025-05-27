#ifndef MAINWINDOWSTYLING_H
#define MAINWINDOWSTYLING_H

// Qt includes
#include <QString>

namespace MainWindowStyling {
namespace Styles {

// Container styling for main backup view
namespace BackupViewContainer {
inline constexpr auto STYLE = R"(
    #BackupViewContainer {
        border-radius: 10px;
        padding: 10px;
        border: 1px solid;
    }

    #DriveTreeView,
    #BackupStagingTreeView,
    #BackupDestinationView {
        border: 1px solid;
    }

    #BackupLocationLabel,
    #SourceListLabel,
    #StagingListLabel {
        font-size: 10pt;
        font-style: italic;
        color: #ccc;
    }
)";
}

// Styling for the reset SnapList "x" button
namespace ResetSnapListButton {
inline constexpr auto STYLE = R"(
    QPushButton {
        background-color: #a33;
        color: white;
        border: none;
        border-radius: 4px;
        font-size: 10px;
        min-width: 16px;
        min-height: 16px;
        max-width: 16px;
        max-height: 16px;
        padding: 0px;
        margin-left: 4px;
    }
    QPushButton:hover {
        background-color: #c44;
    }
)";
}

// Dashboard container frame styling
namespace DashboardContainer {
inline constexpr auto STYLE = R"(
    #DashboardContainer {
        border: 1px solid black;
        border-radius: 6px;
        padding: 10px;
        background: transparent;
    }
)";
}

// Styling for bold dashboard labels
namespace DashboardLabel {
inline constexpr auto STYLE = R"(
    #DashboardLabel {
        font-size: 18px;
        font-weight: bold;
        padding: 0px;
    }
)";
}

// Styling for the staging list title bar
namespace StagingTitleLabel {
inline constexpr auto STYLE = R"(
    QLabel#StagingListTitleLabel {
        font-weight: bold;
        background-color: #444;
        color: white;
        padding: 4px 10px;
        border-radius: 8px;
    }
)";
}

// Default button styling used in general UI
namespace GeneralButton {
inline constexpr auto STYLE = R"(
    QPushButton {
        background: #444;
        color: white;
    }
    QPushButton:hover {
        background: #666;
    }
    QPushButton:pressed {
        background: #888;
    }
)";
}

// Simple text style constants
namespace GeneralText {
inline constexpr auto k_RED_BOLD_STYLE = "color: red; font-weight: bold;";
inline constexpr auto k_DEFAULT_STYLE = "";
}

// Label visibility and status color styles
namespace LabelStyles {
inline constexpr auto k_WARNING_LABEL_STYLE = "color: red; font-weight: bold;";
inline constexpr auto k_BACKUP_STATUS_STYLE = "color: green;";
inline constexpr auto k_INVISIBLE_LABEL = "visibility: hidden;";
}

// Color constants and HTML layout snippets
namespace Visuals {
inline constexpr auto COLOR_GREEN       = "green";
inline constexpr auto COLOR_RED         = "red";
inline constexpr auto COLOR_YELLOW      = "yellow";
inline constexpr auto COLOR_GRAY        = "gray";
inline constexpr auto COLOR_TRANSPARENT = "transparent";
inline constexpr auto COLOR_WARNING     = "orange";
inline constexpr auto COLOR_INFO        = "blue";

inline constexpr auto TITLE_BAR         = "#222";
inline constexpr auto WINDOW_BACKGROUND = "#2B2B2B";

inline constexpr auto BACKUP_STATUS_COLOR_FOUND     = COLOR_GREEN;
inline constexpr auto BACKUP_STATUS_COLOR_NOT_FOUND = COLOR_RED;
inline constexpr auto BACKUP_STATUS_COLOR_WARNING   = COLOR_YELLOW;

inline constexpr auto STATUS_LIGHT_ICON = "<img src='data:image/png;base64,%1' style='%2'>";
inline constexpr auto STATUS_LABEL_HTML = R"(
    <div style='display:flex; align-items:center;'>
        <span>%1</span><span style='margin-left:4px;'>%2</span>
    </div>
)";
}

// Tooltips for buttons and actions
namespace ToolTips {
inline constexpr auto k_ADD_TO_BACKUP      = "Add selected items to the backup staging area";
inline constexpr auto k_REMOVE_FROM_BACKUP = "Remove selected items from the backup staging area";
inline constexpr auto k_CREATE_BACKUP      = "Start the backup process";
inline constexpr auto k_CHANGE_DESTINATION = "Change the destination folder for backups";
inline constexpr auto k_DELETE_BACKUP      = "Delete the selected backup from the destination view";
inline constexpr auto k_RESET_DESTINATION  = "Delete ALL contents from the backup destination";
inline constexpr auto k_NOTIFICATIONS      = "View backup notifications";
inline constexpr auto k_UNINSTALL          = "Uninstall the application and delete all local data";
inline constexpr auto k_UNLOCK_DRIVE       = "Prompt to unlock a BitLocker-protected drive";
}

} // namespace Styles
} // namespace MainWindowStyling

#endif // MAINWINDOWSTYLING_H
