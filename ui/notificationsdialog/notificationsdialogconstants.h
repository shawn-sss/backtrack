// filename: notificationsdialogconstants.h

#ifndef NOTIFICATIONSDIALOGCONSTANTS_H
#define NOTIFICATIONSDIALOGCONSTANTS_H

// C++ includes
#include <QString>

namespace NotificationsDialogConstants {

inline const QString kWindowTitle = QStringLiteral("Notifications");

constexpr int kDialogWidth  = 400;
constexpr int kDialogHeight = 300;

inline const QString kClearAllText = QStringLiteral("Clear All");
inline const QString kCloseText    = QStringLiteral("Close");

inline const QString kClearAllTooltip = QStringLiteral("Delete all notifications");
inline const QString kCloseTooltip    = QStringLiteral("Close the notifications dialog");

inline const QString kNoNotificationsText = QStringLiteral("No notifications.");

inline const QString kConfirmTitle   = QStringLiteral("Confirm Deletion");
inline const QString kConfirmMessage = QStringLiteral("This will delete all notifications permanently. Are you sure?");
inline const QString kClearedTitle   = QStringLiteral("Cleared");
inline const QString kClearedMessage = QStringLiteral("All notifications have been deleted.");

} // namespace NotificationsDialogConstants

#endif // NOTIFICATIONSDIALOGCONSTANTS_H
