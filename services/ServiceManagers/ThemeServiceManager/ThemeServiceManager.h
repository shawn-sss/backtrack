#ifndef THEMESERVICEMANAGER_H
#define THEMESERVICEMANAGER_H

// Project includes
#include "ThemeServiceConstants.h"

// Qt includes
#include <QObject>

#ifdef Q_OS_WIN
class ThemeChangeFilter;
#endif

// Manages application theming
class ThemeServiceManager : public QObject {
    Q_OBJECT

public:
    // Lifecycle
    static ThemeServiceManager& instance();

    // Query
    bool isDarkTheme() const;
    ThemeServiceConstants::AppTheme currentTheme() const;
    ThemeServiceConstants::UserThemePreference getUserThemePreference() const;

    // Mutation
    void setUserThemePreference(ThemeServiceConstants::UserThemePreference preference);
    void applyTheme();
    void installEventFilter(QObject* target = nullptr);

signals:
    void themeChanged();

private:
    explicit ThemeServiceManager(QObject* parent = nullptr);

#ifdef Q_OS_WIN
    static ThemeChangeFilter* eventFilter;
#endif

    ThemeServiceConstants::AppTheme _currentTheme = ThemeServiceConstants::AppTheme::Dark;
};

#endif
