// filename: scheduledialogconstants.h

#ifndef SCHEDULEDIALOGCONSTANTS_H
#define SCHEDULEDIALOGCONSTANTS_H

// C++ includes
#include <QString>

namespace ScheduleDialogConstants {

constexpr int k_MIN_WIDTH  = 420;
constexpr int k_MIN_HEIGHT = 220;

inline const QString k_WINDOW_TITLE = QStringLiteral("Schedule");
inline const QString k_ENABLE_LABEL = QStringLiteral("Enable");

inline const QString k_GROUP_TITLE      = QStringLiteral("Schedule settings");
inline const QString k_DATETIME_LABEL   = QStringLiteral("Date & Time");
inline const QString k_RECURRENCE_LABEL = QStringLiteral("Recurrence");

inline const QString k_BUTTON_OK_TEXT = QStringLiteral("Save");

inline const QString k_DATETIME_DISPLAY_FORMAT = QStringLiteral("MMM d, yyyy h:mm AP");

inline const QString k_HINT_DISABLED_HTML =
    QStringLiteral("Scheduling is <b>disabled</b>. "
                   "<a href=\"#enable\">Enable now</a> to edit date/time and recurrence.");

inline const QString k_HINT_ENABLED_HTML =
    QStringLiteral("Scheduling is <b>enabled</b>. "
                   "<a href=\"#disable\">Disable now</a> to turn off the schedule.");

inline const QString k_LINK_ENABLE  = QStringLiteral("#enable");
inline const QString k_LINK_DISABLE = QStringLiteral("#disable");

inline const QString k_TOOLTIP_LOCKED =
    QStringLiteral("Enable scheduling to edit date/time and recurrence.");

inline const QString k_TIME_INVALID_TITLE = QStringLiteral("Invalid Time");
inline const QString k_TIME_INVALID_BODY  =
    QStringLiteral("Please choose a time at the top of a minute that is not earlier than the next minute.\n"
                   "Example: If the time is 5:45:40, the earliest valid choice is 5:46:00.");

inline const QString k_RECURRENCE_ONCE    = QStringLiteral("Once");
inline const QString k_RECURRENCE_DAILY   = QStringLiteral("Daily");
inline const QString k_RECURRENCE_WEEKLY  = QStringLiteral("Weekly");
inline const QString k_RECURRENCE_MONTHLY = QStringLiteral("Monthly");

} // namespace ScheduleDialogConstants

#endif // SCHEDULEDIALOGCONSTANTS_H
