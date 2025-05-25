// Project includes
#include "../ui/mainwindow/mainwindow.h"
#include "../services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h"
#include "../services/ServiceDirector/ServiceDirector.h"
#include "../../../../constants/app_info.h"
#include "../../../../constants/interface_config.h"

// Qt includes
#include <QApplication>

// Entry point for application startup
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Set app name and display name
    app.setApplicationName(App::Info::k_APP_NAME);
    app.setApplicationDisplayName(App::Info::k_APP_NAME);

    // Initialize core services and theme
    ServiceDirector::getInstance();
    ServiceDirector::getInstance().applyTheme();
    ServiceDirector::getInstance().installThemeEventFilter(&app);

    // Create and show main window
    MainWindow mainWindow;
    mainWindow.setWindowTitle(App::Info::k_APP_NAME);
    mainWindow.setWindowIcon(QIcon(UI::Resources::k_ICON_PATH));
    mainWindow.show();

    // Set pointer cursor for tab widget
    Shared::UI::setTabWidgetCursorToPointer(mainWindow.getDetailsTabWidget());

    // Start event loop
    return app.exec();
}
