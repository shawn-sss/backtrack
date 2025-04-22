#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

// Built-in Qt includes
#include <QObject>

// Enum representing available application themes
enum class AppTheme {
    Light,
    Dark
};

// Enum representing user's theme preference
enum class UserThemePreference {
    Auto,
    Dark,
    Light
};

// Static class for managing application theme behavior
class ThemeManager {
public:
    // System and application theme management
    static bool isDarkTheme();
    static void applyTheme();
    static void installEventFilter(QObject* target);
    static AppTheme currentTheme();

    // User preference storage and retrieval
    static UserThemePreference getUserThemePreference();
    static void setUserThemePreference(UserThemePreference preference);
};

#endif // THEMEMANAGER_H
