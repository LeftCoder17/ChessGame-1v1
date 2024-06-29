cmake_minimum_required(VERSION 3.5)

project(ChessApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ChessApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        libs/chessboardwidget.h libs/chessboardwidget.cpp
        libs/chessboard.h libs/chessboard.cpp
        libs/position.h libs/position.cpp
        libs/piece.h libs/piece.cpp
        libs/managebits.h libs/managebits.cpp
        libs/capturedpieces.h libs/capturedpieces.cpp
        libs/promotiondialog.h libs/promotiondialog.cpp
        libs/resultdialog.h libs/resultdialog.cpp
        resources.qrc
    )
endif()

target_link_libraries(ChessApp PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Svg
)

set_target_properties(ChessApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ChessApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ChessApp)
endif()
