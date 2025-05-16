#ifndef NOTIFICATIONSERVICECONSTANTS_H
#define NOTIFICATIONSERVICECONSTANTS_H

// Project includes
#include "../../../../constants/window_config.h"

// Qt includes
#include <QString>

// Notification service values and keys
namespace NotificationSettings {
inline constexpr auto& kNotificationFileName   = App::ConfigFiles::k_APPDATA_SETUP_NOTIFICATIONS_FILE;
inline constexpr auto* k_DEFAULT_WELCOME_MESSAGE  = "👋 Welcome! Thanks for using this app.";
inline constexpr auto& kNotificationFolder        = App::ConfigFiles::k_APPDATA_SETUP_FOLDER;

// Keys used in each notification entry
namespace Fields {
inline constexpr auto* k_MESSAGE   = "message";
inline constexpr auto* k_TIMESTAMP = "timestamp";
inline constexpr auto* k_READ      = "read";
}

} // namespace NotificationSettings

#endif // NOTIFICATIONSERVICECONSTANTS_H
