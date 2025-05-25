// Project includes
#include "ThemeServiceManager.h"
#include "../../servicedirector/servicedirector.h"
#include "ThemeServiceConstants.h"

// Qt includes
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QStyleFactory>
#include <QStyle>

// OS includes
#ifdef Q_OS_WIN
#include <QAbstractNativeEventFilter>
#include <windows.h>
#endif

using namespace ThemeServiceConstants;
using namespace ThemeConstants;

#ifdef Q_OS_WIN
using namespace WindowsThemeRegistry;

// Listens for system theme change messages
class ThemeChangeFilter : public QAbstractNativeEventFilter {
public:
    bool nativeEventFilter(const QByteArray&, void* message, qintptr*) override {
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_SETTINGCHANGE) {
            ThemeServiceManager::instance().applyTheme();
        }
        return false;
    }
};

ThemeChangeFilter* ThemeServiceManager::eventFilter = nullptr;
#endif

// Loads QSS file content
static QString loadQssFile(const QString& path) {
    QFile file(path);
    return (file.open(QFile::ReadOnly | QFile::Text)) ? QString::fromUtf8(file.readAll()) : QString();
}

// Returns the singleton instance
ThemeServiceManager& ThemeServiceManager::instance() {
    static ThemeServiceManager _instance;
    return _instance;
}

// Constructor
ThemeServiceManager::ThemeServiceManager(QObject* parent)
    : QObject(parent),
    _currentTheme(AppTheme::Dark) {}

// Returns the current app theme
AppTheme ThemeServiceManager::currentTheme() const {
    return _currentTheme;
}

// Gets saved user theme preference
UserThemePreference ThemeServiceManager::getUserThemePreference() const {
    return ServiceDirector::getInstance().getThemePreference();
}

// Determines if the system is in dark mode
bool ThemeServiceManager::isDarkTheme() const {
#ifdef Q_OS_WIN
    QSettings settings(kThemeRegistryPath, QSettings::NativeFormat);
    return settings.value(kAppsUseLightThemeKey, 1).toInt() == 0;
#else
    return false;
#endif
}

// Sets user theme preference and reapplies theme
void ThemeServiceManager::setUserThemePreference(UserThemePreference preference) {
    ServiceDirector::getInstance().setThemePreference(preference);
    applyTheme();
}

// Applies the appropriate theme
void ThemeServiceManager::applyTheme() {
    const UserThemePreference preference = getUserThemePreference();

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

    emit themeChanged();
}

// Installs system theme change listener (Windows only)
void ThemeServiceManager::installEventFilter(QObject*) {
#ifdef Q_OS_WIN
    if (!eventFilter) {
        eventFilter = new ThemeChangeFilter();
        qApp->installNativeEventFilter(eventFilter);
    }
#endif
}
