#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

// Qt includes
#include <QObject>
#include <QStyle>
#include <QApplication>

// Enum representing available application themes
enum class AppTheme {
    Light,
    Dark
};

// Enum representing user's theme preference
enum class UserThemePreference {
    Auto,
    Light,
    Dark
};

// Static class for managing application theme behavior
class ThemeManager {
public:
    static bool isDarkTheme();
    static AppTheme currentTheme();

    static UserThemePreference getUserThemePreference();
    static void setUserThemePreference(UserThemePreference preference);

    static void applyTheme();
    static void installEventFilter(QObject* target);
};

#endif // THEMEMANAGER_H
