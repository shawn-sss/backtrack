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
set(ENTRY_POINT_SOURCES
    app/main.cpp
    app/app_manifest.rc
)

# Qt Resource Files
set(QT_RESOURCE_FILES
    resources/icons_resource.qrc
    resources/themes_resource.qrc
)

# Configuration Settings Headers
set(CONFIG_SETTINGS_HEADERS
    config/configsettings/_settings.h
    config/configsettings/app_settings.h
    config/configsettings/backup_settings.h
    config/configsettings/cfgmgr_settings.h
    config/configsettings/dialog_settings.h
    config/configsettings/labels_settings.h
    config/configsettings/messages_settings.h
    config/configsettings/notifications_settings.h
    config/configsettings/progress_settings.h
    config/configsettings/utilities_settings.h
    config/configsettings/resources_settings.h
    config/configsettings/timing_settings.h
    config/configsettings/visual_settings.h
)

# UI Components
set(UI_COMPONENTS
    ui/mainwindow/mainwindow.cpp
    ui/mainwindow/mainwindow.h
    ui/mainwindow/mainwindow.ui
    ui/notificationsdialog/notificationsdialog.h
    ui/notificationsdialog/notificationsdialog.cpp
    ui/notificationsdialog/notificationsdialog.ui
    ui/settingsdialog/settingsdialog.cpp
    ui/settingsdialog/settingsdialog.h
    ui/settingsdialog/settingsdialog.ui
    ui/toolbarmanager/toolbarmanager.h
    ui/toolbarmanager/toolbarmanager.cpp
    ui/toolbarmanager/toolbarmanagerstyling.h
    core/backup_module/styles/backup_styling.h
    ui/settingsdialog/settingsdialogstyling.h
    ui/mainwindow/mainwindowstyling.h
)

# Backup Core - Controller
set(BACKUP_CONTROLLER
    core/backup_module/controller/backupcontroller.cpp
    core/backup_module/controller/backupcontroller.h
)

# Backup Core - Service
set(BACKUP_SERVICE
    core/backup_module/service/backupservice.cpp
    core/backup_module/service/backupservice.h
)

# Backup Core - Worker
set(BACKUP_WORKER
    core/backup_module/worker/transferworker.cpp
    core/backup_module/worker/transferworker.h
)

# Backup Core - Models
set(BACKUP_MODELS
    core/backup_module/models/stagingmodel.cpp
    core/backup_module/models/stagingmodel.h
    core/backup_module/models/destinationproxymodel.cpp
    core/backup_module/models/destinationproxymodel.h
)

# Configuration Management
set(CONFIG_MANAGEMENT_MODULES
    config/configdirector/configdirector.cpp
    config/configdirector/configdirector.h
    config/configmanagers/thememanager/thememanager.cpp
    config/configmanagers/thememanager/thememanager.h
    config/configmanagers/installmetadatamanager/installmetadatamanager.cpp
    config/configmanagers/installmetadatamanager/installmetadatamanager.h
    config/configmanagers/backupsettingsmanager/backupsettingsmanager.cpp
    config/configmanagers/backupsettingsmanager/backupsettingsmanager.h
    config/configmanagers/usersettingsmanager/usersettingsmanager.cpp
    config/configmanagers/usersettingsmanager/usersettingsmanager.h
    config/configmanagers/notificationsmanager/notificationsmanager.cpp
    config/configmanagers/notificationsmanager/notificationsmanager.h
    config/configmanagers/notificationsmanager/notificationstruct.h
)

# Utility Modules
set(UTILITY_MODULES
    core/utils/file_utils/filewatcher.cpp
    core/utils/file_utils/filewatcher.h
    core/utils/file_utils/fileoperations.cpp
    core/utils/file_utils/fileoperations.h
    core/utils/common_utils/utils.cpp
    core/utils/common_utils/utils.h
    core/utils/file_utils/jsonmanager.cpp
    core/utils/file_utils/jsonmanager.h
)

# Aggregate All Sources
set(PROJECT_SOURCES
    ${ENTRY_POINT_SOURCES}
    ${QT_RESOURCE_FILES}
    ${CONFIG_SETTINGS_HEADERS}
    ${UI_COMPONENTS}
    ${BACKUP_CONTROLLER}
    ${BACKUP_SERVICE}
    ${BACKUP_WORKER}
    ${BACKUP_MODELS}
    ${CONFIG_MANAGEMENT_MODULES}
    ${UTILITY_MODULES}
)

# Add executable (define the target before applying properties)
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
