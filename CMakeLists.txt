cmake_minimum_required(VERSION 3.16)

# Default to Debug build type if none is specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(PROJECT_NAME MyDataBackupApp)
set(PROJECT_VERSION 0.3)
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

# App sources
set(APP_SOURCES
    app/main.cpp
    app/app_manifest.rc
)

# Project resources
set(PROJECT_RESOURCES
    resources/resource.qrc
)

# Project settings
set(PROJECT_SETTINGS
    config/_constants.h
    config/app_settings.h
    config/backup_settings.h
    config/cfgmgr_settings.h
    config/messages_settings.h
    config/progress_settings.h
    config/utilities_settings.h
    config/resources_settings.h
    config/visual_settings.h
)

# UI sources
set(UI_SOURCES
    ui/customtitlebar/customtitlebar.cpp
    ui/customtitlebar/customtitlebar.h
    ui/mainwindow/MainWindow.cpp
    ui/mainwindow/MainWindow.h
    ui/mainwindow/MainWindow.ui
    ui/settingsdialog/settingsdialog.cpp
    ui/settingsdialog/settingsdialog.h
    ui/settingsdialog/settingsdialog.ui
)

# Backup Controller sources
set(BACKUP_CONTROLLER_SOURCES
    core/backup_module/controller/BackupController.cpp
    core/backup_module/controller/BackupController.h
)

# Backup Service sources
set(BACKUP_SERVICE_SOURCES
    core/backup_module/service/BackupService.cpp
    core/backup_module/service/BackupService.h
)

# Backup Worker sources
set(BACKUP_WORKER_SOURCES
    core/backup_module/worker/TransferWorker.cpp
    core/backup_module/worker/TransferWorker.h
)

# Backup Models sources
set(BACKUP_MODELS_SOURCES
    core/backup_module/models/StagingModel.cpp
    core/backup_module/models/StagingModel.h
)

# Utilities sources
set(UTILS_SOURCES
    core/utils/file_utils/FileWatcher.cpp
    core/utils/file_utils/FileWatcher.h
    core/utils/file_utils/fileoperations.cpp
    core/utils/file_utils/fileoperations.h
    core/utils/common_utils/Utils.cpp
    core/utils/common_utils/Utils.h
    config/configmanager/configmanager.cpp
    config/configmanager/configmanager.h
)

# Combine all sources
set(PROJECT_SOURCES
    ${APP_SOURCES}
    ${PROJECT_RESOURCES}
    ${PROJECT_SETTINGS}
    ${UI_SOURCES}
    ${BACKUP_CONTROLLER_SOURCES}
    ${BACKUP_SERVICE_SOURCES}
    ${BACKUP_OPERATIONS_SOURCES}
    ${BACKUP_WORKER_SOURCES}
    ${BACKUP_MODELS_SOURCES}
    ${UTILS_SOURCES}
)

# Add executable (define the target before applying properties)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ui/customtitlebar/titlebarmode.h
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
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

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
