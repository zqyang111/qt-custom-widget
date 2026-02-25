set(SOURCES
    main.cpp
    ChartDemoWindow.cpp
    ChartDemoWindow.h
)

add_executable(ChartDemo ${SOURCES})

# Add include directories for chart components
target_include_directories(ChartDemo PRIVATE
    ${CMAKE_SOURCE_DIR}/src/WaveformChart
    ${CMAKE_SOURCE_DIR}/src/XYGraph
    ${CMAKE_SOURCE_DIR}/src/IntensityChart
    ${CMAKE_SOURCE_DIR}/src/PieChart
    ${CMAKE_SOURCE_DIR}/src/LineChart
    ${CMAKE_SOURCE_DIR}/src/Histogram
    ${CMAKE_SOURCE_DIR}/src/PolarPlot
    ${CMAKE_SOURCE_DIR}/src/SpectrumAnalyzer
    ${CMAKE_SOURCE_DIR}/src/ScatterPlot
    ${CMAKE_SOURCE_DIR}/src/AreaChart
    ${CMAKE_SOURCE_DIR}/src/RadarChart
    ${CMAKE_SOURCE_DIR}/src/BubbleChart
    ${CMAKE_SOURCE_DIR}/src/DigitalWaveform
    ${CMAKE_SOURCE_DIR}/src/StackedBarChart
)

target_link_libraries(ChartDemo
    ${QT_CORE}
    ${QT_WIDGETS}
    WaveformChart
    XYGraph
    IntensityChart
    PieChart
    LineChart
    Histogram
    PolarPlot
    SpectrumAnalyzer
    ScatterPlot
    AreaChart
    RadarChart
    BubbleChart
    DigitalWaveform
    StackedBarChart
)

# Set output name with Debug postfix
set_target_properties(ChartDemo PROPERTIES
    DEBUG_POSTFIX d
)

