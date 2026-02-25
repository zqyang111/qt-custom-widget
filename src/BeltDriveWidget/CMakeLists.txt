cmake_minimum_required(VERSION 3.16)

project(BeltDriveWidget VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(BeltDriveWidget STATIC
    BeltDriveWidget.h
    BeltDriveWidget.cpp
)

target_link_libraries(BeltDriveWidget
    ${QT_CORE}
    ${QT_GUI}
    ${QT_WIDGETS}
)

target_include_directories(BeltDriveWidget PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)













