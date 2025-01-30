cmake_minimum_required(VERSION 3.16)

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

set(UI_SOURCES
    UIComponents/mainwindow.cpp
    UIComponents/mainwindow.h
    UIComponents/mainwindow.ui
)

set(BACKUP_MODULE_SOURCES
    BackupModule/backupservice.cpp
    BackupModule/backupservice.h
    BackupModule/fileoperations.cpp
    BackupModule/fileoperations.h
    BackupModule/stagingmodel.cpp
    BackupModule/stagingmodel.h
    BackupModule/transferworker.cpp
    BackupModule/transferworker.h
    BackupModule/backupcontroller.cpp
    BackupModule/backupcontroller.h
)

set(UTILS_SOURCES
    Utils/constants.h
    Utils/filewatcher.cpp
    Utils/filewatcher.h
    Utils/utils.cpp
    Utils/utils.h
)

set(PROJECT_SOURCES
    img/resource.qrc
    ${UI_SOURCES}
    ${BACKUP_MODULE_SOURCES}
    ${UTILS_SOURCES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        main.cpp
        app_manifest.rc
    )
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
    else()
        add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
