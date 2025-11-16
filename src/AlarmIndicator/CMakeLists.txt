project(AlarmIndicatorWidget)

set(SOURCES
    AlarmIndicatorWidget.cpp
)

set(HEADERS
    AlarmIndicatorWidget.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX d
)












