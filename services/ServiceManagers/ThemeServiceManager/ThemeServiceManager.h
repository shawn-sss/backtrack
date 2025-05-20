#ifndef THEMESERVICEMANAGER_H
#define THEMESERVICEMANAGER_H

// Project includes
#include "ThemeServiceConstants.h"

// Qt includes
#include <QObject>

#ifdef Q_OS_WIN
#include <QAbstractNativeEventFilter>
#endif

// Forward declaration (Qt class)
#ifdef Q_OS_WIN
class ThemeChangeFilter;
#endif

class ThemeServiceManager : public QObject {
    Q_OBJECT

public:
    // Returns the singleton instance
    static ThemeServiceManager& instance();

    // Returns true if the system is using dark mode
    bool isDarkTheme() const;

    // Returns the currently applied theme
    ThemeServiceConstants::AppTheme currentTheme() const;

    // Returns the saved user theme preference
    ThemeServiceConstants::UserThemePreference getUserThemePreference() const;

    // Sets the user preference and applies it
    void setUserThemePreference(ThemeServiceConstants::UserThemePreference preference);

    // Applies the theme based on preference and system setting
    void applyTheme();

    // Installs a native event filter (Windows only)
    void installEventFilter(QObject* target = nullptr);

signals:
    // Emitted after theme is applied
    void themeChanged();

private:
    // Constructor
    explicit ThemeServiceManager(QObject* parent = nullptr);

#ifdef Q_OS_WIN
    static ThemeChangeFilter* eventFilter;
#endif

    ThemeServiceConstants::AppTheme _currentTheme = ThemeServiceConstants::AppTheme::Dark;
};

#endif // THEMESERVICEMANAGER_H
