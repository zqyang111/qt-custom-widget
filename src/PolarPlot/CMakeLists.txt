add_library(PolarPlot STATIC
    PolarPlot.h
    PolarPlot.cpp
)

target_link_libraries(PolarPlot
    ${QT_CORE}
    ${QT_WIDGETS}
)










