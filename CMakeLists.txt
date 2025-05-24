cmake_minimum_required(VERSION 3.16)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(PROJECT_NAME MyDataBackupApp)
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

# Entry Point
set(SRC_ENTRY_POINT
    app/app_manifest.rc
    app/main.cpp
)

# Qt Resource Files
set(SRC_RESOURCES
    resources/icons_resource.qrc
    resources/themes_resource.qrc
)

# UI: Main Window
set(UI_MAIN_WINDOW
    ui/mainwindow/mainwindow.cpp
    ui/mainwindow/mainwindow.h
    ui/mainwindow/mainwindow.ui
    ui/mainwindow/mainwindowlabels.h
    ui/mainwindow/mainwindowmessages.h
    ui/mainwindow/mainwindowstyling.h
)

# UI: Dialogs - Notifications
set(UI_DIALOG_NOTIFICATIONS
    ui/notificationsdialog/notificationsdialog.cpp
    ui/notificationsdialog/notificationsdialog.h
    ui/notificationsdialog/notificationsdialog.ui
    ui/notificationsdialog/notificationsdialogconstants.h
    ui/notificationsdialog/notificationsdialogstyling.h
)

# UI: Dialogs - Settings
set(UI_DIALOG_SETTINGS
    ui/settingsdialog/settingsdialog.cpp
    ui/settingsdialog/settingsdialog.h
    ui/settingsdialog/settingsdialog.ui
    ui/settingsdialog/settingsdialogconstants.h
    ui/settingsdialog/settingsdialogstyling.h
)

# UI: Dialogs - Help
set(UI_DIALOG_HELP
    ui/helpdialog/helpdialog.cpp
    ui/helpdialog/helpdialog.h
    ui/helpdialog/helpdialog.ui
    ui/helpdialog/helpdialogconstants.h
    ui/helpdialog/helpdialogstyling.h
)

# UI: Dialogs - About
set(UI_DIALOG_ABOUT
    ui/aboutdialog/aboutdialog.cpp
    ui/aboutdialog/aboutdialog.h
    ui/aboutdialog/aboutdialog.ui
    ui/aboutdialog/aboutdialogconstants.h
    ui/aboutdialog/aboutdialogstyling.h
)

# All UI Sources
set(SRC_UI
    ${UI_MAIN_WINDOW}
    ${UI_DIALOG_NOTIFICATIONS}
    ${UI_DIALOG_SETTINGS}
    ${UI_DIALOG_HELP}
    ${UI_DIALOG_ABOUT}
)

# Core: Backup Controllers
set(CORE_BACKUP_CONTROLLERS
    core/backup/controller/backupcontroller.cpp
    core/backup/controller/backupcontroller.h
)

# Core: Backup Services
set(CORE_BACKUP_SERVICES
    core/backup/service/backupservice.cpp
    core/backup/service/backupservice.h
    core/backup/service/fileoperations.cpp
    core/backup/service/fileoperations.h
    core/backup/service/stagingutils.cpp
    core/backup/service/stagingutils.h
)

# Core: Backup Workers
set(CORE_BACKUP_WORKERS
    core/backup/worker/transferworker.cpp
    core/backup/worker/transferworker.h
)

# Core: Backup Models
set(CORE_BACKUP_MODELS
    core/backup/models/destinationproxymodel.cpp
    core/backup/models/destinationproxymodel.h
    core/backup/models/stagingmodel.cpp
    core/backup/models/stagingmodel.h
)

# Core: All Backup Components
set(SRC_CORE_BACKUP
    ${CORE_BACKUP_CONTROLLERS}
    ${CORE_BACKUP_SERVICES}
    ${CORE_BACKUP_WORKERS}
    ${CORE_BACKUP_MODELS}
)

# Service Managers
set(SRC_SERVICE_MANAGERS
    services/ServiceDirector/ServiceDirector.cpp
    services/ServiceDirector/ServiceDirector.h
    services/ServiceDirector/ServiceDirectorConstants.h

    services/ServiceManagers/BackupServiceManager/BackupServiceConstants.h
    services/ServiceManagers/BackupServiceManager/BackupServiceManager.cpp
    services/ServiceManagers/BackupServiceManager/BackupServiceManager.h

    services/ServiceManagers/FilewatcherServiceManager/FilewatcherServiceConstants.h
    services/ServiceManagers/FilewatcherServiceManager/FilewatcherServiceManager.cpp
    services/ServiceManagers/FilewatcherServiceManager/FilewatcherServiceManager.h

    services/ServiceManagers/FormatUtilsServiceManager/FormatUtilsServiceManager.cpp
    services/ServiceManagers/FormatUtilsServiceManager/FormatUtilsServiceManager.h

    services/ServiceManagers/InstallServiceManager/InstallServiceConstants.h
    services/ServiceManagers/InstallServiceManager/InstallServiceManager.cpp
    services/ServiceManagers/InstallServiceManager/InstallServiceManager.h

    services/ServiceManagers/JsonServiceManager/JsonServiceConstants.h
    services/ServiceManagers/JsonServiceManager/JsonServiceManager.cpp
    services/ServiceManagers/JsonServiceManager/JsonServiceManager.h

    services/ServiceManagers/NotificationServiceManager/NotificationServiceConstants.h
    services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.cpp
    services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h
    services/ServiceManagers/NotificationServiceManager/NotificationServiceStruct.h

    services/ServiceManagers/PathServiceManager/PathServiceConstants.h
    services/ServiceManagers/PathServiceManager/PathServiceManager.cpp
    services/ServiceManagers/PathServiceManager/PathServiceManager.h

    services/ServiceManagers/ThemeServiceManager/ThemeServiceConstants.h
    services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.cpp
    services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.h

    services/ServiceManagers/ToolbarServiceManager/ToolbarServiceConstants.h
    services/ServiceManagers/ToolbarServiceManager/ToolbarServiceManager.cpp
    services/ServiceManagers/ToolbarServiceManager/ToolbarServiceManager.h
    services/ServiceManagers/ToolbarServiceManager/ToolbarServiceStyling.h

    services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.cpp
    services/ServiceManagers/UIUtilsServiceManager/UIUtilsServiceManager.h

    services/ServiceManagers/UninstallServiceManager/UninstallServiceConstants.h
    services/ServiceManagers/UninstallServiceManager/UninstallServiceManager.cpp
    services/ServiceManagers/UninstallServiceManager/UninstallServiceManager.h

    services/ServiceManagers/UserServiceManager/UserServiceConstants.h
    services/ServiceManagers/UserServiceManager/UserServiceManager.cpp
    services/ServiceManagers/UserServiceManager/UserServiceManager.h
)

# Constants
set(SRC_CONSTANTS
    constants/app_info.h
    constants/backup_config.h
    constants/interface_config.h
    constants/kvp_info.h
    constants/system_constants.h
    constants/units_config.h
    constants/window_config.h
)

# Full Project Sources
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
