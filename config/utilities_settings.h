#ifndef UTILITIES_SETTINGS_H
#define UTILITIES_SETTINGS_H

// Built-in Qt includes
#include <QString>

// Default values
namespace Utilities {
constexpr auto DEFAULT_VALUE_NOT_AVAILABLE = "N/A";
}

// Color definitions
namespace Colors {
constexpr auto COLOR_GREEN = "green";
constexpr auto COLOR_RED = "red";
constexpr auto COLOR_YELLOW = "yellow";
constexpr auto COLOR_GRAY = "gray";
constexpr auto COLOR_TRANSPARENT = "transparent";

constexpr auto COLOR_WARNING = "orange";
constexpr auto COLOR_INFO = "blue";

constexpr auto BACKUP_STATUS_COLOR_FOUND = COLOR_GREEN;
constexpr auto BACKUP_STATUS_COLOR_NOT_FOUND = COLOR_RED;
constexpr auto BACKUP_STATUS_COLOR_WARNING = COLOR_YELLOW;
}

// File size units and conversion
namespace Units {
namespace FileSize {
constexpr auto SIZE_UNIT_BYTES = " B";
constexpr auto SIZE_UNIT_KILOBYTES = " KB";
constexpr auto SIZE_UNIT_MEGABYTES = " MB";
constexpr auto SIZE_UNIT_GIGABYTES = " GB";

constexpr double SIZE_CONVERSION_FACTOR = 1024.0;
}

// Time units
namespace Time {
constexpr auto UNIT_MILLISECONDS = " milliseconds";
constexpr auto UNIT_SECONDS = " seconds";
constexpr auto UNIT_MINUTES = " minutes";
constexpr auto UNIT_HOURS = " hours";
constexpr auto UNIT_DAYS = " days";
}
}

#endif // UTILITIES_SETTINGS_H
