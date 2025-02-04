#include "UIComponents/mainwindow.h"
#include <QApplication>
#include "Utils/constants.h"

int main(int argc, char *argv[]) {
    // Initialize the application
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName(Constants::WINDOW_TITLE);
    app.setApplicationDisplayName(Constants::WINDOW_TITLE);

    // Create and configure the main window
    MainWindow mainWindow;
    mainWindow.setWindowTitle(Constants::WINDOW_TITLE);
    mainWindow.setWindowIcon(QIcon(Constants::ICON_PATH));
    mainWindow.show();

    // Start the application event loop
    return app.exec();
}
