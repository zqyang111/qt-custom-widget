cmake_minimum_required(VERSION 3.20)

project(SliderCrankWidget)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(SliderCrankWidget STATIC
    SliderCrankWidget.h
    SliderCrankWidget.cpp
)

target_link_libraries(SliderCrankWidget
    ${QT_CORE}
    ${QT_WIDGETS}
)

target_include_directories(SliderCrankWidget PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)




