cmake_minimum_required(VERSION 3.17)
project(2dxcustomize)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${PROJECT_NAME} WIN32
    src/main.cpp
    src/context.cpp
    src/process.cpp
    src/customize.cpp
    res/main.rc
    res/assets.qrc
    src/main_window.ui
    src/main_window.cpp
    src/loading_dialog.ui
    src/loading_dialog.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets)