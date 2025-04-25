#ifndef THEMEUTILS_H
#define THEMEUTILS_H

// Project includes
#include "thememanager.h"

// Qt includes
#include <QString>

// Converts a string to UserThemePreference
UserThemePreference stringToUserThemePreference(const QString& value);

// Converts UserThemePreference to a string
QString userThemePreferenceToString(UserThemePreference pref);

#endif // THEMEUTILS_H
