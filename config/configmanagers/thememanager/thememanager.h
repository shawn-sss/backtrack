#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

// Qt includes
#include <QObject>

// Project includes
#include "../../configsettings/visual_settings.h"

// Manages application themes
class ThemeManager {
public:
    // Detect if the system is using a dark theme
    static bool isDarkTheme();

    // Get the current application theme
    static AppTheme currentTheme();

    // Access and modify the user's theme preference
    static UserThemePreference getUserThemePreference();
    static void setUserThemePreference(UserThemePreference preference);

    // Apply the user's selected or auto-detected theme
    static void applyTheme();

    // Install native OS event filter (for theme changes)
    static void installEventFilter(QObject* target);
};

#endif // THEMEMANAGER_H
