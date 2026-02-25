#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTabWidget>

class GaugeWidget;
class ThermometerWidget;
class IndustrialProgressBar;
class LEDWidget;
class DigitalMeterWidget;
class LevelIndicatorWidget;
class SevenSegmentWidget;
class SwitchButtonWidget;
class KnobWidget;
class ValveWidget;
class PumpWidget;
class TankWidget;
class TrendChartWidget;
class IndustrialSliderWidget;
class FlowMeterWidget;
class SpeedometerWidget;
class AlarmIndicatorWidget;
class PipeWidget;
class BarGraphWidget;
class AnalogClockWidget;
class DigitalClockWidget;
class TimerWidget;
class CompassWidget;
class MotorControllerWidget;
class BatteryIndicatorWidget;
class ThreePhaseMeterWidget;
class GearWidget;
class BeltDriveWidget;
class PistonWidget;
class PendulumWidget;
class QSlider;

/**
 * @brief Main application window demonstrating all instrument widgets
 * 
 * This window displays all available industrial instrument widgets
 * organized in tabs with controls to interact with them.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onSimulationTimer();
    void onKnobValueChanged(double value);
    void onSwitchToggled(bool checked);

private:
    void setupUi();
    void createMenuBar();
    void createDisplayTab();
    void createControlTab();
    void createProcessTab();
    void createMonitorTab();
    void createExtendedTab();  // 5th tab
    void createTimeNavigationTab();  // 6th tab
    void createElectricalTab();  // 7th tab for electrical and energy widgets
    void createMechanicalTab();  // 8th tab for mechanical components
    void startSimulation();
    void stopSimulation();

    // Display widgets (Tab 1)
    GaugeWidget *m_gauge;
    ThermometerWidget *m_thermometer;
    IndustrialProgressBar *m_progressBar;
    LEDWidget *m_ledRed;
    LEDWidget *m_ledGreen;
    LEDWidget *m_ledYellow;
    DigitalMeterWidget *m_digitalMeter;
    SevenSegmentWidget *m_sevenSegment;

    // Control widgets (Tab 2)
    SwitchButtonWidget *m_switch1;
    SwitchButtonWidget *m_switch2;
    KnobWidget *m_knob;

    // Process widgets (Tab 3)
    ValveWidget *m_valve;
    PumpWidget *m_pump;
    LevelIndicatorWidget *m_levelIndicator;
    TankWidget *m_tank;

    // Monitor widgets (Tab 4)
    TrendChartWidget *m_trendChart;

    // Extended widgets (Tab 5)
    IndustrialSliderWidget *m_slider;
    FlowMeterWidget *m_flowMeter;
    SpeedometerWidget *m_speedometer;
    AlarmIndicatorWidget *m_alarmIndicator;
    PipeWidget *m_pipe;
    BarGraphWidget *m_barGraph;

    // Time and Navigation widgets (Tab 6)
    AnalogClockWidget *m_analogClock;
    DigitalClockWidget *m_digitalClock;
    TimerWidget *m_timer;
    CompassWidget *m_compass;

    // Electrical and Energy widgets (Tab 7)
    MotorControllerWidget *m_motorController;
    BatteryIndicatorWidget *m_battery;
    ThreePhaseMeterWidget *m_threePhaseMeter;

    // Mechanical Components widgets (Tab 8)
    GearWidget *m_gear;
    BeltDriveWidget *m_beltDrive;
    PistonWidget *m_piston;
    PendulumWidget *m_pendulum;

    // Main container
    QTabWidget *m_tabWidget;

    // Simulation
    QTimer *m_simulationTimer;
    bool m_simulationRunning;
    double m_simulationTime;
};

#endif // MAINWINDOW_H
