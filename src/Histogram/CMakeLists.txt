add_library(Histogram STATIC
    Histogram.h
    Histogram.cpp
)

target_link_libraries(Histogram
    ${QT_CORE}
    ${QT_WIDGETS}
)










