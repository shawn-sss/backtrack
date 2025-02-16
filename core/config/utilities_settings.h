#ifndef UTILITIES_SETTINGS_H
#define UTILITIES_SETTINGS_H

#include <QString>

namespace Utilities {
// Default Values
inline const QString DEFAULT_VALUE_NOT_AVAILABLE = "N/A";
}

namespace Colors {
// General Colors
inline const QString COLOR_GREEN = "green";
inline const QString COLOR_RED = "red";
inline const QString COLOR_YELLOW = "yellow";
inline const QString COLOR_TRANSPARENT = "transparent";
inline const QString COLOR_GRAY = "gray";
inline const QString COLOR_WARNING = "orange";
inline const QString COLOR_INFO = "blue";

// Backup Status Colors
inline const QString BACKUP_STATUS_COLOR_FOUND = COLOR_GREEN;
inline const QString BACKUP_STATUS_COLOR_NOT_FOUND = COLOR_RED;
inline const QString BACKUP_STATUS_COLOR_WARNING = COLOR_YELLOW;
}

namespace Numbers {
// File Size Units
inline const QString SIZE_UNIT_BYTES = " B";
inline const QString SIZE_UNIT_KILOBYTES = " KB";
inline const QString SIZE_UNIT_MEGABYTES = " MB";
inline const QString SIZE_UNIT_GIGABYTES = " GB";

// Size Conversion Factor
constexpr double SIZE_CONVERSION_FACTOR = 1024.0;
}

namespace TimeUnits {
// Time Measurement Units
inline const QString UNIT_MILLISECONDS = " milliseconds";
inline const QString UNIT_SECONDS = " seconds";
inline const QString UNIT_MINUTES = " minutes";
inline const QString UNIT_HOURS = " hours";
inline const QString UNIT_DAYS = " days";
}

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
}

#endif // UTILITIES_SETTINGS_H
