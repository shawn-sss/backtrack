#ifndef SCHEDULEDIALOGCONSTANTS_H
#define SCHEDULEDIALOGCONSTANTS_H

namespace ScheduleDialogConstants {

constexpr int k_MIN_WIDTH  = 420;
constexpr int k_MIN_HEIGHT = 220;

constexpr int k_MAIN_MARGIN  = 10;
constexpr int k_MAIN_SPACING = 10;

inline constexpr const char* k_WINDOW_TITLE      = "Schedule";
inline constexpr const char* k_ENABLE_LABEL      = "Enable scheduling";
inline constexpr const char* k_GROUP_TITLE       = "Schedule Settings";
inline constexpr const char* k_DATETIME_LABEL    = "Date & Time";
inline constexpr const char* k_RECURRENCE_LABEL  = "Recurrence";
inline constexpr const char* k_BUTTON_OK_TEXT    = "Save";

inline constexpr const char* k_DATETIME_DISPLAY_FORMAT = "MMM d, yyyy h:mm AP";

inline constexpr const char* k_HINT_DISABLED_HTML =
    "Scheduling is <b>disabled</b>. "
    "<a href=\"#enable\">Enable now</a> to edit date/time and recurrence.";

inline constexpr const char* k_HINT_ENABLED_HTML =
    "Scheduling is <b>enabled</b>. "
    "<a href=\"#disable\">Disable now</a> to turn off the schedule.";

inline constexpr const char* k_LINK_ENABLE  = "#enable";
inline constexpr const char* k_LINK_DISABLE = "#disable";

inline constexpr const char* k_TOOLTIP_LOCKED =
    "Enable scheduling to edit date/time and recurrence.";

inline constexpr const char* k_TIME_INVALID_TITLE = "Invalid Time";
inline constexpr const char* k_TIME_INVALID_BODY  =
    "Please choose a time at the top of a minute that is not earlier than the next minute.\n"
    "Example: If the time is 5:45:40, the earliest valid choice is 5:46:00.";

inline constexpr const char* k_RECURRENCE_ONCE    = "Once";
inline constexpr const char* k_RECURRENCE_DAILY   = "Daily";
inline constexpr const char* k_RECURRENCE_WEEKLY  = "Weekly";
inline constexpr const char* k_RECURRENCE_MONTHLY = "Monthly";

constexpr int k_IDX_ONCE    = 0;
constexpr int k_IDX_DAILY   = 1;
constexpr int k_IDX_WEEKLY  = 2;
constexpr int k_IDX_MONTHLY = 3;

} // namespace ScheduleDialogConstants

#endif // SCHEDULEDIALOGCONSTANTS_H
