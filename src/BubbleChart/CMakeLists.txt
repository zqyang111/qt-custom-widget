add_library(BubbleChart STATIC
    BubbleChart.h
    BubbleChart.cpp
)

target_link_libraries(BubbleChart
    ${QT_CORE}
    ${QT_WIDGETS}
)










