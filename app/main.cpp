// Project includes
#include "../config/configsettings/_settings.h"
#include "../config/configmanagers/thememanager/thememanager.h"
#include "../config/configdirector/configdirector.h"
#include "../ui/mainwindow/mainwindow.h"

// Qt includes
#include <QApplication>

// Entry point for application
int main(int argc, char *argv[]) {
    // Set up the Qt application
    QApplication app(argc, argv);

    // Initialize and load core configuration
    ConfigDirector::getInstance();

    // Configure application metadata
    app.setApplicationName(AppInfo::k_APP_NAME);
    app.setApplicationDisplayName(AppInfo::k_APP_NAME);

    // Apply and initialize theme
    ThemeManager::applyTheme();
    ThemeManager::installEventFilter(&app);

    // Create and show the main application window
    MainWindow mainWindow;
    mainWindow.setWindowTitle(AppInfo::k_APP_NAME);
    mainWindow.setWindowIcon(QIcon(Resources::Application::k_ICON_PATH));
    mainWindow.show();

    return app.exec();
}
