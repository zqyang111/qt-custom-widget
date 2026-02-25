#include "MainWindow.h"
#include "GaugeWidget.h"
#include "ThermometerWidget.h"
#include "IndustrialProgressBar.h"
#include "LEDWidget.h"
#include "DigitalMeterWidget.h"
#include "LevelIndicatorWidget.h"
#include "SevenSegmentWidget.h"
#include "SwitchButtonWidget.h"
#include "KnobWidget.h"
#include "ValveWidget.h"
#include "PumpWidget.h"
#include "TankWidget.h"
#include "TrendChartWidget.h"
#include "IndustrialSliderWidget.h"
#include "FlowMeterWidget.h"
#include "SpeedometerWidget.h"
#include "AlarmIndicatorWidget.h"
#include "PipeWidget.h"
#include "BarGraphWidget.h"
#include "AnalogClockWidget.h"
#include "DigitalClockWidget.h"
#include "TimerWidget.h"
#include "CompassWidget.h"
#include "MotorControllerWidget.h"
#include "BatteryIndicatorWidget.h"
#include "ThreePhaseMeterWidget.h"
#include "GearWidget.h"
#include "BeltDriveWidget.h"
#include "PistonWidget.h"
#include "PendulumWidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QTabWidget>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_gauge(nullptr)
    , m_thermometer(nullptr)
    , m_progressBar(nullptr)
    , m_ledRed(nullptr)
    , m_ledGreen(nullptr)
    , m_ledYellow(nullptr)
    , m_digitalMeter(nullptr)
    , m_sevenSegment(nullptr)
    , m_switch1(nullptr)
    , m_switch2(nullptr)
    , m_knob(nullptr)
    , m_valve(nullptr)
    , m_pump(nullptr)
    , m_levelIndicator(nullptr)
    , m_tank(nullptr)
    , m_trendChart(nullptr)
    , m_slider(nullptr)
    , m_flowMeter(nullptr)
    , m_speedometer(nullptr)
    , m_alarmIndicator(nullptr)
    , m_pipe(nullptr)
    , m_barGraph(nullptr)
    , m_analogClock(nullptr)
    , m_digitalClock(nullptr)
    , m_timer(nullptr)
    , m_compass(nullptr)
    , m_motorController(nullptr)
    , m_battery(nullptr)
    , m_threePhaseMeter(nullptr)
    , m_gear(nullptr)
    , m_beltDrive(nullptr)
    , m_piston(nullptr)
    , m_pendulum(nullptr)
    , m_tabWidget(nullptr)
    , m_simulationTimer(new QTimer(this))
    , m_simulationRunning(false)
    , m_simulationTime(0.0)
{
    setWindowTitle(tr("Industrial Instrument Control Demo - Extended"));
    resize(1400, 900);

    setupUi();
    createMenuBar();

    connect(m_simulationTimer, &QTimer::timeout, this, &MainWindow::onSimulationTimer);
    m_simulationTimer->setInterval(50); // 20 Hz update rate
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Title
    QLabel *titleLabel = new QLabel(tr("Industrial Instrument Control System - Complete Suite"), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Tab widget
    m_tabWidget = new QTabWidget(this);
    mainLayout->addWidget(m_tabWidget, 1);

    // Create tabs
    createDisplayTab();
    createControlTab();
    createProcessTab();
    createMonitorTab();
    createExtendedTab();
    createTimeNavigationTab();
    createElectricalTab();  // 7th tab for electrical and energy
    createMechanicalTab();  // 8th tab for mechanical components

    // Control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    mainLayout->addLayout(buttonLayout);

    QPushButton *startSimBtn = new QPushButton(tr("Start Simulation"), this);
    QPushButton *stopSimBtn = new QPushButton(tr("Stop Simulation"), this);
    QPushButton *resetBtn = new QPushButton(tr("Reset All"), this);

    buttonLayout->addStretch();
    buttonLayout->addWidget(startSimBtn);
    buttonLayout->addWidget(stopSimBtn);
    buttonLayout->addWidget(resetBtn);
    buttonLayout->addStretch();

    connect(startSimBtn, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(stopSimBtn, &QPushButton::clicked, this, &MainWindow::stopSimulation);
    connect(resetBtn, &QPushButton::clicked, [this]() {
        stopSimulation();
        m_simulationTime = 0.0;
        m_gauge->setValue(0);
        m_thermometer->setTemperature(20);
        m_progressBar->setValue(0);
        m_digitalMeter->setValue(0);
        m_levelIndicator->setLevel(50);
        m_tank->setLevel(50);
        m_valve->setOpening(0);
        m_pump->stop();
        m_knob->setValue(0);
        m_sevenSegment->setValue(0);
        m_trendChart->clearAllSeries();
        
        // Reset extended widgets
        if (m_slider) m_slider->setValue(0);
        if (m_flowMeter) {
            m_flowMeter->setFlowRate(0);
            m_flowMeter->resetTotal();
        }
        if (m_speedometer) m_speedometer->setValue(0);
        if (m_alarmIndicator) m_alarmIndicator->clearAllAlarms();
        if (m_pipe) m_pipe->setFlowRate(0);
    });

    // Set dark theme
    setStyleSheet(
        "QMainWindow { background-color: #2b2b2b; }"
        "QWidget { background-color: #2b2b2b; color: #ffffff; }"
        "QTabWidget::pane { border: 2px solid #555555; background-color: #2b2b2b; }"
        "QTabBar::tab { background-color: #3a3a3a; color: #ffffff; padding: 8px 20px; "
        "              border: 1px solid #555555; margin-right: 2px; }"
        "QTabBar::tab:selected { background-color: #4a4a4a; border-bottom: 2px solid #00c800; }"
        "QTabBar::tab:hover { background-color: #454545; }"
        "QGroupBox { border: 2px solid #555555; border-radius: 5px; "
        "           margin-top: 10px; padding-top: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; "
        "                  padding: 5px; background-color: #3a3a3a; border-radius: 3px; }"
        "QPushButton { background-color: #4a4a4a; border: 1px solid #666666; "
        "             border-radius: 4px; padding: 8px; min-width: 120px; }"
        "QPushButton:hover { background-color: #5a5a5a; }"
        "QPushButton:pressed { background-color: #3a3a3a; }"
    );
}

void MainWindow::createDisplayTab()
{
    QWidget *displayTab = new QWidget();
    QHBoxLayout *tabLayout = new QHBoxLayout(displayTab);

    // Left column
    QVBoxLayout *leftColumn = new QVBoxLayout();
    
    QGroupBox *gaugeGroup = new QGroupBox(tr("Pressure Gauge"), displayTab);
    QVBoxLayout *gaugeLayout = new QVBoxLayout(gaugeGroup);
    m_gauge = new GaugeWidget(displayTab);
    m_gauge->setRange(0, 100);
    m_gauge->setUnit("PSI");
    m_gauge->setWarningRange(70, 90);
    gaugeLayout->addWidget(m_gauge, 0, Qt::AlignCenter);
    leftColumn->addWidget(gaugeGroup);

    QGroupBox *sevenSegGroup = new QGroupBox(tr("Seven Segment Display"), displayTab);
    QVBoxLayout *sevenSegLayout = new QVBoxLayout(sevenSegGroup);
    m_sevenSegment = new SevenSegmentWidget(displayTab);
    m_sevenSegment->setDigitCount(6);
    m_sevenSegment->setValue(12345);
    sevenSegLayout->addWidget(m_sevenSegment, 0, Qt::AlignCenter);
    leftColumn->addWidget(sevenSegGroup);

    tabLayout->addLayout(leftColumn);

    // Middle column
    QVBoxLayout *middleColumn = new QVBoxLayout();

    QGroupBox *thermoGroup = new QGroupBox(tr("Temperature Sensor"), displayTab);
    QVBoxLayout *thermoLayout = new QVBoxLayout(thermoGroup);
    m_thermometer = new ThermometerWidget(displayTab);
    m_thermometer->setRange(-20, 120);
    m_thermometer->setWarningRange(80, 100);
    thermoLayout->addWidget(m_thermometer, 0, Qt::AlignCenter);
    middleColumn->addWidget(thermoGroup);

    tabLayout->addLayout(middleColumn);

    // Right column
    QVBoxLayout *rightColumn = new QVBoxLayout();

    QGroupBox *progressGroup = new QGroupBox(tr("Process Progress"), displayTab);
    QVBoxLayout *progressLayout = new QVBoxLayout(progressGroup);
    m_progressBar = new IndustrialProgressBar(displayTab);
    m_progressBar->setRange(0, 100);
    progressLayout->addWidget(m_progressBar);
    rightColumn->addWidget(progressGroup);

    QGroupBox *ledGroup = new QGroupBox(tr("Status Indicators"), displayTab);
    QHBoxLayout *ledLayout = new QHBoxLayout(ledGroup);
    
    QVBoxLayout *redLedLayout = new QVBoxLayout();
    m_ledRed = new LEDWidget(displayTab);
    m_ledRed->setColor(LEDWidget::Red);
    redLedLayout->addWidget(m_ledRed, 0, Qt::AlignCenter);
    redLedLayout->addWidget(new QLabel(tr("Critical"), displayTab), 0, Qt::AlignCenter);
    ledLayout->addLayout(redLedLayout);

    QVBoxLayout *yellowLedLayout = new QVBoxLayout();
    m_ledYellow = new LEDWidget(displayTab);
    m_ledYellow->setColor(LEDWidget::Yellow);
    yellowLedLayout->addWidget(m_ledYellow, 0, Qt::AlignCenter);
    yellowLedLayout->addWidget(new QLabel(tr("Warning"), displayTab), 0, Qt::AlignCenter);
    ledLayout->addLayout(yellowLedLayout);

    QVBoxLayout *greenLedLayout = new QVBoxLayout();
    m_ledGreen = new LEDWidget(displayTab);
    m_ledGreen->setColor(LEDWidget::Green);
    m_ledGreen->setState(LEDWidget::On);
    greenLedLayout->addWidget(m_ledGreen, 0, Qt::AlignCenter);
    greenLedLayout->addWidget(new QLabel(tr("Normal"), displayTab), 0, Qt::AlignCenter);
    ledLayout->addLayout(greenLedLayout);
    
    rightColumn->addWidget(ledGroup);

    QGroupBox *digitalGroup = new QGroupBox(tr("Power Meter"), displayTab);
    QVBoxLayout *digitalLayout = new QVBoxLayout(digitalGroup);
    m_digitalMeter = new DigitalMeterWidget(displayTab);
    m_digitalMeter->setLabel(tr("Power Output"));
    m_digitalMeter->setUnit("kW");
    m_digitalMeter->setPrecision(2);
    digitalLayout->addWidget(m_digitalMeter);
    rightColumn->addWidget(digitalGroup);

    tabLayout->addLayout(rightColumn);

    m_tabWidget->addTab(displayTab, tr("Display Instruments"));
}

void MainWindow::createControlTab()
{
    QWidget *controlTab = new QWidget();
    QHBoxLayout *tabLayout = new QHBoxLayout(controlTab);

    // Knob control
    QGroupBox *knobGroup = new QGroupBox(tr("Rotary Knob Control"), controlTab);
    QVBoxLayout *knobLayout = new QVBoxLayout(knobGroup);
    m_knob = new KnobWidget(controlTab);
    m_knob->setRange(0, 100);
    m_knob->setStep(1.0);
    knobLayout->addWidget(m_knob, 0, Qt::AlignCenter);
    
    QLabel *knobInfo = new QLabel(tr("Drag or use mouse wheel to adjust"), controlTab);
    knobInfo->setAlignment(Qt::AlignCenter);
    knobLayout->addWidget(knobInfo);
    
    connect(m_knob, &KnobWidget::valueChanged, this, &MainWindow::onKnobValueChanged);
    
    tabLayout->addWidget(knobGroup);

    // Switch controls
    QVBoxLayout *switchColumn = new QVBoxLayout();
    
    QGroupBox *switch1Group = new QGroupBox(tr("Main Power Switch"), controlTab);
    QVBoxLayout *switch1Layout = new QVBoxLayout(switch1Group);
    m_switch1 = new SwitchButtonWidget(controlTab);
    switch1Layout->addWidget(m_switch1, 0, Qt::AlignCenter);
    connect(m_switch1, &SwitchButtonWidget::toggled, this, &MainWindow::onSwitchToggled);
    switchColumn->addWidget(switch1Group);

    QGroupBox *switch2Group = new QGroupBox(tr("Auxiliary Switch"), controlTab);
    QVBoxLayout *switch2Layout = new QVBoxLayout(switch2Group);
    m_switch2 = new SwitchButtonWidget(controlTab);
    switch2Layout->addWidget(m_switch2, 0, Qt::AlignCenter);
    switchColumn->addWidget(switch2Group);

    switchColumn->addStretch();

    tabLayout->addLayout(switchColumn);

    m_tabWidget->addTab(controlTab, tr("Control Widgets"));
}

void MainWindow::createProcessTab()
{
    QWidget *processTab = new QWidget();
    QHBoxLayout *tabLayout = new QHBoxLayout(processTab);

    // Valve
    QGroupBox *valveGroup = new QGroupBox(tr("Control Valve"), processTab);
    QVBoxLayout *valveLayout = new QVBoxLayout(valveGroup);
    m_valve = new ValveWidget(processTab);
    m_valve->setType(ValveWidget::BallValve);
    valveLayout->addWidget(m_valve, 0, Qt::AlignCenter);
    tabLayout->addWidget(valveGroup);

    // Pump
    QGroupBox *pumpGroup = new QGroupBox(tr("Centrifugal Pump"), processTab);
    QVBoxLayout *pumpLayout = new QVBoxLayout(pumpGroup);
    m_pump = new PumpWidget(processTab);
    pumpLayout->addWidget(m_pump, 0, Qt::AlignCenter);
    
    QHBoxLayout *pumpControlLayout = new QHBoxLayout();
    QPushButton *startPumpBtn = new QPushButton(tr("Start Pump"), processTab);
    QPushButton *stopPumpBtn = new QPushButton(tr("Stop Pump"), processTab);
    pumpControlLayout->addWidget(startPumpBtn);
    pumpControlLayout->addWidget(stopPumpBtn);
    pumpLayout->addLayout(pumpControlLayout);
    
    connect(startPumpBtn, &QPushButton::clicked, [this]() {
        m_pump->start();
    });
    connect(stopPumpBtn, &QPushButton::clicked, [this]() {
        m_pump->stop();
    });
    
    tabLayout->addWidget(pumpGroup);

    // Level Indicator
    QGroupBox *levelGroup = new QGroupBox(tr("Level Indicator"), processTab);
    QVBoxLayout *levelLayout = new QVBoxLayout(levelGroup);
    m_levelIndicator = new LevelIndicatorWidget(processTab);
    m_levelIndicator->setRange(0, 100);
    m_levelIndicator->setUnit("%");
    levelLayout->addWidget(m_levelIndicator, 0, Qt::AlignCenter);
    tabLayout->addWidget(levelGroup);

    // Tank
    QGroupBox *tankGroup = new QGroupBox(tr("Storage Tank"), processTab);
    QVBoxLayout *tankLayout = new QVBoxLayout(tankGroup);
    m_tank = new TankWidget(processTab);
    m_tank->setShape(TankWidget::Cylindrical);
    m_tank->setCapacity(1000, "L");
    tankLayout->addWidget(m_tank, 0, Qt::AlignCenter);
    tabLayout->addWidget(tankGroup);

    m_tabWidget->addTab(processTab, tr("Process Control"));
}

void MainWindow::createMonitorTab()
{
    QWidget *monitorTab = new QWidget();
    QVBoxLayout *tabLayout = new QVBoxLayout(monitorTab);

    QGroupBox *trendGroup = new QGroupBox(tr("Real-Time Trend Chart"), monitorTab);
    QVBoxLayout *trendLayout = new QVBoxLayout(trendGroup);
    
    m_trendChart = new TrendChartWidget(monitorTab);
    m_trendChart->setMaxDataPoints(100);
    m_trendChart->setAutoScale(true);
    
    // Add series
    m_trendChart->addSeries("Pressure", QColor(255, 100, 100));
    m_trendChart->addSeries("Temperature", QColor(100, 255, 100));
    m_trendChart->addSeries("Level", QColor(100, 100, 255));
    
    trendLayout->addWidget(m_trendChart);

    QLabel *trendInfo = new QLabel(
        tr("The trend chart displays real-time data from Pressure Gauge, Temperature Sensor, and Level Indicator"),
        monitorTab);
    trendInfo->setWordWrap(true);
    trendInfo->setAlignment(Qt::AlignCenter);
    trendLayout->addWidget(trendInfo);

    tabLayout->addWidget(trendGroup);

    m_tabWidget->addTab(monitorTab, tr("Data Monitoring"));
}

void MainWindow::createMenuBar()
{
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu(tr("&File"));
    QAction *exitAction = fileMenu->addAction(tr("E&xit"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);

    QMenu *viewMenu = menuBar->addMenu(tr("&View"));
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        QAction *tabAction = viewMenu->addAction(m_tabWidget->tabText(i));
        int tabIndex = i;
        connect(tabAction, &QAction::triggered, [this, tabIndex]() {
            m_tabWidget->setCurrentIndex(tabIndex);
        });
    }

    QMenu *helpMenu = menuBar->addMenu(tr("&Help"));
    QAction *aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, &QAction::triggered, [this]() {
        // Could add an about dialog here
    });
}

void MainWindow::startSimulation()
{
    if (!m_simulationRunning) {
        m_simulationRunning = true;
        m_simulationTimer->start();
        m_pump->start();
    }
}

void MainWindow::stopSimulation()
{
    if (m_simulationRunning) {
        m_simulationRunning = false;
        m_simulationTimer->stop();
        m_pump->stop();
    }
}

void MainWindow::onSimulationTimer()
{
    m_simulationTime += 0.05;

    // Gauge (pressure)
    double gaugeValue = 50 + 30 * qSin(m_simulationTime * 0.5);
    m_gauge->setValue(gaugeValue);

    // Thermometer
    double tempValue = 60 + 20 * qSin(m_simulationTime * 0.3);
    m_thermometer->setTemperature(tempValue);

    // Progress bar
    int progressValue = static_cast<int>(fmod(m_simulationTime * 10, 100));
    m_progressBar->setValue(progressValue);

    // Digital meter
    double powerValue = 50 + 40 * qCos(m_simulationTime * 0.4);
    m_digitalMeter->setValue(powerValue);

    // Seven segment display
    m_sevenSegment->setValue(static_cast<int>(powerValue * 100));

    // Level indicator and tank
    double levelValue = 50 + 40 * qSin(m_simulationTime * 0.2);
    m_levelIndicator->setLevel(levelValue);
    m_tank->setLevel(levelValue);
    m_tank->setTemperature(tempValue);
    m_tank->setPressure(gaugeValue / 50.0);

    // Valve
    double valveOpening = 50 + 50 * qSin(m_simulationTime * 0.35);
    m_valve->setOpening(valveOpening);

    // Pump speed
    if (m_pump->isRunning()) {
        m_pump->setSpeed(50 + 50 * qSin(m_simulationTime * 0.45));
    }

    // Update LEDs based on gauge value
    if (gaugeValue >= 90) {
        m_ledRed->setState(LEDWidget::On);
        m_ledYellow->setState(LEDWidget::Off);
        m_ledGreen->setState(LEDWidget::Off);
    } else if (gaugeValue >= 70) {
        m_ledRed->setState(LEDWidget::Off);
        m_ledYellow->setState(LEDWidget::Flashing);
        m_ledGreen->setState(LEDWidget::Off);
    } else {
        m_ledRed->setState(LEDWidget::Off);
        m_ledYellow->setState(LEDWidget::Off);
        m_ledGreen->setState(LEDWidget::On);
    }

    // Update trend chart
    m_trendChart->addDataPoint("Pressure", gaugeValue);
    m_trendChart->addDataPoint("Temperature", tempValue);
    m_trendChart->addDataPoint("Level", levelValue);

    // Update extended widgets
    if (m_slider) {
        double sliderValue = 50 + 40 * qSin(m_simulationTime * 0.6);
        m_slider->setValue(sliderValue);
    }

    if (m_flowMeter) {
        double flowRate = 30 + 20 * qSin(m_simulationTime * 0.7);
        m_flowMeter->setFlowRate(flowRate);
        // Accumulate total flow
        m_flowMeter->setTotalFlow(m_flowMeter->totalFlow() + flowRate * 0.05 / 60.0);
    }

    if (m_speedometer) {
        double speed = 80 + 60 * qSin(m_simulationTime * 0.55);
        m_speedometer->setValue(speed);
    }

    if (m_barGraph) {
        m_barGraph->setBarValue("Pressure", gaugeValue);
        m_barGraph->setBarValue("Temp", tempValue);
        m_barGraph->setBarValue("Flow", m_flowMeter ? m_flowMeter->flowRate() : 50);
        m_barGraph->setBarValue("Level", levelValue);
    }

    // Test alarm generation
    if (m_alarmIndicator && static_cast<int>(m_simulationTime * 10) % 50 == 0) {
        if (gaugeValue > 85) {
            m_alarmIndicator->addAlarm("High Pressure Warning", 
                AlarmIndicatorWidget::Warning);
        }
    }

    // Update compass
    if (m_compass) {
        double compassAngle = fmod(m_simulationTime * 30.0, 360.0);  // Full rotation every 12 seconds
        m_compass->setAngle(compassAngle);
    }

    // Update electrical widgets
    if (m_motorController) {
        double motorRpm = 800 + 400 * qSin(m_simulationTime * 0.4);
        m_motorController->setRPM(motorRpm);
        m_motorController->setTorque(motorRpm * 0.05);
        m_motorController->setPower(motorRpm * 0.05 * motorRpm / 9550.0);
        
        if (motorRpm > 10) {
            m_motorController->setState(MotorControllerWidget::RunningForward);
        }
    }

    if (m_battery) {
        // Simulate battery discharge
        static double batteryLevel = 75.0;
        batteryLevel -= 0.01;  // Slowly discharge
        if (batteryLevel < 10.0) batteryLevel = 100.0;  // Reset when low
        
        m_battery->setChargeLevel(batteryLevel);
        m_battery->setVoltage(12.0 + batteryLevel / 100.0 * 1.2);
        m_battery->setCurrent(5.0 + 2.0 * qSin(m_simulationTime * 0.5));
    }

    if (m_threePhaseMeter) {
        // Simulate slight variations in three-phase power
        double baseVoltage = 220.0;
        m_threePhaseMeter->setVoltages(
            baseVoltage + 5 * qSin(m_simulationTime * 0.6),
            baseVoltage + 5 * qSin(m_simulationTime * 0.6 + 2.09),  // 120 deg phase shift
            baseVoltage + 5 * qSin(m_simulationTime * 0.6 + 4.19)   // 240 deg phase shift
        );
        
        double baseCurrent = 10.0;
        m_threePhaseMeter->setCurrents(
            baseCurrent + 2 * qSin(m_simulationTime * 0.5),
            baseCurrent + 2 * qSin(m_simulationTime * 0.5 + 2.09),
            baseCurrent + 2 * qSin(m_simulationTime * 0.5 + 4.19)
        );
        
        m_threePhaseMeter->setFrequency(50.0 + 0.5 * qSin(m_simulationTime * 0.3));
        m_threePhaseMeter->setPowerFactor(0.90 + 0.08 * qSin(m_simulationTime * 0.25));
    }
}

void MainWindow::onKnobValueChanged(double value)
{
    // Use knob to control gauge
    m_gauge->setValue(value);
}

void MainWindow::onSwitchToggled(bool checked)
{
    if (checked) {
        m_ledGreen->setState(LEDWidget::On);
    } else {
        m_ledGreen->setState(LEDWidget::Off);
    }
}

void MainWindow::createExtendedTab()
{
    QWidget *extendedTab = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(extendedTab);

    // Row 0: Slider and FlowMeter
    QGroupBox *sliderGroup = new QGroupBox(tr("Industrial Slider"), extendedTab);
    QVBoxLayout *sliderLayout = new QVBoxLayout(sliderGroup);
    m_slider = new IndustrialSliderWidget(extendedTab);
    m_slider->setRange(0, 100);
    m_slider->setTickInterval(10);
    sliderLayout->addWidget(m_slider, 0, Qt::AlignCenter);
    
    QLabel *sliderInfo = new QLabel(tr("Click and drag to adjust"), extendedTab);
    sliderInfo->setAlignment(Qt::AlignCenter);
    sliderLayout->addWidget(sliderInfo);
    
    gridLayout->addWidget(sliderGroup, 0, 0);

    QGroupBox *flowGroup = new QGroupBox(tr("Flow Meter"), extendedTab);
    QVBoxLayout *flowLayout = new QVBoxLayout(flowGroup);
    m_flowMeter = new FlowMeterWidget(extendedTab);
    m_flowMeter->setUnit("L/min");
    flowLayout->addWidget(m_flowMeter);
    
    // Add manual control for flow meter
    QLabel *flowLabel = new QLabel(tr("Manual Control:"), extendedTab);
    flowLayout->addWidget(flowLabel);
    QSlider *flowSlider = new QSlider(Qt::Horizontal, extendedTab);
    flowSlider->setRange(0, 100);
    flowSlider->setValue(0);
    connect(flowSlider, &QSlider::valueChanged, [this](int value) {
        m_flowMeter->setFlowRate(value);
        // Simulate accumulation
        static double lastValue = 0;
        m_flowMeter->setTotalFlow(m_flowMeter->totalFlow() + (value - lastValue) * 0.01);
        lastValue = value;
    });
    flowLayout->addWidget(flowSlider);
    
    QPushButton *resetFlowBtn = new QPushButton(tr("Reset Total"), extendedTab);
    connect(resetFlowBtn, &QPushButton::clicked, [this]() {
        m_flowMeter->resetTotal();
    });
    flowLayout->addWidget(resetFlowBtn);
    
    gridLayout->addWidget(flowGroup, 0, 1);

    // Row 0: Speedometer
    QGroupBox *speedGroup = new QGroupBox(tr("Speedometer"), extendedTab);
    QVBoxLayout *speedLayout = new QVBoxLayout(speedGroup);
    m_speedometer = new SpeedometerWidget(extendedTab);
    m_speedometer->setRange(0, 200);
    m_speedometer->setUnit("km/h");
    m_speedometer->setWarningZone(140);
    m_speedometer->setRedlineZone(180);
    speedLayout->addWidget(m_speedometer, 0, Qt::AlignCenter);
    
    // Add manual control for speedometer
    QLabel *speedLabel = new QLabel(tr("Manual Control:"), extendedTab);
    speedLayout->addWidget(speedLabel);
    QSlider *speedSlider = new QSlider(Qt::Horizontal, extendedTab);
    speedSlider->setRange(0, 200);
    speedSlider->setValue(0);
    connect(speedSlider, &QSlider::valueChanged, [this](int value) {
        m_speedometer->setValue(value);
    });
    speedLayout->addWidget(speedSlider);
    
    gridLayout->addWidget(speedGroup, 0, 2);

    // Row 1: Alarm Indicator (spans 2 columns)
    QGroupBox *alarmGroup = new QGroupBox(tr("Alarm Indicator"), extendedTab);
    QVBoxLayout *alarmLayout = new QVBoxLayout(alarmGroup);
    m_alarmIndicator = new AlarmIndicatorWidget(extendedTab);
    alarmLayout->addWidget(m_alarmIndicator);
    
    QPushButton *addAlarmBtn = new QPushButton(tr("Test Alarm"), extendedTab);
    connect(addAlarmBtn, &QPushButton::clicked, [this]() {
        static int alarmCount = 0;
        AlarmIndicatorWidget::AlarmLevel level = 
            (AlarmIndicatorWidget::AlarmLevel)(alarmCount % 3 + 1);
        m_alarmIndicator->addAlarm(
            QString("Test Alarm %1").arg(++alarmCount), level);
    });
    alarmLayout->addWidget(addAlarmBtn);
    gridLayout->addWidget(alarmGroup, 1, 0, 1, 2);

    // Row 1: Bar Graph
    QGroupBox *barGraphGroup = new QGroupBox(tr("Bar Graph"), extendedTab);
    QVBoxLayout *barGraphLayout = new QVBoxLayout(barGraphGroup);
    m_barGraph = new BarGraphWidget(extendedTab);
    m_barGraph->addBar("Pressure", 75, QColor(255, 100, 100));
    m_barGraph->addBar("Temp", 60, QColor(100, 255, 100));
    m_barGraph->addBar("Flow", 85, QColor(100, 100, 255));
    m_barGraph->addBar("Level", 45, QColor(255, 255, 100));
    barGraphLayout->addWidget(m_barGraph);
    gridLayout->addWidget(barGraphGroup, 1, 2);

    // Row 2: Pipe (spans all columns)
    QGroupBox *pipeGroup = new QGroupBox(tr("Pipe with Flow Animation"), extendedTab);
    QVBoxLayout *pipeLayout = new QVBoxLayout(pipeGroup);
    m_pipe = new PipeWidget(extendedTab);
    m_pipe->setOrientation(PipeWidget::Horizontal);
    m_pipe->setFlowDirection(PipeWidget::Forward);
    m_pipe->setFlowRate(75);
    pipeLayout->addWidget(m_pipe);
    
    QHBoxLayout *pipeControlLayout = new QHBoxLayout();
    QPushButton *flowBtn = new QPushButton(tr("Toggle Flow"), extendedTab);
    connect(flowBtn, &QPushButton::clicked, [this]() {
        if (m_pipe->flowDirection() == PipeWidget::Forward)
            m_pipe->setFlowDirection(PipeWidget::Reverse);
        else
            m_pipe->setFlowDirection(PipeWidget::Forward);
    });
    pipeControlLayout->addWidget(flowBtn);
    pipeLayout->addLayout(pipeControlLayout);
    
    gridLayout->addWidget(pipeGroup, 2, 0, 1, 3);

    m_tabWidget->addTab(extendedTab, tr("Extended Widgets"));
}

void MainWindow::createTimeNavigationTab()
{
    QWidget *timeNavTab = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(timeNavTab);

    // Row 0: Analog Clock
    QGroupBox *analogClockGroup = new QGroupBox(tr("Analog Clock"), timeNavTab);
    QVBoxLayout *analogLayout = new QVBoxLayout(analogClockGroup);
    m_analogClock = new AnalogClockWidget(timeNavTab);
    analogLayout->addWidget(m_analogClock, 0, Qt::AlignCenter);
    analogLayout->addStretch();
    
    gridLayout->addWidget(analogClockGroup, 0, 0);

    // Row 0: Digital Clock
    QGroupBox *digitalClockGroup = new QGroupBox(tr("Digital Clock"), timeNavTab);
    QVBoxLayout *digitalLayout = new QVBoxLayout(digitalClockGroup);
    m_digitalClock = new DigitalClockWidget(timeNavTab);
    digitalLayout->addWidget(m_digitalClock);
    gridLayout->addWidget(digitalClockGroup, 0, 1);

    // Row 1: Timer/Stopwatch
    QGroupBox *timerGroup = new QGroupBox(tr("Timer / Stopwatch"), timeNavTab);
    QVBoxLayout *timerLayout = new QVBoxLayout(timerGroup);
    m_timer = new TimerWidget(timeNavTab);
    timerLayout->addWidget(m_timer);
    
    QHBoxLayout *timerControlLayout = new QHBoxLayout();
    QPushButton *startTimerBtn = new QPushButton(tr("Start"), timeNavTab);
    QPushButton *stopTimerBtn = new QPushButton(tr("Stop"), timeNavTab);
    QPushButton *resetTimerBtn = new QPushButton(tr("Reset"), timeNavTab);
    QPushButton *modeBtn = new QPushButton(tr("Mode: Stopwatch"), timeNavTab);
    
    connect(startTimerBtn, &QPushButton::clicked, [this]() { m_timer->start(); });
    connect(stopTimerBtn, &QPushButton::clicked, [this]() { m_timer->stop(); });
    connect(resetTimerBtn, &QPushButton::clicked, [this]() { m_timer->reset(); });
    connect(modeBtn, &QPushButton::clicked, [this, modeBtn]() {
        if (m_timer->mode() == TimerWidget::Stopwatch) {
            m_timer->setMode(TimerWidget::Countdown);
            m_timer->setTargetSeconds(60);
            modeBtn->setText(tr("Mode: Countdown"));
        } else {
            m_timer->setMode(TimerWidget::Stopwatch);
            modeBtn->setText(tr("Mode: Stopwatch"));
        }
    });
    
    timerControlLayout->addWidget(modeBtn);
    timerControlLayout->addWidget(startTimerBtn);
    timerControlLayout->addWidget(stopTimerBtn);
    timerControlLayout->addWidget(resetTimerBtn);
    timerLayout->addLayout(timerControlLayout);
    
    gridLayout->addWidget(timerGroup, 1, 0);

    // Row 1: Compass
    QGroupBox *compassGroup = new QGroupBox(tr("Compass / Direction Indicator"), timeNavTab);
    QVBoxLayout *compassLayout = new QVBoxLayout(compassGroup);
    m_compass = new CompassWidget(timeNavTab);
    compassLayout->addWidget(m_compass, 0, Qt::AlignCenter);
    
    QLabel *compassLabel = new QLabel(tr("Manual Control (0-359бу):"), timeNavTab);
    compassLayout->addWidget(compassLabel);
    QSlider *compassSlider = new QSlider(Qt::Horizontal, timeNavTab);
    compassSlider->setRange(0, 359);  // 0-359 degrees (360 positions)
    compassSlider->setValue(0);
    connect(compassSlider, &QSlider::valueChanged, [this](int value) {
        m_compass->setAngle(value);
    });
    compassLayout->addWidget(compassSlider);
    
    gridLayout->addWidget(compassGroup, 1, 1);

    m_tabWidget->addTab(timeNavTab, tr("Time && Navigation"));
}

void MainWindow::createElectricalTab()
{
    QWidget *electricalTab = new QWidget();
    QHBoxLayout *tabLayout = new QHBoxLayout(electricalTab);

    // Motor Controller
    QGroupBox *motorGroup = new QGroupBox(tr("Motor Controller"), electricalTab);
    QVBoxLayout *motorLayout = new QVBoxLayout(motorGroup);
    m_motorController = new MotorControllerWidget(electricalTab);
    motorLayout->addWidget(m_motorController);
    
    QHBoxLayout *motorControlLayout = new QHBoxLayout();
    QPushButton *motorFwdBtn = new QPushButton(tr("Forward"), electricalTab);
    QPushButton *motorRevBtn = new QPushButton(tr("Reverse"), electricalTab);
    QPushButton *motorStopBtn = new QPushButton(tr("Stop"), electricalTab);
    
    connect(motorFwdBtn, &QPushButton::clicked, [this]() {
        m_motorController->setState(MotorControllerWidget::RunningForward);
        m_motorController->setRPM(1200);
    });
    connect(motorRevBtn, &QPushButton::clicked, [this]() {
        m_motorController->setState(MotorControllerWidget::RunningReverse);
        m_motorController->setRPM(1000);
    });
    connect(motorStopBtn, &QPushButton::clicked, [this]() {
        m_motorController->setState(MotorControllerWidget::Stopped);
        m_motorController->setRPM(0);
    });
    
    motorControlLayout->addWidget(motorFwdBtn);
    motorControlLayout->addWidget(motorRevBtn);
    motorControlLayout->addWidget(motorStopBtn);
    motorLayout->addLayout(motorControlLayout);
    
    QLabel *motorRpmLabel = new QLabel(tr("RPM Control:"), electricalTab);
    motorLayout->addWidget(motorRpmLabel);
    QSlider *motorRpmSlider = new QSlider(Qt::Horizontal, electricalTab);
    motorRpmSlider->setRange(0, 1500);
    motorRpmSlider->setValue(0);
    connect(motorRpmSlider, &QSlider::valueChanged, [this](int value) {
        m_motorController->setRPM(value);
        // Calculate torque and power
        double torque = value * 0.05;  // Simplified calculation
        double power = (value * torque) / 9550.0;  // kW
        m_motorController->setTorque(torque);
        m_motorController->setPower(power);
    });
    motorLayout->addWidget(motorRpmSlider);
    
    tabLayout->addWidget(motorGroup);

    // Battery Indicator
    QGroupBox *batteryGroup = new QGroupBox(tr("Battery Status"), electricalTab);
    QVBoxLayout *batteryLayout = new QVBoxLayout(batteryGroup);
    m_battery = new BatteryIndicatorWidget(electricalTab);
    batteryLayout->addWidget(m_battery);
    
    QLabel *chargeLabel = new QLabel(tr("Charge Level:"), electricalTab);
    batteryLayout->addWidget(chargeLabel);
    QSlider *chargeSlider = new QSlider(Qt::Horizontal, electricalTab);
    chargeSlider->setRange(0, 100);
    chargeSlider->setValue(75);
    connect(chargeSlider, &QSlider::valueChanged, [this](int value) {
        m_battery->setChargeLevel(value);
    });
    batteryLayout->addWidget(chargeSlider);
    
    QHBoxLayout *batteryStateLayout = new QHBoxLayout();
    QPushButton *dischargingBtn = new QPushButton(tr("Discharging"), electricalTab);
    QPushButton *chargingBtn = new QPushButton(tr("Charging"), electricalTab);
    
    connect(dischargingBtn, &QPushButton::clicked, [this]() {
        m_battery->setChargingState(BatteryIndicatorWidget::Discharging);
        m_battery->setCurrent(5.0);
    });
    connect(chargingBtn, &QPushButton::clicked, [this]() {
        m_battery->setChargingState(BatteryIndicatorWidget::Charging);
        m_battery->setCurrent(-2.0);
    });
    
    batteryStateLayout->addWidget(dischargingBtn);
    batteryStateLayout->addWidget(chargingBtn);
    batteryLayout->addLayout(batteryStateLayout);
    
    tabLayout->addWidget(batteryGroup);

    // Three Phase Meter
    QGroupBox *threephaseGroup = new QGroupBox(tr("Three Phase Meter"), electricalTab);
    QVBoxLayout *threephaseLayout = new QVBoxLayout(threephaseGroup);
    m_threePhaseMeter = new ThreePhaseMeterWidget(electricalTab);
    threephaseLayout->addWidget(m_threePhaseMeter);
    
    QLabel *phaseInfo = new QLabel(tr("Displays A/B/C phase voltage and current"), electricalTab);
    phaseInfo->setWordWrap(true);
    phaseInfo->setAlignment(Qt::AlignCenter);
    threephaseLayout->addWidget(phaseInfo);
    
    tabLayout->addWidget(threephaseGroup);

    m_tabWidget->addTab(electricalTab, tr("Electrical && Energy"));
}

void MainWindow::createMechanicalTab()
{
    QWidget *mechanicalTab = new QWidget();
    QGridLayout *tabLayout = new QGridLayout(mechanicalTab);

    // Gear Train
    QGroupBox *gearGroup = new QGroupBox(tr("Gear Train"), mechanicalTab);
    QVBoxLayout *gearLayout = new QVBoxLayout(gearGroup);
    m_gear = new GearWidget(mechanicalTab);
    gearLayout->addWidget(m_gear, 1);
    
    QHBoxLayout *gearControlLayout = new QHBoxLayout();
    QPushButton *gearStartBtn = new QPushButton(tr("Start"), mechanicalTab);
    QPushButton *gearStopBtn = new QPushButton(tr("Stop"), mechanicalTab);
    
    connect(gearStartBtn, &QPushButton::clicked, this, [this]() {
        m_gear->setRPM(300);
        m_gear->setRunning(true);
    });
    connect(gearStopBtn, &QPushButton::clicked, this, [this]() {
        m_gear->setRunning(false);
    });
    
    gearControlLayout->addWidget(gearStartBtn);
    gearControlLayout->addWidget(gearStopBtn);
    gearLayout->addLayout(gearControlLayout);
    
    // Initialize with default stopped state
    m_gear->setRPM(0);
    m_gear->setRunning(false);
    
    tabLayout->addWidget(gearGroup, 0, 0);

    // Belt Drive
    QGroupBox *beltGroup = new QGroupBox(tr("Belt Drive"), mechanicalTab);
    QVBoxLayout *beltLayout = new QVBoxLayout(beltGroup);
    m_beltDrive = new BeltDriveWidget(mechanicalTab);
    beltLayout->addWidget(m_beltDrive, 1);
    
    QHBoxLayout *beltControlLayout = new QHBoxLayout();
    QPushButton *beltStartBtn = new QPushButton(tr("Start"), mechanicalTab);
    QPushButton *beltStopBtn = new QPushButton(tr("Stop"), mechanicalTab);
    
    connect(beltStartBtn, &QPushButton::clicked, [this]() {
        m_beltDrive->setRPM(400);
        m_beltDrive->setRunning(true);
    });
    connect(beltStopBtn, &QPushButton::clicked, [this]() {
        m_beltDrive->setRunning(false);
    });
    
    beltControlLayout->addWidget(beltStartBtn);
    beltControlLayout->addWidget(beltStopBtn);
    beltLayout->addLayout(beltControlLayout);
    
    // Initialize with default stopped state
    m_beltDrive->setRPM(0);
    m_beltDrive->setRunning(false);
    
    tabLayout->addWidget(beltGroup, 0, 1);

    // Piston Engine
    QGroupBox *pistonGroup = new QGroupBox(tr("Piston Engine"), mechanicalTab);
    QVBoxLayout *pistonLayout = new QVBoxLayout(pistonGroup);
    m_piston = new PistonWidget(mechanicalTab);
    pistonLayout->addWidget(m_piston, 1);
    
    QHBoxLayout *pistonControlLayout = new QHBoxLayout();
    QPushButton *pistonStartBtn = new QPushButton(tr("Start"), mechanicalTab);
    QPushButton *pistonStopBtn = new QPushButton(tr("Stop"), mechanicalTab);
    
    connect(pistonStartBtn, &QPushButton::clicked, [this]() {
        m_piston->setRPM(600);
        m_piston->setPressure(30.0);
        m_piston->setRunning(true);
    });
    connect(pistonStopBtn, &QPushButton::clicked, [this]() {
        m_piston->setRunning(false);
    });
    
    pistonControlLayout->addWidget(pistonStartBtn);
    pistonControlLayout->addWidget(pistonStopBtn);
    pistonLayout->addLayout(pistonControlLayout);
    
    // Initialize with default stopped state
    m_piston->setRPM(0);
    m_piston->setPressure(0.0);
    m_piston->setRunning(false);
    
    tabLayout->addWidget(pistonGroup, 1, 0);

    // Pendulum
    QGroupBox *pendulumGroup = new QGroupBox(tr("Pendulum"), mechanicalTab);
    QVBoxLayout *pendulumLayout = new QVBoxLayout(pendulumGroup);
    m_pendulum = new PendulumWidget(mechanicalTab);
    pendulumLayout->addWidget(m_pendulum);
    
    QHBoxLayout *pendulumControlLayout = new QHBoxLayout();
    QPushButton *pendulumStartBtn = new QPushButton(tr("Start Swing"), mechanicalTab);
    QPushButton *pendulumStopBtn = new QPushButton(tr("Stop"), mechanicalTab);
    
    connect(pendulumStartBtn, &QPushButton::clicked, [this]() {
        m_pendulum->setSwinging(true);
    });
    connect(pendulumStopBtn, &QPushButton::clicked, [this]() {
        m_pendulum->setSwinging(false);
    });
    
    pendulumControlLayout->addWidget(pendulumStartBtn);
    pendulumControlLayout->addWidget(pendulumStopBtn);
    pendulumLayout->addLayout(pendulumControlLayout);
    
    QLabel *amplitudeLabel = new QLabel(tr("Amplitude:"), mechanicalTab);
    pendulumLayout->addWidget(amplitudeLabel);
    QSlider *amplitudeSlider = new QSlider(Qt::Horizontal, mechanicalTab);
    amplitudeSlider->setRange(10, 85);
    amplitudeSlider->setValue(45);
    connect(amplitudeSlider, &QSlider::valueChanged, [this](int value) {
        m_pendulum->setAmplitude(value);
    });
    pendulumLayout->addWidget(amplitudeSlider);
    
    QLabel *dampingLabel = new QLabel(tr("Damping:"), mechanicalTab);
    pendulumLayout->addWidget(dampingLabel);
    QSlider *dampingSlider = new QSlider(Qt::Horizontal, mechanicalTab);
    dampingSlider->setRange(0, 100);
    dampingSlider->setValue(10);
    connect(dampingSlider, &QSlider::valueChanged, [this](int value) {
        m_pendulum->setDamping(value / 100.0);
    });
    pendulumLayout->addWidget(dampingSlider);
    
    // Initialize pendulum with default stopped state
    m_pendulum->setAmplitude(45);
    m_pendulum->setDamping(0.1);
    m_pendulum->setSwinging(false);
    
    tabLayout->addWidget(pendulumGroup, 1, 1);

    m_tabWidget->addTab(mechanicalTab, tr("Mechanical Components"));
}
