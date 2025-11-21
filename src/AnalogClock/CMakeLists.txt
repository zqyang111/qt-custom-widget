project(AnalogClockWidget)

set(SOURCES
    AnalogClockWidget.cpp
)

set(HEADERS
    AnalogClockWidget.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
    ${QT_CORE}
    ${QT_WIDGETS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX d
)













