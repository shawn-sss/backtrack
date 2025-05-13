// Project includes
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

} // namespace

// Returns true if current system theme is dark
bool ThemeServiceManager::isDarkTheme() {
#ifdef Q_OS_WIN
    QSettings settings(kThemeRegistryPath, QSettings::NativeFormat);
    return settings.value(kAppsUseLightThemeKey, 1).toInt() == 0;
#else
    return false;
#endif
}

// Returns cached theme value
AppTheme ThemeServiceManager::currentTheme() {
    return _currentTheme;
}

// Gets stored user theme preference
UserThemePreference ThemeServiceManager::getUserThemePreference() {
    return ServiceDirector::getInstance().getThemePreference();
}

// Sets and applies new user theme preference
void ThemeServiceManager::setUserThemePreference(UserThemePreference preference) {
    ServiceDirector::getInstance().setThemePreference(preference);
    applyTheme();
}

// Applies theme based on user or system setting
void ThemeServiceManager::applyTheme() {
    auto& service = ServiceDirector::getInstance();
    const UserThemePreference preference = service.getThemePreference();

    const AppTheme newTheme = (preference == UserThemePreference::Auto)
                                  ? (isDarkTheme() ? AppTheme::Dark : AppTheme::Light)
                                  : (preference == UserThemePreference::Dark ? AppTheme::Dark : AppTheme::Light);

    _currentTheme = newTheme;

    const QString baseQssPath = QString::fromUtf8(k_BASE_THEME_PATH);
    const QString themeQssPath = (_currentTheme == AppTheme::Dark)
                                     ? QString::fromUtf8(k_DARK_THEME_PATH)
                                     : QString::fromUtf8(k_LIGHT_THEME_PATH);

    qApp->setStyle(QStyleFactory::create("Fusion"));
    qApp->setPalette(qApp->style()->standardPalette());

    QString finalStyle;

    QFile baseFile(baseQssPath);
    if (baseFile.open(QFile::ReadOnly | QFile::Text)) {
        finalStyle += QLatin1String(baseFile.readAll()) + QLatin1Char('\n');
    }

    QFile themeFile(themeQssPath);
    if (themeFile.open(QFile::ReadOnly | QFile::Text)) {
        finalStyle += QLatin1String(themeFile.readAll());
    }

    qApp->setStyleSheet(finalStyle);
}

// Installs Windows-specific theme event listener
void ThemeServiceManager::installEventFilter(QObject*) {
#ifdef Q_OS_WIN
    if (!eventFilter) {
        eventFilter = new ThemeChangeFilter();
        qApp->installNativeEventFilter(eventFilter);
    }
#endif
}
