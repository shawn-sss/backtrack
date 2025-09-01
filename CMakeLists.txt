cmake_minimum_required(VERSION 3.16)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(PROJECT_NAME Backtrack)
set(PROJECT_VERSION 0.5)
set(PROJECT_LANGUAGES CXX)

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES ${PROJECT_LANGUAGES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg)

if(NOT DEFINED QT_VERSION_MAJOR)
    message(FATAL_ERROR "QT_VERSION_MAJOR is not defined. Ensure Qt5 or Qt6 is found properly.")
endif()

# Entry point files
set(SRC_ENTRY_POINT
    app/app_manifest.rc
    app/main.cpp
)

# Qt resource files
set(SRC_RESOURCES
    resources/icons_resource.qrc
    resources/themes_resource.qrc
)

# Shared UI headers
set(UI_SHARED_HEADERS
    ui/systemtraycontextmenu.h
)

# UI: Main window
set(UI_MAIN_WINDOW
    ui/mainwindow/mainwindow.cpp
    ui/mainwindow/mainwindow.h
    ui/mainwindow/mainwindow.ui
    ui/mainwindow/mainwindowlabels.h
    ui/mainwindow/mainwindowmessages.h
    ui/mainwindow/mainwindowstyling.h
    ui/mainwindow/mainwindowconstants.h
)

# UI: Notifications dialog
set(UI_DIALOG_NOTIFICATIONS
    ui/notificationsdialog/notificationsdialog.cpp
    ui/notificationsdialog/notificationsdialog.h
    ui/notificationsdialog/notificationsdialog.ui
    ui/notificationsdialog/notificationsdialogconstants.h
    ui/notificationsdialog/notificationsdialogstyling.h
)

# UI: Settings dialog
set(UI_DIALOG_SETTINGS
    ui/settingsdialog/settingsdialog.cpp
    ui/settingsdialog/settingsdialog.h
    ui/settingsdialog/settingsdialog.ui
    ui/settingsdialog/settingsdialogconstants.h
    ui/settingsdialog/settingsdialogstyling.h
)

# UI: Help dialog
set(UI_DIALOG_HELP
    ui/helpdialog/helpdialog.cpp
    ui/helpdialog/helpdialog.h
    ui/helpdialog/helpdialog.ui
    ui/helpdialog/helpdialogconstants.h
    ui/helpdialog/helpdialogstyling.h
)

# UI: About dialog
set(UI_DIALOG_ABOUT
    ui/aboutdialog/aboutdialog.cpp
    ui/aboutdialog/aboutdialog.h
    ui/aboutdialog/aboutdialog.ui
    ui/aboutdialog/aboutdialogconstants.h
    ui/aboutdialog/aboutdialogstyling.h
)

# UI: Welcome dialog
set(UI_DIALOG_WELCOME
    ui/welcomedialog/welcomedialog.h
    ui/welcomedialog/welcomedialog.cpp
    ui/welcomedialog/welcomedialog.ui
    ui/welcomedialog/welcomedialogconstants.h
    ui/welcomedialog/welcomedialogstyling.h
)

# UI: Template dialog
set(UI_DIALOG_TEMPLATE
    ui/templatedialog/templatedialog.h
    ui/templatedialog/templatedialog.cpp
    ui/templatedialog/templatedialog.ui
    ui/templatedialog/templatedialogconstants.h
    ui/templatedialog/templatedialogstyling.h
)

# UI: Schedule dialog
set(UI_DIALOG_SCHEDULE
    ui/scheduledialog/scheduledialog.h
    ui/scheduledialog/scheduledialog.cpp
    ui/scheduledialog/scheduledialog.ui
    ui/scheduledialog/scheduledialogconstants.h
    ui/scheduledialog/scheduledialogstyling.h
)

# UI: Prompt dialog
set(UI_DIALOG_PROMPT
    ui/promptdialog/promptdialog.h
    ui/promptdialog/promptdialog.cpp
    ui/promptdialog/promptdialog.ui
    ui/promptdialog/promptdialogconstants.h
    ui/promptdialog/promptdialogstyling.h
)

# Combined UI sources
set(SRC_UI
    ${UI_MAIN_WINDOW}
    ${UI_DIALOG_NOTIFICATIONS}
    ${UI_DIALOG_SETTINGS}
    ${UI_DIALOG_HELP}
    ${UI_DIALOG_ABOUT}
    ${UI_DIALOG_WELCOME}
    ${UI_DIALOG_TEMPLATE}
    ${UI_DIALOG_SCHEDULE}
    ${UI_DIALOG_PROMPT}
    ${UI_SHARED_HEADERS}
)

# Core: Backup constants
set(CORE_BACKUP_CONSTANTS
    backup_module/constants/backupconstants.h
)

# Core: Backup controllers
set(CORE_BACKUP_CONTROLLERS
    backup_module/controller/backupcontroller.cpp
    backup_module/controller/backupcontroller.h
)

# Core: Backup services
set(CORE_BACKUP_SERVICES
    backup_module/service/backupservice.cpp
    backup_module/service/backupservice.h
    backup_module/service/fileoperations.cpp
    backup_module/service/fileoperations.h
    backup_module/service/stagingutils.cpp
    backup_module/service/stagingutils.h
)

