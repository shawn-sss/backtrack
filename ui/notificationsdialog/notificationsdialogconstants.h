#ifndef NOTIFICATIONSDIALOGCONSTANTS_H
#define NOTIFICATIONSDIALOGCONSTANTS_H

namespace NotificationsDialogConstants {

constexpr int k_DIALOG_WIDTH  = 400;
constexpr int k_DIALOG_HEIGHT = 300;

constexpr int k_MAIN_MARGIN   = 10;
constexpr int k_MAIN_SPACING  = 8;

inline constexpr const char* k_WINDOW_TITLE             = "Notifications";
inline constexpr const char* k_CLEAR_ALL_TEXT           = "Clear All";
inline constexpr const char* k_NO_NOTIFICATIONS_TEXT    = "No notifications.";

inline constexpr const char* k_CONFIRM_TITLE   = "Confirm Deletion";
inline constexpr const char* k_CONFIRM_MESSAGE =
    "This will delete all notifications permanently. Are you sure?";
inline constexpr const char* k_CLEARED_TITLE   = "Cleared";
inline constexpr const char* k_CLEARED_MESSAGE = "All notifications have been deleted.";

} // namespace NotificationsDialogConstants

#endif // NOTIFICATIONSDIALOGCONSTANTS_H
