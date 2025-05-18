#ifndef UNITS_CONFIG_H
#define UNITS_CONFIG_H

// File size unit labels and conversion factor
namespace Units::Size {
constexpr auto k_SIZE_UNIT_BYTES = " B";
constexpr auto k_SIZE_UNIT_KILOBYTES = " KB";
constexpr auto k_SIZE_UNIT_MEGABYTES = " MB";
constexpr auto k_SIZE_UNIT_GIGABYTES = " GB";
constexpr double k_SIZE_CONVERSION_FACTOR = 1024.0;
}

// Time duration unit labels
namespace Units::Duration {
constexpr auto k_UNIT_MILLISECONDS = " milliseconds";
constexpr auto k_UNIT_SECONDS = " seconds";
constexpr auto k_UNIT_MINUTES = " minutes";
constexpr auto k_UNIT_HOURS = " hours";
constexpr auto k_UNIT_DAYS = " days";
}

#endif // UNITS_CONFIG_H
