project(InstrumentControlDemo)

set(SOURCES
    main.cpp
    MainWindow.cpp
)

set(HEADERS
    MainWindow.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
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
    # Extended widgets (Phase 2)
    IndustrialSliderWidget
    FlowMeterWidget
    SpeedometerWidget
    AlarmIndicatorWidget
    PipeWidget
    BarGraphWidget
    # Time and Navigation widgets (Phase 3)
    AnalogClockWidget
    DigitalClockWidget
    TimerWidget
    CompassWidget
    # Electrical and Energy widgets (Phase 4)
    MotorControllerWidget
    BatteryIndicatorWidget
    ThreePhaseMeterWidget
    # Mechanical Components widgets (Phase 5)
    GearWidget
    BeltDriveWidget
    PistonWidget
    PendulumWidget
)

# Set output name with Debug postfix
set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX d
    WIN32_EXECUTABLE ON
)

# Copy to run directories
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/run/Debug,${CMAKE_SOURCE_DIR}/run/Release>
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME}>
        $<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/run/Debug,${CMAKE_SOURCE_DIR}/run/Release>
    COMMENT "Copying executable to run directory"
)


