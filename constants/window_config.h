#ifndef WINDOW_CONFIG_H
#define WINDOW_CONFIG_H

#include <QSize>

namespace App::Window {
constexpr QSize k_MINIMUM_WINDOW_SIZE(500, 250);
constexpr QSize k_DEFAULT_WINDOW_SIZE(1000, 560);
constexpr QSize k_MAXIMUM_WINDOW_SIZE(2000, 800);
}

namespace App::ConfigFiles {
constexpr auto k_APPDATA_SETUP_NOTIFICATIONS_FILE = "app_notifications.json";
constexpr auto k_APPDATA_SETUP_FOLDER             = "app_config";
constexpr auto k_USERDATA_SETUP_FOLDER            = "user_config";
constexpr auto k_APPDATA_SETUP_INFO_FILE          = "app_init.json";
constexpr auto k_APPDATA_SETUP_USER_SETTINGS_FILE = "user_settings.json";
}

#endif // WINDOW_CONFIG_H
