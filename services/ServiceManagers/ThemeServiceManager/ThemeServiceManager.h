#ifndef THEMESERVICEMANAGER_H
#define THEMESERVICEMANAGER_H

// Project includes
#include "ThemeServiceConstants.h"

// Qt includes
#include <QObject>

// Forward declaration (Qt class)
#ifdef Q_OS_WIN
#include <QAbstractNativeEventFilter>
class ThemeChangeFilter;
#endif

class ThemeServiceManager : public QObject {
    Q_OBJECT

public:
    // Singleton access
    static ThemeServiceManager& instance();

    // Theme status
    bool isDarkTheme() const;
    ThemeServiceConstants::AppTheme currentTheme() const;
    ThemeServiceConstants::UserThemePreference getUserThemePreference() const;

    // Theme control
    void setUserThemePreference(ThemeServiceConstants::UserThemePreference preference);
    void applyTheme();
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
