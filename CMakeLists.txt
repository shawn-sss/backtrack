cmake_minimum_required(VERSION 3.16)

# Default to Debug build type if none is specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(PROJECT_NAME MyDataBackupApp)
set(PROJECT_VERSION 0.1)
set(PROJECT_LANGUAGES CXX)

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES ${PROJECT_LANGUAGES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if(NOT DEFINED QT_VERSION_MAJOR)
    message(FATAL_ERROR "QT_VERSION_MAJOR is not defined. Ensure Qt5 or Qt6 is found properly.")
endif()

# UI sources
set(UI_SOURCES
    src/ui/mainwindow/MainWindow.cpp
    src/ui/mainwindow/MainWindow.h
    src/ui/mainwindow/MainWindow.ui
)

# Backup module sources
set(BACKUP_MODULE_SOURCES
    src/core/backup/BackupService.cpp
    src/core/backup/BackupService.h
    src/core/backup/FileOperations.cpp
    src/core/backup/FileOperations.h
    src/core/backup/StagingModel.cpp
    src/core/backup/StagingModel.h
    src/core/backup/TransferWorker.cpp
    src/core/backup/TransferWorker.h
    src/core/backup/BackupController.cpp
    src/core/backup/BackupController.h
)

# Utilities sources
set(UTILS_SOURCES
    src/core/utils/constants.h
    src/core/utils/FileWatcher.cpp
    src/core/utils/FileWatcher.h
    src/core/utils/Utils.cpp
    src/core/utils/Utils.h
)

# Project resources
set(PROJECT_RESOURCES
    src/resources/resource.qrc
)

# Combine all sources
set(PROJECT_SOURCES
    ${PROJECT_RESOURCES}
    ${UI_SOURCES}
    ${BACKUP_MODULE_SOURCES}
    ${UTILS_SOURCES}
    src/main.cpp
    src/app_manifest.rc
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
