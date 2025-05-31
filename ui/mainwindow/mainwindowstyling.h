#ifndef MAINWINDOWSTYLING_H
#define MAINWINDOWSTYLING_H

// Qt includes
#include <QString>

namespace MainWindowStyling {
namespace Styles {

// Styling for the main backup view container and internal elements
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

// Styling for the staging list title label
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

// Simple reusable text styles
namespace GeneralText {
inline constexpr auto k_RED_BOLD_STYLE = "color: red; font-weight: bold;";
inline constexpr auto k_DEFAULT_STYLE  = "";
}

// Styles for labels with warning semantics
namespace LabelStyles {
inline constexpr auto k_WARNING_LABEL_STYLE = "color: red; font-weight: bold;";
}

// Backup status color codes
namespace Visuals {
inline constexpr auto COLOR_GREEN  = "green";
inline constexpr auto COLOR_RED    = "red";
inline constexpr auto COLOR_YELLOW = "yellow";

inline constexpr auto BACKUP_STATUS_COLOR_FOUND     = COLOR_GREEN;
inline constexpr auto BACKUP_STATUS_COLOR_NOT_FOUND = COLOR_RED;
inline constexpr auto BACKUP_STATUS_COLOR_WARNING   = COLOR_YELLOW;
}

} // namespace Styles
} // namespace MainWindowStyling

#endif // MAINWINDOWSTYLING_H
