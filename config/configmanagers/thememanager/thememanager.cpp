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

// Returns true if the system is using a dark theme
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

// Retrieves the user's theme preference from configuration
UserThemePreference ThemeManager::getUserThemePreference() {
    return ConfigDirector::getInstance().getThemePreference();
}

// Sets the user's theme preference and applies it
void ThemeManager::setUserThemePreference(UserThemePreference preference) {
    ConfigDirector::getInstance().setThemePreference(preference);
    applyTheme();
}

// Applies the selected or system-detected theme
void ThemeManager::applyTheme() {
    UserThemePreference preference = ConfigDirector::getInstance().getThemePreference();
    AppTheme newTheme;

    if (preference == UserThemePreference::Auto) {
        newTheme = isDarkTheme() ? AppTheme::Dark : AppTheme::Light;
    } else {
        newTheme = (preference == UserThemePreference::Dark) ? AppTheme::Dark : AppTheme::Light;
    }

    _currentTheme = newTheme;

    const QString baseQssPath = k_BASE_THEME_PATH;
    const QString themeQssPath = (_currentTheme == AppTheme::Dark) ? k_DARK_THEME_PATH : k_LIGHT_THEME_PATH;

    qApp->setStyle(QStyleFactory::create("Fusion"));
    qApp->setPalette(qApp->style()->standardPalette());

    QString finalStyle;

    QFile baseFile(baseQssPath);
    if (baseFile.open(QFile::ReadOnly)) {
        finalStyle += QLatin1String(baseFile.readAll()) + "\n";
    }

    QFile themeFile(themeQssPath);
    if (themeFile.open(QFile::ReadOnly)) {
        finalStyle += QLatin1String(themeFile.readAll());
    }

    qApp->setStyleSheet(finalStyle);
}

// Installs an event filter to listen for system theme changes
void ThemeManager::installEventFilter(QObject* target) {
#ifdef Q_OS_WIN
    if (!eventFilter) {
        eventFilter = new ThemeChangeFilter();
        qApp->installNativeEventFilter(eventFilter);
    }
#endif
}
