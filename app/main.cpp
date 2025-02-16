#include <QApplication>

#include "../ui/mainwindow/mainwindow.h"
#include "../core/config/_constants.h"

int main(int argc, char *argv[]) {
    // Initialize the application
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName(AppInfo::APP_DISPLAY_TITLE);
    app.setApplicationDisplayName(AppInfo::APP_DISPLAY_TITLE);

    // Create and configure the main window
    MainWindow mainWindow;
    mainWindow.setWindowTitle(AppInfo::APP_DISPLAY_TITLE);
    mainWindow.setWindowIcon(QIcon(BackupResources::ICON_PATH));
    mainWindow.show();

    // Start the application event loop
    return app.exec();
}
