#ifndef MAINWINDOWSTYLING_H
#define MAINWINDOWSTYLING_H

#include <QString>

namespace MainWindowStyling {

// Unified style for short feedback
inline const QString BUTTON_FEEDBACK_STYLE = QStringLiteral(
    "QPushButton {"
    "  background-color: #64B5F6;"
    "  color: black;"
    "  font-weight: bold;"
    "  border: 1px solid black;"
    "  border-radius: 5px;"
    "}"
    );

namespace Styles {

// Styles for the main backup view container and its components
namespace BackupViewContainer {
constexpr auto STYLE = R"(
    #BackupViewContainer {
        border-radius: 10px;
        padding: 10px;
        border: 1px solid;
    }

    #DestinationTreeTitleLabel,
    #SourceTreeTitleLabel,
    #StagingListTitleLabel {
        font-weight: bold;
    }

    #DriveTreeView,
    #BackupStagingTreeView,
    #BackupDestinationView {
        border: 1px solid;
    }

    #MainToolBar QToolButton {
        border-radius: 16px;
        padding: 8px;
        margin: 4px;
        min-width: 40px;
        min-height: 40px;
        font-size: 14px;
        border: none;
    }

    #ChangeBackupDestinationButton,
    #DeleteBackupButton,
    #AddToBackupButton,
    #CreateBackupButton,
    #RemoveFromBackupButton {
        border: 1px solid;
        border-radius: 6px;
        padding: 6px 12px;
    }
)";
}

// Dashboard container style
namespace DashboardContainer {
constexpr auto STYLE = R"(
    #DashboardContainer {
        border: 1px solid black;
        border-radius: 6px;
        padding: 10px;
        background: transparent;
    }
)";
}

// Dashboard label style
namespace DashboardLabel {
constexpr auto STYLE = R"(
    #DashboardLabel {
        font-size: 18px;
        font-weight: bold;
        padding: 0px;
    }
)";
}

// Generic push button styling
namespace GeneralButton {
constexpr auto STYLE =
    "QPushButton { background: #444; color: white; border-radius: 5px; padding: 5px; }"
    "QPushButton:hover { background: #666; }"
    "QPushButton:pressed { background: #888; }";
}

// UI colors, layout HTML, etc.
namespace Visuals {
constexpr auto COLOR_GREEN       = "green";
constexpr auto COLOR_RED         = "red";
constexpr auto COLOR_YELLOW      = "yellow";
constexpr auto COLOR_GRAY        = "gray";
constexpr auto COLOR_TRANSPARENT = "transparent";
constexpr auto COLOR_WARNING     = "orange";
constexpr auto COLOR_INFO        = "blue";

constexpr auto TITLE_BAR         = "#222";
constexpr auto WINDOW_BACKGROUND = "#2B2B2B";

constexpr auto BACKUP_STATUS_COLOR_FOUND     = COLOR_GREEN;
constexpr auto BACKUP_STATUS_COLOR_NOT_FOUND = COLOR_RED;
constexpr auto BACKUP_STATUS_COLOR_WARNING   = COLOR_YELLOW;

constexpr auto STATUS_LIGHT_ICON = "<img src='data:image/png;base64,%1' style='%2'>";
constexpr auto STATUS_LABEL_HTML =
    "<div style='display:flex; align-items:center;'>"
    "<span>%1</span><span style='margin-left:4px;'>%2</span>"
    "</div>";
}

} // namespace Styles

} // namespace MainWindowStyling

#endif // MAINWINDOWSTYLING_H
