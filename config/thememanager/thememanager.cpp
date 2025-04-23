// Project includes same directory
#include "thememanager.h"

// Built-in Qt includes
#include <QFile>
#include <QSettings>
#include <QStyleFactory>
#include <QAbstractNativeEventFilter>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace {

// Theme stylesheet paths
constexpr auto DARK_THEME_PATH  = ":/resources/styles/dark.qss";
constexpr auto LIGHT_THEME_PATH = ":/resources/styles/light.qss";

// Holds the current application theme state
AppTheme _currentTheme = AppTheme::Dark;

// Native event filter to detect Windows system theme changes
class ThemeChangeFilter : public QAbstractNativeEventFilter {
public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override {
#ifdef Q_OS_WIN
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_SETTINGCHANGE) {
            ThemeManager::applyTheme();
        }
#endif
        return false;
    }
};

ThemeChangeFilter* eventFilter = nullptr;

}

// Checks if the current operating system theme is dark
bool ThemeManager::isDarkTheme() {
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                       QSettings::NativeFormat);
    return settings.value("AppsUseLightTheme", 1).toInt() == 0;
#else
    return false;
#endif
}

// Gets the user's saved theme preference
UserThemePreference ThemeManager::getUserThemePreference() {
    QSettings settings("YourCompany", "YourApp");
    return static_cast<UserThemePreference>(settings.value("UserTheme", static_cast<int>(UserThemePreference::Auto)).toInt());
}

// Sets the user's theme preference
void ThemeManager::setUserThemePreference(UserThemePreference preference) {
    QSettings settings("YourCompany", "YourApp");
    settings.setValue("UserTheme", static_cast<int>(preference));
}

// Applies the appropriate theme based on user preference and system setting
void ThemeManager::applyTheme() {
    UserThemePreference preference = getUserThemePreference();
    AppTheme newTheme;

    if (preference == UserThemePreference::Auto) {
        newTheme = isDarkTheme() ? AppTheme::Dark : AppTheme::Light;
    } else {
        newTheme = (preference == UserThemePreference::Dark) ? AppTheme::Dark : AppTheme::Light;
    }

    _currentTheme = newTheme;

    const QString qssPath = (_currentTheme == AppTheme::Dark) ? DARK_THEME_PATH : LIGHT_THEME_PATH;

    qApp->setStyle(QStyleFactory::create("Fusion"));
    qApp->setPalette(qApp->style()->standardPalette());

    QFile file(qssPath);
    if (file.open(QFile::ReadOnly)) {
        const QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
    }
}

// Installs a native event filter for detecting system theme changes
void ThemeManager::installEventFilter(QObject* target) {
#ifdef Q_OS_WIN
    if (!eventFilter) {
        eventFilter = new ThemeChangeFilter();
        qApp->installNativeEventFilter(eventFilter);
    }
#endif
}

// Returns the currently active application theme
AppTheme ThemeManager::currentTheme() {
    return _currentTheme;
}
