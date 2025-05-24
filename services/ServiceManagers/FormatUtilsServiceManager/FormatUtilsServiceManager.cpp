// Project includes
#include "FormatUtilsServiceManager.h"
#include "../../../../constants/units_config.h"

// C++ includes
#include <array>

namespace Shared::Formatting {

// Format byte size into human-readable form
QString formatSize(qint64 size) {
    constexpr std::array<const char*, 4> units = {
        Units::Size::k_SIZE_UNIT_BYTES,
        Units::Size::k_SIZE_UNIT_KILOBYTES,
        Units::Size::k_SIZE_UNIT_MEGABYTES,
        Units::Size::k_SIZE_UNIT_GIGABYTES
    };

    int unitIndex = 0;
    double value = static_cast<double>(size);

    while (value >= Units::Size::k_SIZE_CONVERSION_FACTOR && unitIndex < units.size() - 1) {
        value /= Units::Size::k_SIZE_CONVERSION_FACTOR;
        ++unitIndex;
    }

    return QString::number(value, 'f', 2) % " " % units[unitIndex];
}

// Format duration from milliseconds to a human-readable unit
QString formatDuration(qint64 milliseconds) {
    constexpr qint64 MS_PER_SEC = 1000;
    constexpr qint64 SEC_PER_MIN = 60;
    constexpr qint64 MIN_PER_HOUR = 60;
    constexpr qint64 HOUR_PER_DAY = 24;

    if (milliseconds < MS_PER_SEC)
        return QString::number(milliseconds) % Units::Duration::k_UNIT_MILLISECONDS;

    const qint64 seconds = milliseconds / MS_PER_SEC;
    if (seconds < SEC_PER_MIN)
        return QString::number(seconds) % Units::Duration::k_UNIT_SECONDS;

    const qint64 minutes = seconds / SEC_PER_MIN;
    if (minutes < MIN_PER_HOUR)
        return QString::number(minutes) % Units::Duration::k_UNIT_MINUTES;

    const qint64 hours = minutes / MIN_PER_HOUR;
    if (hours < HOUR_PER_DAY)
        return QString::number(hours) % Units::Duration::k_UNIT_HOURS;

    const qint64 days = hours / HOUR_PER_DAY;
    return QString::number(days) % Units::Duration::k_UNIT_DAYS;
}

// Format a timestamp using a custom string format
QString formatTimestamp(const QDateTime& datetime, const QString& format) {
    return datetime.toString(format);
}

// Format a timestamp using a Qt::DateFormat enum
QString formatTimestamp(const QDateTime& datetime, Qt::DateFormat format) {
    return datetime.toString(format);
}

} // namespace Shared::Formatting
