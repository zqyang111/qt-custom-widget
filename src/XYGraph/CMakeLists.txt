add_library(XYGraph STATIC
    XYGraph.h
    XYGraph.cpp
)

target_link_libraries(XYGraph
    ${QT_CORE}
    ${QT_WIDGETS}
)












