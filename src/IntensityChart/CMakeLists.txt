add_library(IntensityChart STATIC
    IntensityChart.h
    IntensityChart.cpp
)

target_link_libraries(IntensityChart
    ${QT_CORE}
    ${QT_WIDGETS}
)












