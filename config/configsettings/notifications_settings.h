#ifndef NOTIFICATIONS_SETTINGS_H
#define NOTIFICATIONS_SETTINGS_H

#include <QString>

namespace NotificationSettings {
constexpr auto k_NOTIFICATION_FILE_NAME = "app_notifications.json";
constexpr auto k_DEFAULT_WELCOME_MESSAGE = "👋 Welcome! Thanks for using this app.";

namespace Fields {
constexpr auto k_MESSAGE = "message";
constexpr auto k_TIMESTAMP = "timestamp";
constexpr auto k_READ = "read";
}

} // namespace NotificationSettings

#endif // NOTIFICATIONS_SETTINGS_H
