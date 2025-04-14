#ifndef UTILITIES_SETTINGS_H
#define UTILITIES_SETTINGS_H

#include <QString>

// Default values
namespace Utilities {
constexpr auto k_DEFAULT_VALUE_NOT_AVAILABLE = "N/A";
}

// Color definitions
namespace Colors {
constexpr auto k_COLOR_GREEN = "green";
constexpr auto k_COLOR_RED = "red";
constexpr auto k_COLOR_YELLOW = "yellow";
constexpr auto k_COLOR_GRAY = "gray";
constexpr auto k_COLOR_TRANSPARENT = "transparent";

constexpr auto k_COLOR_WARNING = "orange";
constexpr auto k_COLOR_INFO = "blue";

constexpr auto k_BACKUP_STATUS_COLOR_FOUND = k_COLOR_GREEN;
constexpr auto k_BACKUP_STATUS_COLOR_NOT_FOUND = k_COLOR_RED;
constexpr auto k_BACKUP_STATUS_COLOR_WARNING = k_COLOR_YELLOW;
}

// File size units and conversion
namespace Units::FileSize {
constexpr auto k_SIZE_UNIT_BYTES = " B";
constexpr auto k_SIZE_UNIT_KILOBYTES = " KB";
constexpr auto k_SIZE_UNIT_MEGABYTES = " MB";
constexpr auto k_SIZE_UNIT_GIGABYTES = " GB";

constexpr double k_SIZE_CONVERSION_FACTOR = 1024.0;
}

// Time units
namespace Units::Time {
constexpr auto k_UNIT_MILLISECONDS = " milliseconds";
constexpr auto k_UNIT_SECONDS = " seconds";
constexpr auto k_UNIT_MINUTES = " minutes";
constexpr auto k_UNIT_HOURS = " hours";
constexpr auto k_UNIT_DAYS = " days";
}

#endif // UTILITIES_SETTINGS_H
