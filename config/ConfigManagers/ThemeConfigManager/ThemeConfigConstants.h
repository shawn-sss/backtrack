#ifndef THEMECONFIGCONSTANTS_H
#define THEMECONFIGCONSTANTS_H

// Qt includes
#include <QString>

namespace ThemeConfigConstants {

// Default string for auto theme
inline constexpr const char* k_DEFAULT_THEME_STRING = "auto";

// Theme options for app appearance
enum class AppTheme {
    Light,
    Dark
};

// Theme options user can choose
enum class UserThemePreference {
    Light,
    Dark,
    Auto
};

// Converts string to UserThemePreference enum
inline UserThemePreference stringToUserThemePreference(const QString& value) {
    if (value.compare(QStringLiteral("light"), Qt::CaseInsensitive) == 0)
        return UserThemePreference::Light;
    if (value.compare(QStringLiteral("dark"), Qt::CaseInsensitive) == 0)
        return UserThemePreference::Dark;
    return UserThemePreference::Auto;
}

// Converts UserThemePreference enum to string
inline QString userThemePreferenceToString(UserThemePreference pref) {
    switch (pref) {
    case UserThemePreference::Light: return QStringLiteral("light");
    case UserThemePreference::Dark:  return QStringLiteral("dark");
    case UserThemePreference::Auto:
    default:                         return QStringLiteral("auto");
    }
}

// QSS file paths
namespace ThemeConstants {
inline constexpr const char* k_BASE_THEME_PATH  = ":/resources/themes/base.qss";
inline constexpr const char* k_DARK_THEME_PATH  = ":/resources/themes/dark.qss";
inline constexpr const char* k_LIGHT_THEME_PATH = ":/resources/themes/light.qss";
}

} // namespace ThemeConfigConstants

#ifdef Q_OS_WIN
// Windows registry keys for theme detection
namespace WindowsThemeRegistry {
inline constexpr auto kThemeRegistryPath    = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
inline constexpr auto kAppsUseLightThemeKey = "AppsUseLightTheme";
}
#endif

#endif // THEMECONFIGCONSTANTS_H
