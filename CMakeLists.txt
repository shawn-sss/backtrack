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
set(APP_ENTRY
    app/app_manifest.rc
    app/main.cpp
)

# Qt Resource Files
set(RESOURCES
    resources/icons_resource.qrc
    resources/themes_resource.qrc
)

# UI Components
set(UI_MAIN_WINDOW
    ui/mainwindow/mainwindow.cpp
    ui/mainwindow/mainwindow.h
    ui/mainwindow/mainwindow.ui
    ui/mainwindow/mainwindowlabels.h
    ui/mainwindow/mainwindowmessages.h
    ui/mainwindow/mainwindowstyling.h
)

set(UI_DIALOG_NOTIFICATIONS
    ui/notificationsdialog/notificationsdialog.cpp
    ui/notificationsdialog/notificationsdialog.h
    ui/notificationsdialog/notificationsdialog.ui
    ui/notificationsdialog/notificationsdialogconstants.h
    ui/notificationsdialog/notificationsdialogstyling.h
)

set(UI_DIALOG_SETTINGS
    ui/settingsdialog/settingsdialog.cpp
    ui/settingsdialog/settingsdialog.h
    ui/settingsdialog/settingsdialog.ui
    ui/settingsdialog/settingsdialogconstants.h
    ui/settingsdialog/settingsdialogstyling.h
    ui/helpdialog/helpdialog.h
    ui/helpdialog/helpdialog.cpp
    ui/helpdialog/helpdialog.ui
    ui/helpdialog/helpdialogconstants.h
    ui/helpdialog/helpdialogstyling.h
    ui/aboutdialog/aboutdialog.h
    ui/aboutdialog/aboutdialog.cpp
    ui/aboutdialog/aboutdialog.ui
    ui/aboutdialog/aboutdialogconstants.h
    ui/aboutdialog/aboutdialogstyling.h
)

set(UI_ALL
    ${UI_MAIN_WINDOW}
    ${UI_DIALOG_NOTIFICATIONS}
    ${UI_DIALOG_SETTINGS}
)

# Backup Core Modules
set(CORE_BACKUP_CONTROLLERS
    core/backup/controller/backupcontroller.cpp
    core/backup/controller/backupcontroller.h
)

set(CORE_BACKUP_SERVICES
    core/backup/service/backupservice.cpp
    core/backup/service/backupservice.h
)

set(CORE_BACKUP_WORKERS
    core/backup/worker/transferworker.cpp
    core/backup/worker/transferworker.h
)

set(CORE_BACKUP_MODELS
    core/backup/models/destinationproxymodel.cpp
    core/backup/models/destinationproxymodel.h
    core/backup/models/stagingmodel.cpp
    core/backup/models/stagingmodel.h
)

set(CORE_BACKUP
    ${CORE_BACKUP_CONTROLLERS}
    ${CORE_BACKUP_SERVICES}
    ${CORE_BACKUP_WORKERS}
    ${CORE_BACKUP_MODELS}
)

# Service Managers
set(SERVICE_MANAGER_DIRECTOR
    services/ServiceDirector/ServiceDirector.cpp
    services/ServiceDirector/ServiceDirector.h
    services/ServiceDirector/ServiceDirectorConstants.h
)

set(SERVICE_MANAGER_THEME
    services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.cpp
    services/ServiceManagers/ThemeServiceManager/ThemeServiceManager.h
    services/ServiceManagers/ThemeServiceManager/ThemeServiceConstants.h
)

set(SERVICE_MANAGER_INSTALL
    services/ServiceManagers/InstallServiceManager/InstallServiceManager.cpp
    services/ServiceManagers/InstallServiceManager/InstallServiceManager.h
    services/ServiceManagers/InstallServiceManager/InstallServiceConstants.h
)

set(SERVICE_MANAGER_BACKUP
    services/ServiceManagers/BackupServiceManager/BackupServiceManager.cpp
    services/ServiceManagers/BackupServiceManager/BackupServiceManager.h
    services/ServiceManagers/BackupServiceManager/BackupServiceConstants.h
)

set(SERVICE_MANAGER_USER
    services/ServiceManagers/UserServiceManager/UserServiceManager.cpp
    services/ServiceManagers/UserServiceManager/UserServiceManager.h
    services/ServiceManagers/UserServiceManager/UserServiceConstants.h
)

set(SERVICE_MANAGER_PATH
    services/ServiceManagers/PathServiceManager/PathServiceConstants.h
    services/ServiceManagers/PathServiceManager/PathServiceManager.h
    services/ServiceManagers/PathServiceManager/PathServiceManager.cpp
)

set(SERVICE_MANAGER_NOTIFICATION
    services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.cpp
    services/ServiceManagers/NotificationServiceManager/NotificationServiceManager.h
    services/ServiceManagers/NotificationServiceManager/NotificationServiceConstants.h
    services/ServiceManagers/NotificationServiceManager/NotificationServiceStruct.h
)

set(SERVICE_MANAGER_TOOLBAR
    services/ServiceManagers/ToolbarServiceManager/ToolbarServiceManager.cpp
    services/ServiceManagers/ToolbarServiceManager/ToolbarServiceManager.h
    services/ServiceManagers/ToolbarServiceManager/ToolbarServiceConstants.h
    services/ServiceManagers/ToolbarServiceManager/ToolbarServiceStyling.h
)

set(SERVICE_MANAGER_UNINSTALL
    services/ServiceManagers/UninstallServiceManager/UninstallServiceManager.cpp
    services/ServiceManagers/UninstallServiceManager/UninstallServiceManager.h
    services/ServiceManagers/UninstallServiceManager/UninstallServiceConstants.h
)

set(SERVICE_MANAGER_FILEWATCHER
    services/ServiceManagers/FilewatcherServiceManager/FilewatcherServiceManager.cpp
    services/ServiceManagers/FilewatcherServiceManager/FilewatcherServiceManager.h
    services/ServiceManagers/FilewatcherServiceManager/FilewatcherServiceConstants.h
)

set(SERVICE_MANAGERS
    ${SERVICE_MANAGER_DIRECTOR}
    ${SERVICE_MANAGER_THEME}
    ${SERVICE_MANAGER_INSTALL}
    ${SERVICE_MANAGER_BACKUP}
    ${SERVICE_MANAGER_USER}
    ${SERVICE_MANAGER_NOTIFICATION}
    ${SERVICE_MANAGER_TOOLBAR}
    ${SERVICE_MANAGER_UNINSTALL}
    ${SERVICE_MANAGER_PATH}
    ${SERVICE_MANAGER_FILEWATCHER}
)

# Shared Utilities
set(UTILS_SHARED
    core/shared/fileoperations.cpp
    core/shared/fileoperations.h
    core/shared/jsonmanager.cpp
    core/shared/jsonmanager.h
    core/shared/uiutils.cpp
    core/shared/uiutils.h
    core/shared/formatutils.cpp
    core/shared/formatutils.h
    core/shared/stagingutils.cpp
    core/shared/stagingutils.h
)

# Constants
set(APP_CONSTANTS
    constants/app_info.h
    constants/window_config.h
    constants/backup_config.h
    constants/interface_config.h
    constants/units_config.h
    constants/system_constants.h
    constants/kvp_info.h
)

# Full Project Sources
set(PROJECT_SOURCES
    ${APP_ENTRY}
    ${RESOURCES}
    ${UI_ALL}
    ${CORE_BACKUP}
    ${SERVICE_MANAGERS}
    ${UTILS_SHARED}
    ${APP_CONSTANTS}
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
