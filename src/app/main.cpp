#include <QApplication>

#include "ui/mainwindow/mainwindow.h"
#include "core/config/constants.h"

int main(int argc, char *argv[]) {
    // Initialize the application
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName(AppConfig::WINDOW_TITLE);
    app.setApplicationDisplayName(AppConfig::WINDOW_TITLE);

    // Create and configure the main window
    MainWindow mainWindow;
    mainWindow.setWindowTitle(AppConfig::WINDOW_TITLE);
    mainWindow.setWindowIcon(QIcon(BackupInfo::ICON_PATH));
    mainWindow.show();

    // Start the application event loop
    return app.exec();
}
