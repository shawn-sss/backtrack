#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// Qt includes
#include <QString>
#include <QStringLiteral>
#include <QSize>
#include <QDir>

// Application metadata
namespace AppInfo {
constexpr auto k_APP_AUTHOR_NAME = "Shawn SSS";
constexpr auto k_APP_NAME = "MyDataBackupApp";
constexpr auto k_APP_VERSION = "0.5";
}

// Application window settings
namespace AppConfig {
constexpr QSize k_MINIMUM_WINDOW_SIZE(500, 250);
constexpr QSize k_DEFAULT_WINDOW_SIZE(1300, 320);
constexpr QSize k_MAXIMUM_WINDOW_SIZE(2000, 800);

// Data file paths
constexpr auto k_APPDATA_SETUP_FOLDER = "app_config";
constexpr auto k_APPDATA_SETUP_INFO_FILE = "app_init.json";
constexpr auto k_APPDATA_SETUP_USER_SETTINGS_FILE = "user_settings.json";
}

#endif // APP_CONFIG_H
