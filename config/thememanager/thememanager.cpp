// Project includes (same directory)
#include "thememanager.h"
//#include "../../ui/toolbarmanager/toolbarmanager.h"

// Qt includes
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QAbstractNativeEventFilter>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

// Internal constants and theme state
namespace {
constexpr auto DARK_THEME_PATH  = ":/resources/styles/dark.qss";
constexpr auto LIGHT_THEME_PATH = ":/resources/styles/light.qss";

AppTheme _currentTheme = AppTheme::Light;

class ThemeChangeFilter : public QAbstractNativeEventFilter {
public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override {
#ifdef Q_OS_WIN
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_SETTINGCHANGE) {
            ThemeManager::applyTheme();  // Will only reapply if theme changed
        }
#endif
        return false;
    }
};

ThemeChangeFilter* eventFilter = nullptr;
}

bool ThemeManager::isDarkTheme() {
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                       QSettings::NativeFormat);
    return settings.value("AppsUseLightTheme", 1).toInt() == 0;
#else
    return false;
#endif
}

void ThemeManager::applyTheme() {
    AppTheme newTheme = isDarkTheme() ? AppTheme::Dark : AppTheme::Light;
    _currentTheme = newTheme;

    const QString qssPath = (_currentTheme == AppTheme::Dark)
                                ? DARK_THEME_PATH
                                : LIGHT_THEME_PATH;

    QFile file(qssPath);
    if (file.open(QFile::ReadOnly)) {
        const QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
    }
}


void ThemeManager::installEventFilter(QObject* target) {
#ifdef Q_OS_WIN
    if (!eventFilter) {
        eventFilter = new ThemeChangeFilter();
        qApp->installNativeEventFilter(eventFilter);
    }
#endif
}

AppTheme ThemeManager::currentTheme() {
    return _currentTheme;
}
