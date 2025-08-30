// Project includes
#include "ThemeServiceManager.h"
#include "ThemeServiceConstants.h"
#include "../../servicedirector/servicedirector.h"

// Qt includes
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QStyleFactory>
#include <QStyle>

#ifdef Q_OS_WIN
#include <QAbstractNativeEventFilter>
#include <windows.h>
#endif

using namespace ThemeServiceConstants;
using namespace ThemePaths;
using namespace StyleNames;

#ifdef Q_OS_WIN
using namespace WindowsThemeRegistry;

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

// Internal: load QSS file content
static QString loadQssFile(const QString& path) {
    QFile file(path);
    return (file.open(QFile::ReadOnly | QFile::Text)) ? QString::fromUtf8(file.readAll()) : QString();
}

// Lifecycle: singleton
ThemeServiceManager& ThemeServiceManager::instance() {
    static ThemeServiceManager _instance;
    return _instance;
}

// Lifecycle: constructor
ThemeServiceManager::ThemeServiceManager(QObject* parent)
    : QObject(parent) {}

// Query: current app theme
AppTheme ThemeServiceManager::currentTheme() const {
    return _currentTheme;
}

// Query: saved user theme preference
UserThemePreference ThemeServiceManager::getUserThemePreference() const {
    return ServiceDirector::getInstance().getThemePreference();
}

// Query: system dark mode state
bool ThemeServiceManager::isDarkTheme() const {
#ifdef Q_OS_WIN
    QSettings settings(QString::fromLatin1(ThemeRegistryPath), QSettings::NativeFormat);
    return settings.value(QString::fromLatin1(AppsUseLightThemeKey), 1).toInt() == 0;
#else
    return false;
#endif
}

// Mutation: set user preference and apply
void ThemeServiceManager::setUserThemePreference(UserThemePreference preference) {
    ServiceDirector::getInstance().setThemePreference(preference);
    applyTheme();
}

// Mutation: apply theme
void ThemeServiceManager::applyTheme() {
    const UserThemePreference preference = getUserThemePreference();

    _currentTheme = (preference == UserThemePreference::Auto)
                        ? (isDarkTheme() ? AppTheme::Dark : AppTheme::Light)
                        : (preference == UserThemePreference::Dark ? AppTheme::Dark : AppTheme::Light);

    qApp->setStyle(QStyleFactory::create(QString::fromLatin1(Fusion)));
    qApp->setPalette(qApp->style()->standardPalette());

    const QString finalStyle = loadQssFile(QString::fromLatin1(Base)) + QLatin1Char('\n') +
                               loadQssFile((_currentTheme == AppTheme::Dark)
                                               ? QString::fromLatin1(Dark)
                                               : QString::fromLatin1(Light));

    qApp->setStyleSheet(finalStyle);

    emit themeChanged();
}

// Mutation: install event filter (Windows only)
void ThemeServiceManager::installEventFilter(QObject* target) {
    (void)target;
#ifdef Q_OS_WIN
    if (!eventFilter) {
        eventFilter = new ThemeChangeFilter();
        qApp->installNativeEventFilter(eventFilter);
    }
#endif
}
