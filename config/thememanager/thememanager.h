#pragma once

#include <QObject>

enum class AppTheme {
    Light,
    Dark
};

namespace ThemeManager {
void applyTheme();
bool isDarkTheme();
void installEventFilter(QObject* target);
AppTheme currentTheme();
}
