cmake_minimum_required(VERSION 3.20)

project(SpringWidget)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


add_library(SpringWidget STATIC
    SpringWidget.h
    SpringWidget.cpp
)

target_link_libraries(SpringWidget
    ${QT_CORE}
    ${QT_WIDGETS}
)

target_include_directories(SpringWidget PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)




