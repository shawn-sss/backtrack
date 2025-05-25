#ifndef THEMESERVICECONSTANTS_H
#define THEMESERVICECONSTANTS_H

// Qt includes
#include <QString>

namespace ThemeServiceConstants {

// Default theme string
inline constexpr const char* k_DEFAULT_THEME_STRING = "auto";

// App-wide theme styles
enum class AppTheme {
    Light,
    Dark
};

// User-selectable theme preferences
enum class UserThemePreference {
    Light,
    Dark,
    Auto
};

// Converts a string to UserThemePreference
inline UserThemePreference stringToUserThemePreference(const QString& value) {
    const QString v = value.trimmed().toLower();
    if (v == "light") return UserThemePreference::Light;
    if (v == "dark")  return UserThemePreference::Dark;
    return UserThemePreference::Auto;
}

// Converts UserThemePreference to a string
inline QString userThemePreferenceToString(UserThemePreference pref) {
    switch (pref) {
    case UserThemePreference::Light: return QStringLiteral("light");
    case UserThemePreference::Dark:  return QStringLiteral("dark");
    case UserThemePreference::Auto:
    default:                         return QStringLiteral("auto");
    }
}

// Theme file paths
namespace ThemeConstants {
inline constexpr const char* k_BASE_THEME_PATH  = ":/resources/themes/base.qss";
inline constexpr const char* k_DARK_THEME_PATH  = ":/resources/themes/dark.qss";
inline constexpr const char* k_LIGHT_THEME_PATH = ":/resources/themes/light.qss";
}

} // namespace ThemeServiceConstants

#ifdef Q_OS_WIN
// Windows theme detection registry keys
namespace WindowsThemeRegistry {
inline constexpr auto kThemeRegistryPath    = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
inline constexpr auto kAppsUseLightThemeKey = "AppsUseLightTheme";
}
#endif

#endif // THEMESERVICECONSTANTS_H
