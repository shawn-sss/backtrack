#ifndef THEMEUTILS_H
#define THEMEUTILS_H

// Project includes
#include "thememanager.h"

// Qt includes
#include <QString>

// Theme preference conversion utilities
UserThemePreference stringToUserThemePreference(const QString& value);
QString userThemePreferenceToString(UserThemePreference pref);

#endif // THEMEUTILS_H
