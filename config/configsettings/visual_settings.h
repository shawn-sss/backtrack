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

// HTML UI templates
namespace HTMLTemplates {
constexpr auto k_STATUS_LIGHT_ICON = "<img src='data:image/png;base64,%1' style='%2'>";
constexpr auto k_STATUS_LABEL_HTML =
    "<div style='display:flex; align-items:center;'>"
    "<span>%1</span><span style='margin-left:4px;'>%2</span>"
    "</div>";
}

// UI color palette and theme-related colors
namespace UIColors {
constexpr auto k_COLOR_GREEN       = "green";
constexpr auto k_COLOR_RED         = "red";
constexpr auto k_COLOR_YELLOW      = "yellow";
constexpr auto k_COLOR_GRAY        = "gray";
constexpr auto k_COLOR_TRANSPARENT = "transparent";
constexpr auto k_COLOR_WARNING     = "orange";
constexpr auto k_COLOR_INFO        = "blue";

// Theme-specific colors
constexpr auto k_TITLE_BAR          = "#222";
constexpr auto k_WINDOW_BACKGROUND  = "#2B2B2B";

// Backup status indicators
constexpr auto k_BACKUP_STATUS_COLOR_FOUND      = k_COLOR_GREEN;
constexpr auto k_BACKUP_STATUS_COLOR_NOT_FOUND  = k_COLOR_RED;
constexpr auto k_BACKUP_STATUS_COLOR_WARNING    = k_COLOR_YELLOW;
}

// Windows platform-specific settings
namespace PlatformSettings::Windows {
constexpr auto kThemeRegistryPath    = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
constexpr auto kAppsUseLightThemeKey = "AppsUseLightTheme";
}

#endif // VISUAL_SETTINGS_H
