add_library(ScatterPlot STATIC
    ScatterPlot.h
    ScatterPlot.cpp
)

target_link_libraries(ScatterPlot
    ${QT_CORE}
    ${QT_WIDGETS}
)










