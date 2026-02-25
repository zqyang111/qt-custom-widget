add_library(WaveformChart STATIC
    WaveformChart.h
    WaveformChart.cpp
)

target_link_libraries(WaveformChart
    ${QT_CORE}
    ${QT_WIDGETS}
)












