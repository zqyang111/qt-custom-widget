cmake_minimum_required(VERSION 3.20)
project(InstrumentControl VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/run/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/run/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/run/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/run/Release)

# Set Debug postfix
set(CMAKE_DEBUG_POSTFIX d)

# Find Qt5 or Qt6
# Try Qt6 first, fall back to Qt5 if not found
set(QT_MIN_VERSION "5.12")
find_package(Qt6 QUIET COMPONENTS Core Widgets)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
    set(QT_PREFIX Qt6)
    message(STATUS "Found Qt6, using Qt6")
else()
    find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Widgets)
    set(QT_VERSION_MAJOR 5)
    set(QT_PREFIX Qt5)
    message(STATUS "Qt6 not found, using Qt5")
endif()

# Set Qt variables for subdirectories (use CACHE to make them available in subdirectories)
set(QT_CORE ${QT_PREFIX}::Core CACHE STRING "Qt Core library target")
set(QT_GUI ${QT_PREFIX}::Gui CACHE STRING "Qt Gui library target")
set(QT_WIDGETS ${QT_PREFIX}::Widgets CACHE STRING "Qt Widgets library target")

# Add subdirectories for each instrument module
# Basic display widgets
add_subdirectory(src/Gauge)
add_subdirectory(src/Thermometer)
add_subdirectory(src/ProgressBar)
add_subdirectory(src/LED)
add_subdirectory(src/DigitalMeter)

# Advanced widgets
add_subdirectory(src/LevelIndicator)
add_subdirectory(src/SevenSegment)
add_subdirectory(src/SwitchButton)
add_subdirectory(src/Knob)
add_subdirectory(src/Valve)
add_subdirectory(src/Pump)
add_subdirectory(src/Tank)
add_subdirectory(src/TrendChart)

# Extended widgets (Phase 2)
add_subdirectory(src/IndustrialSlider)
add_subdirectory(src/FlowMeter)
add_subdirectory(src/Speedometer)
add_subdirectory(src/AlarmIndicator)
add_subdirectory(src/Pipe)
add_subdirectory(src/BarGraph)

# Time and Navigation widgets (Phase 3)
add_subdirectory(src/AnalogClock)
add_subdirectory(src/DigitalClock)
add_subdirectory(src/TimerWidget)
add_subdirectory(src/Compass)

# Electrical and Energy widgets (Phase 4)
add_subdirectory(src/MotorController)
add_subdirectory(src/BatteryIndicator)
add_subdirectory(src/ThreePhaseMeter)

# Mechanical Components widgets (Phase 5)
add_subdirectory(src/GearWidget)
add_subdirectory(src/BeltDriveWidget)
add_subdirectory(src/PistonWidget)
add_subdirectory(src/PendulumWidget)
add_subdirectory(src/SpringWidget)
add_subdirectory(src/SliderCrankWidget)
add_subdirectory(src/CamWidget)

# LabVIEW-Style Chart Components (Phase 6)
add_subdirectory(src/WaveformChart)
add_subdirectory(src/XYGraph)
add_subdirectory(src/IntensityChart)
add_subdirectory(src/PieChart)
add_subdirectory(src/LineChart)
add_subdirectory(src/Histogram)
add_subdirectory(src/PolarPlot)
add_subdirectory(src/SpectrumAnalyzer)
add_subdirectory(src/ScatterPlot)
add_subdirectory(src/AreaChart)
add_subdirectory(src/RadarChart)
add_subdirectory(src/BubbleChart)
add_subdirectory(src/DigitalWaveform)
add_subdirectory(src/StackedBarChart)

# Add main applications
add_subdirectory(src/MainApp)
add_subdirectory(src/ChartDemo)
add_subdirectory(src/ComponentDemo)

