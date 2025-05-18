#ifndef THEMESERVICEMANAGER_H
#define THEMESERVICEMANAGER_H

// Project includes
#include "ThemeServiceConstants.h"

// Qt includes
#include <QObject>

// Manages application theme selection and application
class ThemeServiceManager {
public:
    // Detects whether the system is using dark mode
    static bool isDarkTheme();

    // Returns the currently applied application theme
    static ThemeServiceConstants::AppTheme currentTheme();

    // Gets and sets the user's theme preference
    static ThemeServiceConstants::UserThemePreference getUserThemePreference();
    static void setUserThemePreference(ThemeServiceConstants::UserThemePreference preference);

    // Applies the appropriate theme based on user/system preference
    static void applyTheme();

    // Installs system theme change event filter (Windows only)
    static void installEventFilter(QObject* target);
};

#endif // THEMESERVICEMANAGER_H
