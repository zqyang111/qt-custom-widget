add_library(LineChart STATIC
    LineChart.h
    LineChart.cpp
)

target_link_libraries(LineChart
    ${QT_CORE}
    ${QT_WIDGETS}
)










