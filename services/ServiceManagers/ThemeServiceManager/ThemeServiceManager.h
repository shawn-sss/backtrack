#ifndef THEMESERVICEMANAGER_H
#define THEMESERVICEMANAGER_H

// Project includes
#include "ThemeServiceConstants.h"

// Qt includes
#include <QObject>

// Manages application theme logic and synchronization with system preferences
class ThemeServiceManager {
public:
    // Checks OS theme setting
    static bool isDarkTheme();

    // Returns current app theme
    static ThemeServiceConstants::AppTheme currentTheme();

    // Gets and sets user theme preference
    static ThemeServiceConstants::UserThemePreference getUserThemePreference();
    static void setUserThemePreference(ThemeServiceConstants::UserThemePreference preference);

    // Applies selected theme to application
    static void applyTheme();

    // Installs event filter to detect system theme changes
    static void installEventFilter(QObject* target);
};

#endif // THEMESERVICEMANAGER_H
