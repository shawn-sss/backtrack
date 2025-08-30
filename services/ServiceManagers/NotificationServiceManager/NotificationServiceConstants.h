#ifndef NOTIFICATIONSERVICECONSTANTS_H
#define NOTIFICATIONSERVICECONSTANTS_H

// Project includes
#include "../../../../constants/app_info.h"

// Notification settings
namespace NotificationSettings {
inline constexpr auto& kNotificationFileName = App::Items::k_APPDATA_SETUP_NOTIFICATIONS_FILE;
inline constexpr auto& kNotificationFolder = App::Items::k_APPDATA_SETUP_FOLDER;
inline constexpr const char* kDefaultWelcomeMessage = "👋 Welcome! Thanks for using this app.";

// Notification entry fields
namespace Fields {
inline constexpr const char* kMessage = "message";
inline constexpr const char* kTimestamp = "timestamp";
inline constexpr const char* kRead = "read";
}
}

#endif
