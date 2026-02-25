add_library(PieChart STATIC
    PieChart.h
    PieChart.cpp
)

target_link_libraries(PieChart
    ${QT_CORE}
    ${QT_WIDGETS}
)










