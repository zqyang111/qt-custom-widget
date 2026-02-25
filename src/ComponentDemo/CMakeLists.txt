project(ComponentDemo)

set(SOURCES
    main.cpp
    ComponentDemoWindow.cpp
)

set(HEADERS
    ComponentDemoWindow.h
)

add_executable(ComponentDemo ${SOURCES} ${HEADERS})

# Add include directories for chart components (other widgets already have PUBLIC includes)
target_include_directories(ComponentDemo PRIVATE
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

target_link_libraries(ComponentDemo
    ${QT_CORE}
    ${QT_WIDGETS}
    # Basic widgets
    GaugeWidget
    ThermometerWidget
    IndustrialProgressBar
    LEDWidget
    DigitalMeterWidget
    # Advanced widgets
    LevelIndicatorWidget
    SevenSegmentWidget
    SwitchButtonWidget
    KnobWidget
    ValveWidget
    PumpWidget
    TankWidget
    TrendChartWidget
    # Extended widgets
    IndustrialSliderWidget
    FlowMeterWidget
    SpeedometerWidget
    AlarmIndicatorWidget
    PipeWidget
    BarGraphWidget
    # Time and Navigation widgets
    AnalogClockWidget
    DigitalClockWidget
    TimerWidget
    CompassWidget
    # Electrical and Energy widgets
    MotorControllerWidget
    BatteryIndicatorWidget
    ThreePhaseMeterWidget
    # Mechanical Components widgets
    GearWidget
    BeltDriveWidget
    PistonWidget
    PendulumWidget
    SpringWidget
    SliderCrankWidget
    CamWidget
    # Chart widgets
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
set_target_properties(ComponentDemo PROPERTIES
    OUTPUT_NAME "ComponentDemo"
    DEBUG_POSTFIX "d"
    WIN32_EXECUTABLE ON
)

# Copy to run directories
add_custom_command(TARGET ComponentDemo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/run/Debug,${CMAKE_SOURCE_DIR}/run/Release>
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:ComponentDemo>
        $<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/run/Debug,${CMAKE_SOURCE_DIR}/run/Release>
    COMMENT "Copying ComponentDemo executable to run directory"
)


