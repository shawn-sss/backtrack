// Project includes different directory
#include "../config/_constants.h"
#include "../ui/mainwindow/mainwindow.h"
#include "../config/configmanager/configmanager.h"
#include "../config/thememanager/thememanager.h"

// Built-in Qt includes
#include <QApplication>

// Entry point for application
int main(int argc, char *argv[]) {

    // Initialize application
    QApplication app(argc, argv);

    // Load configuration
    ConfigManager::getInstance();

    // Set application properties
    app.setApplicationName(AppInfo::k_APP_DISPLAY_TITLE);
    app.setApplicationDisplayName(AppInfo::k_APP_DISPLAY_TITLE);

    // Initialize main window
    MainWindow mainWindow;
    mainWindow.setWindowTitle(AppInfo::k_APP_DISPLAY_TITLE);
    mainWindow.setWindowIcon(QIcon(Resources::Application::ICON_PATH));
    ThemeManager::applyTheme();
    ThemeManager::installEventFilter(&app);
    mainWindow.show();

    // Start event loop
    return app.exec();
}
