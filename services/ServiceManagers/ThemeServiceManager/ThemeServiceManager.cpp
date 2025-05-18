#include "ThemeServiceManager.h"
#include "../../servicedirector/servicedirector.h"
#include "ThemeServiceConstants.h"

// Qt includes
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QStyle>
#include <QStyleFactory>
#include <QAbstractNativeEventFilter>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

using namespace ThemeServiceConstants;
using namespace ThemeConstants;

#ifdef Q_OS_WIN
using namespace WindowsThemeRegistry;
#endif

namespace {

// Holds current theme state
AppTheme _currentTheme = AppTheme::Dark;

#ifdef Q_OS_WIN
// Listens for system theme changes on Windows
class ThemeChangeFilter : public QAbstractNativeEventFilter {
public:
    bool nativeEventFilter(const QByteArray&, void* message, qintptr*) override {
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_SETTINGCHANGE) {
            ThemeServiceManager::applyTheme();
        }
        return false;
    }
};

ThemeChangeFilter* eventFilter = nullptr;
#endif

// Loads a QSS file and returns contents as QString
QString loadQssFile(const QString& path) {
    QFile file(path);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        return QString::fromUtf8(file.readAll());
    }
    return {};
}

} // namespace

// Returns true if the system is using a dark theme
bool ThemeServiceManager::isDarkTheme() {
#ifdef Q_OS_WIN
    QSettings settings(kThemeRegistryPath, QSettings::NativeFormat);
    return settings.value(kAppsUseLightThemeKey, 1).toInt() == 0;
#else
    return false;
#endif
}

// Returns the currently applied app theme
AppTheme ThemeServiceManager::currentTheme() {
    return _currentTheme;
}

// Retrieves the user's theme preference from settings
UserThemePreference ThemeServiceManager::getUserThemePreference() {
    return ServiceDirector::getInstance().getThemePreference();
}

// Sets the user's theme preference and applies the theme
void ThemeServiceManager::setUserThemePreference(UserThemePreference preference) {
    ServiceDirector::getInstance().setThemePreference(preference);
    applyTheme();
}

// Applies the correct theme stylesheet based on system and user preference
void ThemeServiceManager::applyTheme() {
    auto& service = ServiceDirector::getInstance();
    const UserThemePreference preference = service.getThemePreference();

    _currentTheme = (preference == UserThemePreference::Auto)
                        ? (isDarkTheme() ? AppTheme::Dark : AppTheme::Light)
                        : (preference == UserThemePreference::Dark ? AppTheme::Dark : AppTheme::Light);

    qApp->setStyle(QStyleFactory::create("Fusion"));
    qApp->setPalette(qApp->style()->standardPalette());

    const QString finalStyle = loadQssFile(k_BASE_THEME_PATH) + "\n" +
                               loadQssFile((_currentTheme == AppTheme::Dark)
                                               ? k_DARK_THEME_PATH
                                               : k_LIGHT_THEME_PATH);

    qApp->setStyleSheet(finalStyle);
}

// Installs a Windows-specific event filter for system theme changes
void ThemeServiceManager::installEventFilter(QObject*) {
#ifdef Q_OS_WIN
    if (!eventFilter) {
        eventFilter = new ThemeChangeFilter();
        qApp->installNativeEventFilter(eventFilter);
    }
#endif
}
