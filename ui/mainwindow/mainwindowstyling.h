#ifndef MAINWINDOWSTYLING_H
#define MAINWINDOWSTYLING_H

// C++ includes
#include <QString>

namespace MainWindowStyling {
namespace Styles {

// Backup view container
namespace BackupViewContainer {
inline const QString STYLE =
    "QWidget#BackupViewContainer {"
    "   border: 1px solid #444444;"
    "   border-radius: 6px;"
    "   padding: 4px;"
    "   background-color: #2d2d2d;"
    "}";
}

// General text
namespace GeneralText {
inline const QString k_DEFAULT_STYLE =
    "QLabel {"
    "   color: #e0e0e0;"
    "   font-size: 13px;"
    "}";
}

// Label styles
namespace LabelStyles {
inline const QString k_WARNING_LABEL_STYLE =
    "QLabel {"
    "   color: #ffcc00;"
    "   font-weight: bold;"
    "}";
inline const QString k_ERROR_LABEL_STYLE =
    "QLabel {"
    "   color: #F44336;"
    "   font-weight: bold;"
    "}";
}

// Visual indicators
namespace Visuals {
inline const QString BACKUP_STATUS_COLOR_FOUND     = "#4CAF50";
inline const QString BACKUP_STATUS_COLOR_WARNING   = "#FFC107";
inline const QString BACKUP_STATUS_COLOR_NOT_FOUND = "#F44336";
}

// Theme colors
namespace ThemeColors {
inline const QString DARK_HIGHLIGHT  = "#1E88E5";
inline const QString LIGHT_HIGHLIGHT = "#64B5F6";
inline const QString DARK_TEXT       = "#FFFFFF";
inline const QString LIGHT_TEXT      = "#000000";
}

} // namespace Styles
} // namespace MainWindowStyling

#endif // MAINWINDOWSTYLING_H
