#ifndef NOTIFICATIONSDIALOGCONSTANTS_H
#define NOTIFICATIONSDIALOGCONSTANTS_H

namespace NotificationsDialogConstants {

// Window title
inline constexpr auto kWindowTitle = "Notifications";

// Dialog size
inline constexpr int kDialogWidth = 400;
inline constexpr int kDialogHeight = 300;

// Button texts
inline constexpr auto kClearAllText = "Clear All";
inline constexpr auto kCloseText = "Close";

// Tooltips
inline constexpr auto kClearAllTooltip = "Delete all notifications";
inline constexpr auto kCloseTooltip = "Close the notifications dialog";

// Default text
inline constexpr auto kNoNotificationsText = "No notifications.";

// Message box strings
inline constexpr auto kConfirmTitle = "Confirm Deletion";
inline constexpr auto kConfirmMessage = "This will delete all notifications permanently. Are you sure?";
inline constexpr auto kClearedTitle = "Cleared";
inline constexpr auto kClearedMessage = "All notifications have been deleted.";

} // namespace NotificationsDialogConstants

#endif // NOTIFICATIONSDIALOGCONSTANTS_H
