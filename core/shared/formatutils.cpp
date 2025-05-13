// Project includes
#include "formatutils.h"
#include "../../../../constants/units_config.h"

// C++ includes
#include <array>

namespace Shared::Formatting {

QString formatSize(qint64 size) {
    constexpr std::array units = {
        Units::Size::k_SIZE_UNIT_BYTES,
        Units::Size::k_SIZE_UNIT_KILOBYTES,
        Units::Size::k_SIZE_UNIT_MEGABYTES,
        Units::Size::k_SIZE_UNIT_GIGABYTES
    };

    int unitIndex = 0;
    double sizeInUnits = size;

    while (sizeInUnits >= Units::Size::k_SIZE_CONVERSION_FACTOR && unitIndex < units.size() - 1) {
        sizeInUnits /= Units::Size::k_SIZE_CONVERSION_FACTOR;
        ++unitIndex;
    }

    return QString::number(sizeInUnits, 'f', 2) + " " + units[unitIndex];
}

QString formatDuration(qint64 milliseconds) {
    constexpr qint64 MS_IN_SECOND = 1000;
    constexpr qint64 SECONDS_IN_MINUTE = 60;
    constexpr qint64 MINUTES_IN_HOUR = 60;
    constexpr qint64 HOURS_IN_DAY = 24;

    if (milliseconds < MS_IN_SECOND)
        return QString::number(milliseconds) + Units::Duration::k_UNIT_MILLISECONDS;

    qint64 seconds = milliseconds / MS_IN_SECOND;
    if (seconds < SECONDS_IN_MINUTE)
        return QString::number(seconds) + Units::Duration::k_UNIT_SECONDS;

    qint64 minutes = seconds / SECONDS_IN_MINUTE;
    if (minutes < MINUTES_IN_HOUR)
        return QString::number(minutes) + Units::Duration::k_UNIT_MINUTES;

    qint64 hours = minutes / MINUTES_IN_HOUR;
    if (hours < HOURS_IN_DAY)
        return QString::number(hours) + Units::Duration::k_UNIT_HOURS;

    qint64 days = hours / HOURS_IN_DAY;
    return QString::number(days) + Units::Duration::k_UNIT_DAYS;
}

QString formatTimestamp(const QDateTime& datetime, const QString& format) {
    return datetime.toString(format);
}

QString formatTimestamp(const QDateTime& datetime, Qt::DateFormat format) {
    return datetime.toString(format);
}

} // namespace Shared::Formatting
