#ifndef UTILITIES_SETTINGS_H
#define UTILITIES_SETTINGS_H

#include <QString>

namespace Utilities {
// Default Values
constexpr auto DEFAULT_VALUE_NOT_AVAILABLE = "N/A";
}

namespace Colors {
// Primary Colors
constexpr auto COLOR_GREEN = "green";
constexpr auto COLOR_RED = "red";
constexpr auto COLOR_YELLOW = "yellow";
constexpr auto COLOR_GRAY = "gray";
constexpr auto COLOR_TRANSPARENT = "transparent";

// Informational and Warning Colors
constexpr auto COLOR_WARNING = "orange";
constexpr auto COLOR_INFO = "blue";

// Backup Status Colors
constexpr auto BACKUP_STATUS_COLOR_FOUND = COLOR_GREEN;
constexpr auto BACKUP_STATUS_COLOR_NOT_FOUND = COLOR_RED;
constexpr auto BACKUP_STATUS_COLOR_WARNING = COLOR_YELLOW;
}

namespace Units {
namespace FileSize {
// File Size Units
constexpr auto SIZE_UNIT_BYTES = " B";
constexpr auto SIZE_UNIT_KILOBYTES = " KB";
constexpr auto SIZE_UNIT_MEGABYTES = " MB";
constexpr auto SIZE_UNIT_GIGABYTES = " GB";

// Size Conversion Factor
constexpr double SIZE_CONVERSION_FACTOR = 1024.0;
}

namespace Time {
// Time Measurement Units
constexpr auto UNIT_MILLISECONDS = " milliseconds";
constexpr auto UNIT_SECONDS = " seconds";
constexpr auto UNIT_MINUTES = " minutes";
constexpr auto UNIT_HOURS = " hours";
constexpr auto UNIT_DAYS = " days";
}
}

#endif // UTILITIES_SETTINGS_H
