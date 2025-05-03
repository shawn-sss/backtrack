// Project includes
#include "../../configsettings/_settings.h"
#include "../../configsettings/visual_settings.h"
#include "../../configsettings/resources_settings.h"
#include "../../configdirector/configdirector.h"
#include "thememanager.h"

// Qt includes
#include <QFile>
#include <QSettings>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QAbstractNativeEventFilter>
#include <QApplication>
#include <QStyle>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

using namespace Resources::Themes;
using namespace PlatformSettings::Windows;

namespace {

// Current theme state (internal)
AppTheme _currentTheme = AppTheme::Dark;

// Native event filter for system theme changes (Windows only)
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

// Detect if system theme is dark (Windows only)
bool ThemeManager::isDarkTheme() {
#ifdef Q_OS_WIN
    QSettings settings(kThemeRegistryPath, QSettings::NativeFormat);
    return settings.value(kAppsUseLightThemeKey, 1).toInt() == 0;
#else
    return false;
#endif
}

// Get the currently applied app theme
AppTheme ThemeManager::currentTheme() {
    return _currentTheme;
}

// Get the user's theme preference from config
UserThemePreference ThemeManager::getUserThemePreference() {
    return ConfigDirector::getInstance().getThemePreference();
}

// Set the user's theme preference in config and apply it
void ThemeManager::setUserThemePreference(UserThemePreference preference) {
    ConfigDirector::getInstance().setThemePreference(preference);
    applyTheme();
}

// Apply the current or auto-detected theme to the application
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

// Install native Windows event filter to monitor system theme changes
void ThemeManager::installEventFilter(QObject* target) {
#ifdef Q_OS_WIN
    if (!eventFilter) {
        eventFilter = new ThemeChangeFilter();
        qApp->installNativeEventFilter(eventFilter);
    }
#endif
}
