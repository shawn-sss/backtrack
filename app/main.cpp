// Project includes
#include "../config/_constants.h"
#include "../config/configmanager/configmanager.h"
#include "../config/thememanager/thememanager.h"
#include "../ui/mainwindow/mainwindow.h"

// Qt includes
#include <QApplication>

// Entry point for application
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Load configuration
    ConfigManager::getInstance();

    // Set application properties
    app.setApplicationName(AppInfo::k_APP_NAME);
    app.setApplicationDisplayName(AppInfo::k_APP_NAME);

    // Apply theme
    ThemeManager::applyTheme();
    ThemeManager::installEventFilter(&app);

    // Initialize main window
    MainWindow mainWindow;
    mainWindow.setWindowTitle(AppInfo::k_APP_NAME);
    mainWindow.setWindowIcon(QIcon(Resources::Application::k_ICON_PATH));
    mainWindow.show();

    return app.exec();
}
