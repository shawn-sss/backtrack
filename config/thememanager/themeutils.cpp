// Project includes
#include "themeutils.h"

// Theme preference conversion utilities
UserThemePreference stringToUserThemePreference(const QString& value) {
    if (value == "light") return UserThemePreference::Light;
    if (value == "dark")  return UserThemePreference::Dark;
    return UserThemePreference::Auto;
}

QString userThemePreferenceToString(UserThemePreference pref) {
    switch (pref) {
    case UserThemePreference::Light:
        return "light";
    case UserThemePreference::Dark:
        return "dark";
    case UserThemePreference::Auto:
    default:
        return "auto";
    }
}
