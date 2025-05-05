#ifndef VISUAL_SETTINGS_H
#define VISUAL_SETTINGS_H

// Qt includes
#include <QString>
#include <QSize>

// Theme definitions
enum class AppTheme {
    Light,
    Dark
};

enum class UserThemePreference {
    Light,
    Dark,
    Auto
};

// Conversion utilities between string and enum
inline UserThemePreference stringToUserThemePreference(const QString& value) {
    if (value == "light") return UserThemePreference::Light;
    if (value == "dark")  return UserThemePreference::Dark;
    return UserThemePreference::Auto;
}

inline QString userThemePreferenceToString(UserThemePreference pref) {
    switch (pref) {
    case UserThemePreference::Light: return "light";
    case UserThemePreference::Dark:  return "dark";
    case UserThemePreference::Auto:
    default:                         return "auto";
    }
}

// Tree view UI configuration
namespace UISettings::TreeView {
constexpr auto k_STAGING_COLUMN_NAME = "Name";
constexpr int k_START_HIDDEN_COLUMN  = 1;
constexpr int k_DEFAULT_COLUMN_COUNT = 4;
}

// Windows platform-specific settings
namespace PlatformSettings::Windows {
constexpr auto kThemeRegistryPath    = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
constexpr auto kAppsUseLightThemeKey = "AppsUseLightTheme";
}

#endif // VISUAL_SETTINGS_H
