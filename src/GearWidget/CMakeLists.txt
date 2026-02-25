cmake_minimum_required(VERSION 3.16)

project(GearWidget VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


add_library(GearWidget STATIC
    GearWidget.h
    GearWidget.cpp
)

target_link_libraries(GearWidget
    ${QT_CORE}
    ${QT_GUI}
    ${QT_WIDGETS}
)

target_include_directories(GearWidget PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)













