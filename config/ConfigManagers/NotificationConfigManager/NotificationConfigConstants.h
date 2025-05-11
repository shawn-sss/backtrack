#ifndef NOTIFICATIONCONFIGCONSTANTS_H
#define NOTIFICATIONCONFIGCONSTANTS_H

// Project includes
#include "../../configsettings/app_settings.h"

// Qt includes
#include <QString>

// Notification config values and keys
namespace NotificationSettings {
inline constexpr auto* k_NOTIFICATION_FILE_NAME    = "app_notifications.json";
inline constexpr auto* k_DEFAULT_WELCOME_MESSAGE   = "👋 Welcome! Thanks for using this app.";
inline constexpr auto& kNotificationFolder         = AppConfig::k_APPDATA_SETUP_FOLDER;

// Keys used in each notification entry
namespace Fields {
inline constexpr auto* k_MESSAGE   = "message";
inline constexpr auto* k_TIMESTAMP = "timestamp";
inline constexpr auto* k_READ      = "read";
}

} // namespace NotificationSettings

#endif // NOTIFICATIONCONFIGCONSTANTS_H
