// Project includes
#include "../config/configdirector/configdirector.h"
#include "../config/configsettings/app_settings.h"
#include "../config/ConfigManagers/ThemeConfigManager/ThemeConfigManager.h"
#include "../ui/mainwindow/mainwindow.h"
#include "../core/utils/common_utils/utils.h"

// Qt includes
#include <QApplication>

// Entry point for application startup
int main(int argc, char* argv[]) {
    // Init Qt application
    QApplication app(argc, argv);

    // Load global config
    ConfigDirector::getInstance();

    // Set basic app metadata
    app.setApplicationName(AppInfo::k_APP_NAME);
    app.setApplicationDisplayName(AppInfo::k_APP_NAME);

    // Apply and monitor visual theme
    ThemeConfigManager::applyTheme();
    ThemeConfigManager::installEventFilter(&app);

    // Set up and show main window
    MainWindow mainWindow;
    mainWindow.setWindowTitle(AppInfo::k_APP_NAME);
    mainWindow.setWindowIcon(QIcon(Resources::Application::k_ICON_PATH));
    mainWindow.show();


    Utils::UI::setTabWidgetCursorToPointer(mainWindow.getDetailsTabWidget());

    // Start event loop
    return app.exec();
}
