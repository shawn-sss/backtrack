// Project includes
#include "../../configsettings/_settings.h"
#include "../../configsettings/resources_settings.h"
#include "../../configsettings/visual_settings.h"
#include "../../configdirector/configdirector.h"
#include "thememanager.h"

// Qt includes
#include <QAbstractNativeEventFilter>
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>
#include <QStyle>
#include <QStyleFactory>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

using namespace Resources::Themes;
using namespace PlatformSettings::Windows;

namespace {

// Internal state for the currently applied theme
AppTheme _currentTheme = AppTheme::Dark;

// Event filter to detect Windows system theme changes
class ThemeChangeFilter : public QAbstractNativeEventFilter {
public:
    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override {
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

} // namespace

// Determines if the OS is currently using dark theme (Windows)
bool ThemeManager::isDarkTheme() {
#ifdef Q_OS_WIN
    QSettings settings(kThemeRegistryPath, QSettings::NativeFormat);
    return settings.value(kAppsUseLightThemeKey, 1).toInt() == 0;
#else
    return false;
#endif
}

// Returns the currently applied theme
AppTheme ThemeManager::currentTheme() {
    return _currentTheme;
}

// Retrieves the user's theme preference from the config
UserThemePreference ThemeManager::getUserThemePreference() {
    return ConfigDirector::getInstance().getThemePreference();
}

// Sets the user's theme preference in config and applies it
void ThemeManager::setUserThemePreference(UserThemePreference preference) {
    ConfigDirector::getInstance().setThemePreference(preference);
    applyTheme();
}

// Applies the current theme or system-detected one
void ThemeManager::applyTheme() {
    UserThemePreference preference = ConfigDirector::getInstance().getThemePreference();
    AppTheme newTheme;

    if (preference == UserThemePreference::Auto) {
        newTheme = isDarkTheme() ? AppTheme::Dark : AppTheme::Light;
    } else {
        newTheme = (preference == UserThemePreference::Dark) ? AppTheme::Dark : AppTheme::Light;
    }

    _currentTheme = newTheme;

    const QString qssPath = (_currentTheme == AppTheme::Dark) ? k_DARK_THEME_PATH : k_LIGHT_THEME_PATH;

    qApp->setStyle(QStyleFactory::create("Fusion"));
    qApp->setPalette(qApp->style()->standardPalette());

    QFile file(qssPath);
    if (file.open(QFile::ReadOnly)) {
        const QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
    }
}

// Installs a native event filter to track system theme changes (Windows only)
void ThemeManager::installEventFilter(QObject* target) {
#ifdef Q_OS_WIN
    if (!eventFilter) {
        eventFilter = new ThemeChangeFilter();
        qApp->installNativeEventFilter(eventFilter);
    }
#endif
}
