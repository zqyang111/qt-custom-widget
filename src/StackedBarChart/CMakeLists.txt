add_library(StackedBarChart STATIC
    StackedBarChart.h
    StackedBarChart.cpp
)

target_link_libraries(StackedBarChart
    ${QT_CORE}
    ${QT_WIDGETS}
)










