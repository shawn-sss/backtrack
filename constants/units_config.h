#ifndef UNITS_CONFIG_H
#define UNITS_CONFIG_H

// File size unit labels and conversion factor
namespace Units::Size {
inline constexpr auto k_SIZE_UNIT_BYTES = " B";
inline constexpr auto k_SIZE_UNIT_KILOBYTES = " KB";
inline constexpr auto k_SIZE_UNIT_MEGABYTES = " MB";
inline constexpr auto k_SIZE_UNIT_GIGABYTES = " GB";
constexpr double k_SIZE_CONVERSION_FACTOR = 1024.0;
}

// Time duration unit labels
namespace Units::Duration {
inline constexpr auto k_UNIT_MILLISECONDS = " milliseconds";
inline constexpr auto k_UNIT_SECONDS = " seconds";
inline constexpr auto k_UNIT_MINUTES = " minutes";
inline constexpr auto k_UNIT_HOURS = " hours";
inline constexpr auto k_UNIT_DAYS = " days";
}

#endif // UNITS_CONFIG_H
