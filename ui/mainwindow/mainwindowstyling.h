#ifndef MAINWINDOWSTYLING_H
#define MAINWINDOWSTYLING_H

#include <QString>

namespace MainWindowStyling {

namespace Styles {

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

    #ChangeBackupDestinationButton,
    #DeleteBackupButton,
    #AddToBackupButton,
    #CreateBackupButton,
    #RemoveFromBackupButton,
    #NotificationButton {
    }
)";
}

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

namespace DashboardLabel {
constexpr auto STYLE = R"(
    #DashboardLabel {
        font-size: 18px;
        font-weight: bold;
        padding: 0px;
    }
)";
}

namespace GeneralButton {
constexpr auto STYLE = R"(
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
constexpr auto STATUS_LABEL_HTML = R"(
    <div style='display:flex; align-items:center;'>
        <span>%1</span><span style='margin-left:4px;'>%2</span>
    </div>
)";
}

} // namespace Styles

} // namespace MainWindowStyling

#endif // MAINWINDOWSTYLING_H
