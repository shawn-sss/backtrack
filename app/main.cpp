#include <QApplication>
#include "../config/_constants.h"
#include "../config/configmanager/configmanager.h"
#include "../ui/mainwindow/mainwindow.h"

int main(int argc, char *argv[]) {
    // Initialize application
    QApplication app(argc, argv);

    // Load configuration
    ConfigManager::getInstance();

    // Set application properties
    app.setApplicationName(AppInfo::APP_DISPLAY_TITLE);
    app.setApplicationDisplayName(Resources::Application::ICON_PATH);

    // Initialize main window
    MainWindow mainWindow;
    mainWindow.setWindowTitle(AppInfo::APP_DISPLAY_TITLE);
    mainWindow.setWindowIcon(QIcon(Resources::Application::ICON_PATH));
    mainWindow.show();

    // Start event loop
    return app.exec();
}
