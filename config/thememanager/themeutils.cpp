// Project includes
#include "themeutils.h"

// Converts a string to the corresponding UserThemePreference
UserThemePreference stringToUserThemePreference(const QString& value) {
    if (value == "light") return UserThemePreference::Light;
    if (value == "dark")  return UserThemePreference::Dark;
    return UserThemePreference::Auto;
}

// Converts a UserThemePreference to the corresponding string
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
