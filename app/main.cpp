// Project includes
#include "../core/shared/uiutils.h"
#include "../services/ServiceDirector/ServiceDirector.h"
#include "../services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.h"
#include "../ui/mainwindow/mainwindow.h"
#include "../../../../constants/app_info.h"
#include "../../../../constants/interface_config.h"

// Qt includes
#include <QApplication>

// Entry point for application startup
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Initialize global service layer
    ServiceDirector::getInstance();

    // Set application metadata
    app.setApplicationName(App::Info::k_NAME);
    app.setApplicationDisplayName(App::Info::k_NAME);

    // Apply theme and install global event filter
    ThemeServiceManager::applyTheme();
    ThemeServiceManager::installEventFilter(&app);

    // Initialize and display main window
    MainWindow mainWindow;
    mainWindow.setWindowTitle(App::Info::k_NAME);
    mainWindow.setWindowIcon(QIcon(UI::Resources::k_ICON_PATH));
    mainWindow.show();

    // Set tab cursor style
    Shared::UI::setTabWidgetCursorToPointer(mainWindow.getDetailsTabWidget());

    // Run application event loop
    return app.exec();
}