# Core: Backup workers
set(CORE_BACKUP_WORKERS
    backup_module/worker/transferworker.cpp
    backup_module/worker/transferworker.h
)

# Core: Backup models
set(CORE_BACKUP_MODELS
    backup_module/models/destinationproxymodel.cpp
    backup_module/models/destinationproxymodel.h
    backup_module/models/stagingmodel.cpp
    backup_module/models/stagingmodel.h
)

# Combined core backup sources
set(SRC_CORE_BACKUP
    ${CORE_BACKUP_CONSTANTS}
    ${CORE_BACKUP_CONTROLLERS}
    ${CORE_BACKUP_SERVICES}
    ${CORE_BACKUP_WORKERS}
    ${CORE_BACKUP_MODELS}
)

# Service managers
set(SRC_SERVICE_MANAGERS
    services/ServiceDirector/ServiceDirector.cpp
    services/ServiceDirector/ServiceDirector.h

    services/ServiceManagers/BackupServiceManager/BackupServiceConstants.h
    services/ServiceManagers/BackupServiceManager/BackupServiceManager.cpp
    services/ServiceManagers/BackupServiceManager/BackupServiceManager.h

    services/ServiceManagers/ExportServiceManager/ExportServiceConstants.h
    services/ServiceManagers/ExportServiceManager/ExportServiceManager.cpp
    services/ServiceManagers/ExportServiceManager/ExportServiceManager.h

    services/ServiceManagers/ImportServiceManager/ImportServiceConstants.h
    services/ServiceManagers/ImportServiceManager/ImportServiceManager.cpp
    services/ServiceManagers/ImportServiceManager/ImportServiceManager.h

    services/ServiceManagers/FilewatcherServiceManager/FilewatcherServiceConstants.h
    services/ServiceManagers/FilewatcherServiceManager/FilewatcherServiceManager.cpp
    services/ServiceManagers/FilewatcherServiceManager/FilewatcherServiceManager.h

    services/ServiceManagers/FormatUtilsServiceManager/FormatUtilsServiceManager.cpp
    services/ServiceManagers/FormatUtilsServiceManager/FormatUtilsServiceManager.h

    services/ServiceManagers/InstallServiceManager/InstallServiceConstants.h
    services/ServiceManagers/InstallServiceManager/InstallServiceManager.cpp
    services/ServiceManagers/InstallServiceManager/InstallServiceManager.h

    services/ServiceManagers/JsonServiceManager/JsonServiceManager.cpp
    services/ServiceManagers/JsonServiceManager/JsonServiceManager.h

    services/ServiceManagers/NotificationServiceManager/NotificationServiceConstants.h
    services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.cpp
    services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h

    services/ServiceManagers/PathServiceManager/PathServiceConstants.h
    services/ServiceManagers/PathServiceManager/PathServiceManager.cpp
    services/ServiceManagers/PathServiceManager/PathServiceManager.h

    services/ServiceManagers/TemplateServiceManager/TemplateServiceConstants.h
    services/ServiceManagers/TemplateServiceManager/TemplateServiceManager.cpp
    services/ServiceManagers/TemplateServiceManager/TemplateServiceManager.h

    services/ServiceManagers/ThemeServiceManager/ThemeServiceConstants.h
    services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.cpp
    services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.h

    services/ServiceManagers/ToolbarServiceManager/ToolbarServiceConstants.h
    services/ServiceManagers/ToolbarServiceManager/ToolbarServiceManager.cpp
    services/ServiceManagers/ToolbarServiceManager/ToolbarServiceManager.h
    services/ServiceManagers/ToolbarServiceManager/ToolbarServiceStyling.h

    services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.cpp
    services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h
    services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceConstants.h
    services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceStyling.h

    services/ServiceManagers/UninstallServiceManager/UninstallServiceConstants.h
    services/ServiceManagers/UninstallServiceManager/UninstallServiceManager.cpp
    services/ServiceManagers/UninstallServiceManager/UninstallServiceManager.h

    services/ServiceManagers/UserServiceManager/UserServiceConstants.h
    services/ServiceManagers/UserServiceManager/UserServiceManager.cpp
    services/ServiceManagers/UserServiceManager/UserServiceManager.h

    services/ServiceManagers/ScheduleServiceManager/ScheduleServiceConstants.h
    services/ServiceManagers/ScheduleServiceManager/ScheduleServiceManager.cpp
    services/ServiceManagers/ScheduleServiceManager/ScheduleServiceManager.h
)

# Global constants
set(SRC_CONSTANTS
    constants/app_info.h
    constants/interface_config.h
    constants/system_constants.h
    constants/units_config.h
    constants/window_config.h
)

# Final full project source list
set(PROJECT_SOURCES
    ${SRC_ENTRY_POINT}
    ${SRC_RESOURCES}
    ${SRC_UI}
    ${SRC_CORE_BACKUP}
    ${SRC_SERVICE_MANAGERS}
    ${SRC_CONSTANTS}
    ${UTILS_SHARED}
)

# Target Definition
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
    else()
        add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
    endif()
endif()

# Include directories (AFTER defining the target)
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg)

# macOS-specific properties
if(APPLE)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
    )
endif()

# Install rules
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
