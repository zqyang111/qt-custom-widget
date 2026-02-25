cmake_minimum_required(VERSION 3.20)

project(CamWidget)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(CamWidget STATIC
    CamWidget.h
    CamWidget.cpp
)

target_link_libraries(CamWidget
    ${QT_CORE}
    ${QT_WIDGETS}
)

target_include_directories(CamWidget PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)




