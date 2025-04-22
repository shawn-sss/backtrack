#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>

enum class AppTheme {
    Light,
    Dark
};

enum class UserThemePreference {
    Auto,
    Dark,
    Light
};

class ThemeManager {
public:
    static bool isDarkTheme();
    static void applyTheme();
    static void installEventFilter(QObject* target);
    static AppTheme currentTheme();

    static UserThemePreference getUserThemePreference();
    static void setUserThemePreference(UserThemePreference preference);
};

#endif // THEMEMANAGER_H
