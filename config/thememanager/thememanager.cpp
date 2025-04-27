// Project includes
#include "../_constants.h"
#include "../../core/utils/file_utils/jsonmanager.h"
#include "../configmanager/configmanager.h"
#include "../thememanager/themeutils.h"
#include "thememanager.h"

// Qt includes
#include <QFile>
#include <QSettings>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QAbstractNativeEventFilter>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace {

// Theme stylesheet paths
constexpr auto DARK_THEME_PATH  = ":/resources/styles/dark.qss";
constexpr auto LIGHT_THEME_PATH = ":/resources/styles/light.qss";

// Current theme state
AppTheme _currentTheme = AppTheme::Dark;

// Native event filter for system theme changes
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

// System theme detection
bool ThemeManager::isDarkTheme() {
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                       QSettings::NativeFormat);
    return settings.value("AppsUseLightTheme", 1).toInt() == 0;
#else
    return false;
#endif
}

// Current theme retrieval
AppTheme ThemeManager::currentTheme() {
    return _currentTheme;
}

// User theme preference accessors
UserThemePreference ThemeManager::getUserThemePreference() {
    return ConfigManager::getInstance().getThemePreference();
}

void ThemeManager::setUserThemePreference(UserThemePreference preference) {
    ConfigManager::getInstance().setThemePreference(preference);
    applyTheme();
}

// Theme application
void ThemeManager::applyTheme() {
    UserThemePreference preference = ConfigManager::getInstance().getThemePreference();
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

// System event filter installation
void ThemeManager::installEventFilter(QObject* target) {
#ifdef Q_OS_WIN
    if (!eventFilter) {
        eventFilter = new ThemeChangeFilter();
        qApp->installNativeEventFilter(eventFilter);
    }
#endif
}
