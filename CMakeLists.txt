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
set(APP_ENTRY_POINT
    app/main.cpp
    app/app_manifest.rc
)

# Qt Resource Files
set(QT_RESOURCES
    resources/icons_resource.qrc
    resources/themes_resource.qrc
)

# UI - Windows, Dialogs & Styling
set(UI_SOURCES
    ui/mainwindow/mainwindow.cpp
    ui/mainwindow/mainwindow.h
    ui/mainwindow/mainwindow.ui
    ui/mainwindow/mainwindowstyling.h
    ui/mainwindow/mainwindowlabels.h
    ui/mainwindow/mainwindowmessages.h

    ui/notificationsdialog/notificationsdialog.h
    ui/notificationsdialog/notificationsdialog.cpp
    ui/notificationsdialog/notificationsdialog.ui
    ui/notificationsdialog/notificationsdialogstyling.h

    ui/settingsdialog/settingsdialog.cpp
    ui/settingsdialog/settingsdialog.h
    ui/settingsdialog/settingsdialog.ui
    ui/settingsdialog/settingsdialogstyling.h
    ui/settingsdialog/settingsdialogconstants.h

    ui/toolbarmanager/toolbarmanager.h
    ui/toolbarmanager/toolbarmanager.cpp
    ui/toolbarmanager/toolbarmanagerstyling.h
    ui/toolbarmanager/toolbarmanagerconstants.h
)

# Backup Core - Modules
set(BACKUP_CORE
    core/backup_module/controller/backupcontroller.cpp
    core/backup_module/controller/backupcontroller.h

    core/backup_module/service/backupservice.cpp
    core/backup_module/service/backupservice.h

    core/backup_module/worker/transferworker.cpp
    core/backup_module/worker/transferworker.h

    core/backup_module/models/stagingmodel.cpp
    core/backup_module/models/stagingmodel.h
    core/backup_module/models/destinationproxymodel.cpp
    core/backup_module/models/destinationproxymodel.h
)

# Config Modules
set(CONFIG_MODULES
    config/ConfigDirector/ConfigDirector.cpp
    config/ConfigDirector/ConfigDirector.h
    config/ConfigDirector/ConfigDirectorConstants.h

    config/ConfigManagers/ThemeConfigManager/ThemeConfigManager.cpp
    config/ConfigManagers/ThemeConfigManager/ThemeConfigManager.h
    config/ConfigManagers/ThemeConfigManager/ThemeConfigConstants.h

    config/ConfigManagers/InstallConfigManager/InstallConfigManager.cpp
    config/ConfigManagers/InstallConfigManager/InstallConfigManager.h
    config/ConfigManagers/InstallConfigManager/InstallConfigConstants.h

    config/ConfigManagers/BackupConfigManager/BackupConfigManager.cpp
    config/ConfigManagers/BackupConfigManager/BackupConfigManager.h
    config/ConfigManagers/BackupConfigManager/BackupConfigConstants.h

    config/ConfigManagers/UserConfigManager/UserConfigManager.cpp
    config/ConfigManagers/UserConfigManager/UserConfigManager.h
    config/ConfigManagers/UserConfigManager/UserConfigConstants.h

    config/ConfigManagers/NotificationConfigManager/NotificationConfigManager.cpp
    config/ConfigManagers/NotificationConfigManager/NotificationConfigManager.h
    config/ConfigManagers/NotificationConfigManager/NotificationConfigStruct.h
    config/ConfigManagers/NotificationConfigManager/NotificationConfigConstants.h

    config/ConfigSettings/app_settings.h
)

# Utilities
set(UTILITY_SOURCES
    core/utils/file_utils/filewatcher.cpp
    core/utils/file_utils/filewatcher.h
    core/utils/file_utils/fileoperations.cpp
    core/utils/file_utils/fileoperations.h
    core/utils/file_utils/jsonmanager.cpp
    core/utils/file_utils/jsonmanager.h
    core/utils/common_utils/utils.cpp
    core/utils/common_utils/utils.h
)

# Full Project Source List
set(PROJECT_SOURCES
    ${APP_ENTRY_POINT}
    ${QT_RESOURCES}
    ${UI_SOURCES}
    ${BACKUP_CORE}
    ${CONFIG_MODULES}
    ${UTILITY_SOURCES}
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
