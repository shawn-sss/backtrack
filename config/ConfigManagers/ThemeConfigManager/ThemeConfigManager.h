#ifndef THEMECONFIGMANAGER_H
#define THEMECONFIGMANAGER_H

// Qt includes
#include <QObject>

// Project includes
#include "ThemeConfigConstants.h"

class ThemeConfigManager {
public:
    // Checks OS theme setting
    static bool isDarkTheme();

    // Returns current app theme
    static ThemeConfigConstants::AppTheme currentTheme();

    // Gets and sets user theme preference
    static ThemeConfigConstants::UserThemePreference getUserThemePreference();
    static void setUserThemePreference(ThemeConfigConstants::UserThemePreference preference);

    // Applies selected theme to application
    static void applyTheme();

    // Installs event filter to detect system theme changes
    static void installEventFilter(QObject* target);
};

#endif // THEMECONFIGMANAGER_H
