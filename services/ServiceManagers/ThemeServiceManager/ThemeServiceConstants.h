#ifndef THEMESERVICECONSTANTS_H
#define THEMESERVICECONSTANTS_H

// Qt includes
#include <QString>

namespace ThemeServiceConstants {

// Defaults
inline constexpr const char* DefaultThemeString = "auto";

// App-wide theme styles
enum class AppTheme { Light, Dark };

// User-selectable preferences
enum class UserThemePreference { Light, Dark, Auto };

// Conversion helpers
inline UserThemePreference stringToUserThemePreference(const QString& value) {
    const QString v = value.trimmed().toLower();
    if (v == QStringLiteral("light")) return UserThemePreference::Light;
    if (v == QStringLiteral("dark"))  return UserThemePreference::Dark;
    return UserThemePreference::Auto;
}

inline QString userThemePreferenceToString(UserThemePreference pref) {
    switch (pref) {
    case UserThemePreference::Light: return QStringLiteral("light");
    case UserThemePreference::Dark:  return QStringLiteral("dark");
    case UserThemePreference::Auto:
    default:                         return QStringLiteral("auto");
    }
}

// Theme file paths
namespace ThemePaths {
inline constexpr const char* Base = ":/resources/themes/base.qss";
inline constexpr const char* Dark = ":/resources/themes/dark.qss";
inline constexpr const char* Light = ":/resources/themes/light.qss";
}

// Style names
namespace StyleNames {
inline constexpr const char* Fusion = "Fusion";
}

} // namespace ThemeServiceConstants

#ifdef Q_OS_WIN
namespace WindowsThemeRegistry {
inline constexpr const char* ThemeRegistryPath = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
inline constexpr const char* AppsUseLightThemeKey = "AppsUseLightTheme";
}
#endif

#endif
