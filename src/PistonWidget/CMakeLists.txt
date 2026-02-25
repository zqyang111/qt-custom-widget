cmake_minimum_required(VERSION 3.16)

project(PistonWidget VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(PistonWidget STATIC
    PistonWidget.h
    PistonWidget.cpp
)

target_link_libraries(PistonWidget
    ${QT_CORE}
    ${QT_GUI}
    ${QT_WIDGETS}
)

target_include_directories(PistonWidget PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)













