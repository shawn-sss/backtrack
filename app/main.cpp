// Project includes
#include "../config/configsettings/_settings.h"
#include "../config/configdirector/configdirector.h"
#include "../config/configmanagers/thememanager/thememanager.h"
#include "../ui/mainwindow/mainwindow.h"

// Qt includes
#include <QApplication>

// Entry point for application startup
int main(int argc, char* argv[]) {
    // Initialize the Qt application
    QApplication app(argc, argv);

    // Load application configuration
    ConfigDirector::getInstance();

    // Set application metadata
    app.setApplicationName(AppInfo::k_APP_NAME);
    app.setApplicationDisplayName(AppInfo::k_APP_NAME);

    // Apply visual theme and set up event filter for system theme changes
    ThemeManager::applyTheme();
    ThemeManager::installEventFilter(&app);

    // Create and show the main window
    MainWindow mainWindow;
    mainWindow.setWindowTitle(AppInfo::k_APP_NAME);
    mainWindow.setWindowIcon(QIcon(Resources::Application::k_ICON_PATH));
    mainWindow.show();

    return app.exec();
}
