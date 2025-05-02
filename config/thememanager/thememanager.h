#pragma once

// Qt includes
#include <QObject>
#include <QStyle>
#include <QApplication>

// Available application themes
enum class AppTheme {
    Light,
    Dark
};

// User's theme preference
enum class UserThemePreference {
    Auto,
    Light,
    Dark
};

// Utilities for theme preferences
QString userThemePreferenceToString(UserThemePreference preference);

// Manages application themes
class ThemeManager {
public:
    // System theme detection
    static bool isDarkTheme();

    // Current theme retrieval
    static AppTheme currentTheme();

    // User theme preference accessors
    static UserThemePreference getUserThemePreference();
    static void setUserThemePreference(UserThemePreference preference);

    // Theme application
    static void applyTheme();

    // System event filter installation
    static void installEventFilter(QObject* target);
};
