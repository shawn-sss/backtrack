#ifndef SCHEDULEDIALOGCONSTANTS_H
#define SCHEDULEDIALOGCONSTANTS_H

namespace ScheduleDialogConstants {

// Dimensions
constexpr int k_MIN_WIDTH  = 420;
constexpr int k_MIN_HEIGHT = 220;

// Window title
inline constexpr const char* k_WINDOW_TITLE = "Schedule";

// Header / controls
inline constexpr const char* k_ENABLE_LABEL = "Enable"; // Internal hidden checkbox text

// Group + field labels
inline constexpr const char* k_GROUP_TITLE      = "Schedule settings";
inline constexpr const char* k_DATETIME_LABEL   = "Date & Time";
inline constexpr const char* k_RECURRENCE_LABEL = "Recurrence";

// Buttons
inline constexpr const char* k_BUTTON_OK_TEXT     = "Save";
inline constexpr const char* k_BUTTON_CANCEL_TEXT = "Cancel"; // Defined for completeness

// Display format (12-hour with AM/PM; switch to ISO if needed)
inline constexpr const char* k_DATETIME_DISPLAY_FORMAT = "MMM d, yyyy h:mm AP";

// Hints / status (HTML with inline links)
inline constexpr const char* k_HINT_DISABLED_HTML =
    "Scheduling is <b>disabled</b>. "
    "<a href=\"#enable\">Enable now</a> to edit date/time and recurrence.";

inline constexpr const char* k_HINT_ENABLED_HTML =
    "Scheduling is <b>enabled</b>. "
    "<a href=\"#disable\">Disable now</a> to turn off the schedule.";

// Rich-text link anchors
inline constexpr const char* k_LINK_ENABLE  = "#enable";
inline constexpr const char* k_LINK_DISABLE = "#disable";

// Tooltips
inline constexpr const char* k_TOOLTIP_LOCKED =
    "Enable scheduling to edit date/time and recurrence.";

// Validation messages
inline constexpr const char* k_TIME_INVALID_TITLE = "Invalid Time";
inline constexpr const char* k_TIME_INVALID_BODY  =
    "Please choose a time at the top of a minute that is not earlier than the next minute.\n"
    "Example: If the time is 5:45:40, the earliest valid choice is 5:46:00.";

// Recurrence options
inline constexpr const char* k_RECURRENCE_ONCE    = "Once";
inline constexpr const char* k_RECURRENCE_DAILY   = "Daily";
inline constexpr const char* k_RECURRENCE_WEEKLY  = "Weekly";
inline constexpr const char* k_RECURRENCE_MONTHLY = "Monthly";

} // namespace ScheduleDialogConstants

#endif // SCHEDULEDIALOGCONSTANTS_H
