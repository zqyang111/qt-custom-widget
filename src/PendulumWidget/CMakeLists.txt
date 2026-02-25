cmake_minimum_required(VERSION 3.16)

project(PendulumWidget VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(PendulumWidget STATIC
    PendulumWidget.h
    PendulumWidget.cpp
)

target_link_libraries(PendulumWidget
    ${QT_CORE}
    ${QT_GUI}
    ${QT_WIDGETS}
)

target_include_directories(PendulumWidget PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)













