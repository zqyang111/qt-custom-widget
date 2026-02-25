add_library(RadarChart STATIC
    RadarChart.h
    RadarChart.cpp
)

target_link_libraries(RadarChart
    ${QT_CORE}
    ${QT_WIDGETS}
)










