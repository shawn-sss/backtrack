#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

// Qt includes
#include <QObject>

// Project includes
#include "../../configsettings/visual_settings.h"

// Provides application-wide theme management and system integration
class ThemeManager {
public:
    // Detects if the operating system is using a dark theme
    static bool isDarkTheme();

    // Returns the currently applied application theme
    static AppTheme currentTheme();

    // Retrieves and modifies user theme preference from configuration
    static UserThemePreference getUserThemePreference();
    static void setUserThemePreference(UserThemePreference preference);

    // Applies the theme according to user preference or auto-detection
    static void applyTheme();

    // Installs a native OS event filter to detect system theme changes
    static void installEventFilter(QObject* target);
};

#endif // THEMEMANAGER_H
