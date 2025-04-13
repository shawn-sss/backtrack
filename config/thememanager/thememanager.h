#pragma once

#include <QObject>

// Application theme options
enum class AppTheme {
    Light,
    Dark
};

namespace ThemeManager {

// Applies the appropriate theme stylesheet to the app
void applyTheme();

// Returns true if the system theme is currently dark
bool isDarkTheme();

// Installs a native event filter to detect theme changes
void installEventFilter(QObject* target);

// Returns the currently active theme
AppTheme currentTheme();
}
