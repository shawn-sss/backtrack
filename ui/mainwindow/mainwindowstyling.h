#ifndef MAINWINDOWSTYLING_H
#define MAINWINDOWSTYLING_H

// Qt includes
#include <QString>

namespace MainWindowStyling {
namespace Styles {

// Backup view container
namespace BackupViewContainer {
inline constexpr auto STYLE = R"(
    #BackupViewContainer {
        border-radius: 10px;
        padding: 10px;
        border: 1px solid #666;
        background-color: #2b2b2b;
    }

    #DriveTreeView,
    #BackupStagingTreeView,
    #BackupDestinationView {
        border: 1px solid #666;
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

// Staging title label
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

// General reusable text styles
namespace GeneralText {
inline constexpr auto k_RED_BOLD_STYLE = "color: red; font-weight: bold;";
inline constexpr auto k_DEFAULT_STYLE  = "color: #ddd;";
inline constexpr auto k_INFO_STYLE     = "color: blue;";
inline constexpr auto k_SUCCESS_STYLE  = "color: green; font-weight: bold;";
}

// Styles for labels with warning semantics
namespace LabelStyles {
inline constexpr auto k_WARNING_LABEL_STYLE = "color: red; font-weight: bold;";
}

// Backup status colors
namespace Visuals {
inline constexpr auto COLOR_GREEN  = "green";
inline constexpr auto COLOR_RED    = "red";
inline constexpr auto COLOR_YELLOW = "orange";

inline constexpr auto BACKUP_STATUS_COLOR_FOUND     = COLOR_GREEN;
inline constexpr auto BACKUP_STATUS_COLOR_NOT_FOUND = COLOR_RED;
inline constexpr auto BACKUP_STATUS_COLOR_WARNING   = COLOR_YELLOW;
}

// Button styles
namespace ButtonStyles {
inline constexpr auto PRIMARY = R"(
    QPushButton {
        background-color: #4CAF50;
        color: white;
        padding: 6px 12px;
        border-radius: 6px;
    }
    QPushButton:hover {
        background-color: #45a049;
    }
)";

inline constexpr auto DANGER = R"(
    QPushButton {
        background-color: #f44336;
        color: white;
        padding: 6px 12px;
        border-radius: 6px;
    }
    QPushButton:hover {
        background-color: #da190b;
    }
)";

inline constexpr auto RESET = R"(
    QPushButton {
        border: none;
        background: transparent;
    }
    QPushButton:hover {
        background: #666;
        border-radius: 4px;
    }
)";
}

// Theme-dependent colors
namespace ThemeColors {
inline constexpr auto DARK_HIGHLIGHT   = "#5E3D74";
inline constexpr auto LIGHT_HIGHLIGHT  = "#3399FF";
inline constexpr auto DARK_TEXT        = "#D4AAFF";
inline constexpr auto LIGHT_TEXT       = "#004A99";
}

} // namespace Styles
} // namespace MainWindowStyling

#endif // MAINWINDOWSTYLING_H
