#include "ComponentDemoWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QDialog>
#include <QSlider>
#include <QTimer>
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QtMath>
#include <random>
#include <ctime>

// Include all component headers
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
#include "SpringWidget.h"
#include "SliderCrankWidget.h"
#include "CamWidget.h"
#include "WaveformChart.h"
#include "XYGraph.h"
#include "IntensityChart.h"
#include "PieChart.h"
#include "LineChart.h"
#include "Histogram.h"
#include "PolarPlot.h"
#include "SpectrumAnalyzer.h"
#include "ScatterPlot.h"
#include "AreaChart.h"
#include "RadarChart.h"
#include "BubbleChart.h"
#include "DigitalWaveform.h"
#include "StackedBarChart.h"

ComponentDemoWindow::ComponentDemoWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Component Demo - Customer Presentation"));
    resize(1200, 800);
    
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    setupUi();
    
    // Apply modern dark theme
    setStyleSheet(
        "QMainWindow { background-color: #1e1e1e; }"
        "QWidget { background-color: #1e1e1e; color: #ffffff; }"
        "QPushButton { "
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4a4a4a, stop:1 #3a3a3a); "
        "    border: 1px solid #555555; "
        "    border-radius: 4px; "
        "    padding: 4px 8px; "
        "    font-size: 11px; "
        "    font-weight: bold; "
        "    min-width: 80px; "
        "    min-height: 26px; "
        "}"
        "QPushButton:hover { "
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5a5a5a, stop:1 #4a4a4a); "
        "    border: 2px solid #00c800; "
        "}"
        "QPushButton:pressed { "
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3a3a3a, stop:1 #2a2a2a); "
        "}"
        "QScrollArea { border: none; }"
        "QLabel#titleLabel { "
        "    font-size: 22px; "
        "    font-weight: bold; "
        "    color: #00c800; "
        "    padding: 15px; "
        "}"
        "QLabel#categoryLabel { "
        "    font-size: 16px; "
        "    font-weight: bold; "
        "    color: #00aaff; "
        "    padding: 8px; "
        "    margin-top: 10px; "
        "}"
    );
}

ComponentDemoWindow::~ComponentDemoWindow()
{
    // Clean up any open dialogs
    for (auto dialog : m_openDialogs.values()) {
        if (dialog) {
            delete dialog;
        }
    }
}

void ComponentDemoWindow::setupUi()
{
    // Create menu bar
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    QMenu *fileMenu = menuBar->addMenu(tr("&File"));
    QAction *exitAction = fileMenu->addAction(tr("E&xit"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
    
    QMenu *helpMenu = menuBar->addMenu(tr("&Help"));
    QAction *aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, &QAction::triggered, [this]() {
        // About dialog could be added here
    });
    
    // Main widget
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(m_centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Title
    QLabel *titleLabel = new QLabel(tr("Industrial Instrument Control Components Demo"), this);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    QLabel *subtitleLabel = new QLabel(
        tr("Click any button below to open a live demo of the component"), this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 14px; color: #aaaaaa; padding-bottom: 10px;");
    mainLayout->addWidget(subtitleLabel);
    
    // Scroll area for buttons
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    QWidget *scrollWidget = new QWidget();
    m_gridLayout = new QGridLayout(scrollWidget);
    m_gridLayout->setSpacing(15);
    m_gridLayout->setContentsMargins(10, 10, 10, 10);
    
    m_scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(m_scrollArea, 1);
    
    createComponentButtons();
}

void ComponentDemoWindow::createComponentButtons()
{
    int row = 0;
    int col = 0;
    const int maxCols = 5;
    
    // Display Components Category
    QLabel *displayLabel = new QLabel(tr("Display Components"), nullptr);
    displayLabel->setObjectName("categoryLabel");
    m_gridLayout->addWidget(displayLabel, row++, 0, 1, maxCols);
    
    createDemoButton("Gauge", "display", row, col++);
    createDemoButton("Thermometer", "display", row, col++);
    createDemoButton("ProgressBar", "display", row, col++);
    createDemoButton("LED", "display", row, col++);
    createDemoButton("DigitalMeter", "display", row, col++);
    
    col = 0; row++;
    createDemoButton("LevelIndicator", "display", row, col++);
    createDemoButton("SevenSegment", "display", row, col++);
    createDemoButton("BarGraph", "display", row, col++);
    
    // Control Components Category
    col = 0; row++;
    QLabel *controlLabel = new QLabel(tr("Control Components"), nullptr);
    controlLabel->setObjectName("categoryLabel");
    m_gridLayout->addWidget(controlLabel, row++, 0, 1, maxCols);
    
    createDemoButton("SwitchButton", "control", row, col++);
    createDemoButton("Knob", "control", row, col++);
    createDemoButton("IndustrialSlider", "control", row, col++);
    
    // Process Control Category
    col = 0; row++;
    QLabel *processLabel = new QLabel(tr("Process Control"), nullptr);
    processLabel->setObjectName("categoryLabel");
    m_gridLayout->addWidget(processLabel, row++, 0, 1, maxCols);
    
    createDemoButton("Valve", "process", row, col++);
    createDemoButton("Pump", "process", row, col++);
    createDemoButton("Tank", "process", row, col++);
    createDemoButton("Pipe", "process", row, col++);
    createDemoButton("FlowMeter", "process", row, col++);
    
    // Monitoring Category
    col = 0; row++;
    QLabel *monitorLabel = new QLabel(tr("Monitoring & Analysis"), nullptr);
    monitorLabel->setObjectName("categoryLabel");
    m_gridLayout->addWidget(monitorLabel, row++, 0, 1, maxCols);
    
    createDemoButton("TrendChart", "monitor", row, col++);
    createDemoButton("Speedometer", "monitor", row, col++);
    createDemoButton("AlarmIndicator", "monitor", row, col++);
    
    // Time & Navigation Category
    col = 0; row++;
    QLabel *timeLabel = new QLabel(tr("Time & Navigation"), nullptr);
    timeLabel->setObjectName("categoryLabel");
    m_gridLayout->addWidget(timeLabel, row++, 0, 1, maxCols);
    
    createDemoButton("AnalogClock", "time", row, col++);
    createDemoButton("DigitalClock", "time", row, col++);
    createDemoButton("TimerWidget", "time", row, col++);
    createDemoButton("Compass", "time", row, col++);
    
    // Electrical & Energy Category
    col = 0; row++;
    QLabel *electricalLabel = new QLabel(tr("Electrical & Energy"), nullptr);
    electricalLabel->setObjectName("categoryLabel");
    m_gridLayout->addWidget(electricalLabel, row++, 0, 1, maxCols);
    
    createDemoButton("MotorController", "electrical", row, col++);
    createDemoButton("BatteryIndicator", "electrical", row, col++);
    createDemoButton("ThreePhaseMeter", "electrical", row, col++);
    
    // Mechanical Components Category
    col = 0; row++;
    QLabel *mechanicalLabel = new QLabel(tr("Mechanical Components"), nullptr);
    mechanicalLabel->setObjectName("categoryLabel");
    m_gridLayout->addWidget(mechanicalLabel, row++, 0, 1, maxCols);
    
    createDemoButton("Gear", "mechanical", row, col++);
    createDemoButton("BeltDrive", "mechanical", row, col++);
    createDemoButton("Piston", "mechanical", row, col++);
    createDemoButton("Pendulum", "mechanical", row, col++);
    createDemoButton("Spring", "mechanical", row, col++);
    
    col = 0; row++;
    createDemoButton("SliderCrank", "mechanical", row, col++);
    createDemoButton("Cam", "mechanical", row, col++);
    
    // Chart Components Category
    col = 0; row++;
    QLabel *chartLabel = new QLabel(tr("Chart & Graph Components"), nullptr);
    chartLabel->setObjectName("categoryLabel");
    m_gridLayout->addWidget(chartLabel, row++, 0, 1, maxCols);
    
    createDemoButton("WaveformChart", "chart", row, col++);
    createDemoButton("XYGraph", "chart", row, col++);
    createDemoButton("IntensityChart", "chart", row, col++);
    createDemoButton("PieChart", "chart", row, col++);
    createDemoButton("LineChart", "chart", row, col++);
    
    col = 0; row++;
    createDemoButton("Histogram", "chart", row, col++);
    createDemoButton("PolarPlot", "chart", row, col++);
    createDemoButton("SpectrumAnalyzer", "chart", row, col++);
    createDemoButton("ScatterPlot", "chart", row, col++);
    createDemoButton("AreaChart", "chart", row, col++);
    
    col = 0; row++;
    createDemoButton("RadarChart", "chart", row, col++);
    createDemoButton("BubbleChart", "chart", row, col++);
    createDemoButton("DigitalWaveform", "chart", row, col++);
    createDemoButton("StackedBarChart", "chart", row, col++);
    
    // Add stretch at the end
    m_gridLayout->setRowStretch(row + 1, 1);
}

QPushButton* ComponentDemoWindow::createDemoButton(const QString &name, 
                                                    const QString &category, 
                                                    int row, int col)
{
    QPushButton *btn = new QPushButton(name, nullptr);
    btn->setProperty("componentName", name);
    btn->setProperty("category", category);
    
    connect(btn, &QPushButton::clicked, [this, name]() {
        showComponentDemo(name);
    });
    
    m_gridLayout->addWidget(btn, row, col);
    return btn;
}

void ComponentDemoWindow::showComponentDemo(const QString &componentName)
{
    // Check if dialog already exists
    if (m_openDialogs.contains(componentName)) {
        QDialog *dialog = m_openDialogs[componentName];
        if (dialog) {
            dialog->raise();
            dialog->activateWindow();
            return;
        }
    }
    
    // Create appropriate demo dialog
    QDialog *dialog = nullptr;
    
    // Display components
    if (componentName == "Gauge") dialog = createGaugeDemo();
    else if (componentName == "Thermometer") dialog = createThermometerDemo();
    else if (componentName == "ProgressBar") dialog = createProgressBarDemo();
    else if (componentName == "LED") dialog = createLEDDemo();
    else if (componentName == "DigitalMeter") dialog = createDigitalMeterDemo();
    else if (componentName == "LevelIndicator") dialog = createLevelIndicatorDemo();
    else if (componentName == "SevenSegment") dialog = createSevenSegmentDemo();
    else if (componentName == "BarGraph") dialog = createBarGraphDemo();
    
    // Control components
    else if (componentName == "SwitchButton") dialog = createSwitchButtonDemo();
    else if (componentName == "Knob") dialog = createKnobDemo();
    else if (componentName == "IndustrialSlider") dialog = createIndustrialSliderDemo();
    
    // Process control
    else if (componentName == "Valve") dialog = createValveDemo();
    else if (componentName == "Pump") dialog = createPumpDemo();
    else if (componentName == "Tank") dialog = createTankDemo();
    else if (componentName == "Pipe") dialog = createPipeDemo();
    else if (componentName == "FlowMeter") dialog = createFlowMeterDemo();
    
    // Monitoring
    else if (componentName == "TrendChart") dialog = createTrendChartDemo();
    else if (componentName == "Speedometer") dialog = createSpeedometerDemo();
    else if (componentName == "AlarmIndicator") dialog = createAlarmIndicatorDemo();
    
    // Time & Navigation
    else if (componentName == "AnalogClock") dialog = createAnalogClockDemo();
    else if (componentName == "DigitalClock") dialog = createDigitalClockDemo();
    else if (componentName == "TimerWidget") dialog = createTimerWidgetDemo();
    else if (componentName == "Compass") dialog = createCompassDemo();
    
    // Electrical & Energy
    else if (componentName == "MotorController") dialog = createMotorControllerDemo();
    else if (componentName == "BatteryIndicator") dialog = createBatteryIndicatorDemo();
    else if (componentName == "ThreePhaseMeter") dialog = createThreePhaseMeterDemo();
    
    // Mechanical
    else if (componentName == "Gear") dialog = createGearDemo();
    else if (componentName == "BeltDrive") dialog = createBeltDriveDemo();
    else if (componentName == "Piston") dialog = createPistonDemo();
    else if (componentName == "Pendulum") dialog = createPendulumDemo();
    else if (componentName == "Spring") dialog = createSpringDemo();
    else if (componentName == "SliderCrank") dialog = createSliderCrankDemo();
    else if (componentName == "Cam") dialog = createCamDemo();
    
    // Charts
    else if (componentName == "WaveformChart") dialog = createWaveformChartDemo();
    else if (componentName == "XYGraph") dialog = createXYGraphDemo();
    else if (componentName == "IntensityChart") dialog = createIntensityChartDemo();
    else if (componentName == "PieChart") dialog = createPieChartDemo();
    else if (componentName == "LineChart") dialog = createLineChartDemo();
    else if (componentName == "Histogram") dialog = createHistogramDemo();
    else if (componentName == "PolarPlot") dialog = createPolarPlotDemo();
    else if (componentName == "SpectrumAnalyzer") dialog = createSpectrumAnalyzerDemo();
    else if (componentName == "ScatterPlot") dialog = createScatterPlotDemo();
    else if (componentName == "AreaChart") dialog = createAreaChartDemo();
    else if (componentName == "RadarChart") dialog = createRadarChartDemo();
    else if (componentName == "BubbleChart") dialog = createBubbleChartDemo();
    else if (componentName == "DigitalWaveform") dialog = createDigitalWaveformDemo();
    else if (componentName == "StackedBarChart") dialog = createStackedBarChartDemo();
    
    if (dialog) {
        m_openDialogs[componentName] = dialog;
        
        // Remove from map when closed
        connect(dialog, &QDialog::finished, [this, componentName]() {
            m_openDialogs.remove(componentName);
        });
        
        dialog->show();
        dialog->raise();
        dialog->activateWindow();
    }
}

// Component demo creators implementation

QDialog* ComponentDemoWindow::createGaugeDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Gauge Widget Demo"));
    dialog->resize(500, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    GaugeWidget *gauge = new GaugeWidget(dialog);
    gauge->setRange(0, 100);
    gauge->setUnit("PSI");
    gauge->setWarningRange(70, 90);
    gauge->setValue(45);
    layout->addWidget(gauge, 0, Qt::AlignCenter);
    
    // Control slider
    QLabel *label = new QLabel(tr("Value:"), dialog);
    layout->addWidget(label);
    QSlider *slider = new QSlider(Qt::Horizontal, dialog);
    slider->setRange(0, 100);
    slider->setValue(45);
    connect(slider, &QSlider::valueChanged, [gauge, slider](int value) {
        gauge->setValue(value);
    });
    layout->addWidget(slider);
    
    // Auto animation
    QTimer *timer = new QTimer(dialog);
    double *time = new double(0);
    connect(timer, &QTimer::timeout, [gauge, slider, time]() {
        *time += 0.05;
        double value = 50 + 40 * qSin(*time);
        gauge->setValue(value);
        slider->setValue(static_cast<int>(value));
    });
    
    QPushButton *animateBtn = new QPushButton(tr("Auto Animate"), dialog);
    animateBtn->setCheckable(true);
    animateBtn->setChecked(true);  // Start with animation on
    timer->start(50);  // Auto start animation
    animateBtn->setText(tr("Stop Animation"));
    connect(animateBtn, &QPushButton::toggled, [timer, animateBtn](bool checked) {
        if (checked) {
            timer->start(50);
            animateBtn->setText(tr("Stop Animation"));
        } else {
            timer->stop();
            animateBtn->setText(tr("Auto Animate"));
        }
    });
    layout->addWidget(animateBtn);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createThermometerDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Thermometer Widget Demo"));
    dialog->resize(400, 500);
    
    QHBoxLayout *layout = new QHBoxLayout(dialog);
    
    ThermometerWidget *thermometer = new ThermometerWidget(dialog);
    thermometer->setRange(-20, 120);
    thermometer->setWarningRange(80, 100);
    thermometer->setTemperature(25);
    layout->addWidget(thermometer, 0, Qt::AlignCenter);
    
    QVBoxLayout *controlLayout = new QVBoxLayout();
    QLabel *label = new QLabel(tr("Temperature:"), dialog);
    controlLayout->addWidget(label);
    
    QSlider *slider = new QSlider(Qt::Vertical, dialog);
    slider->setRange(-20, 120);
    slider->setValue(25);
    connect(slider, &QSlider::valueChanged, [thermometer](int value) {
        thermometer->setTemperature(value);
    });
    controlLayout->addWidget(slider, 1);
    
    // Auto animation
    QTimer *timer = new QTimer(dialog);
    double *time = new double(0);
    connect(timer, &QTimer::timeout, [thermometer, slider, time]() {
        *time += 0.03;
        double temp = 60 + 30 * qSin(*time);
        thermometer->setTemperature(temp);
        slider->setValue(static_cast<int>(temp));
    });
    timer->start(50);  // Auto start animation
    
    layout->addLayout(controlLayout);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createProgressBarDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Progress Bar Demo"));
    dialog->resize(500, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    IndustrialProgressBar *progressBar = new IndustrialProgressBar(dialog);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    layout->addWidget(progressBar);
    
    QSlider *slider = new QSlider(Qt::Horizontal, dialog);
    slider->setRange(0, 100);
    connect(slider, &QSlider::valueChanged, [progressBar](int value) {
        progressBar->setValue(value);
    });
    layout->addWidget(slider);
    
    QPushButton *animateBtn = new QPushButton(tr("Animate Progress"), dialog);
    layout->addWidget(animateBtn);
    
    QTimer *timer = new QTimer(dialog);
    int *value = new int(0);
    connect(timer, &QTimer::timeout, [progressBar, value, timer]() {
        *value = (*value + 1) % 101;
        progressBar->setValue(*value);
        if (*value == 100) timer->stop();
    });
    
    connect(animateBtn, &QPushButton::clicked, [timer, value]() {
        *value = 0;
        timer->start(30);
    });
    
    return dialog;
}

QDialog* ComponentDemoWindow::createLEDDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("LED Indicator Demo"));
    dialog->resize(500, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    QHBoxLayout *ledLayout = new QHBoxLayout();
    
    LEDWidget *redLED = new LEDWidget(dialog);
    redLED->setColor(LEDWidget::Red);
    redLED->setState(LEDWidget::Off);
    
    LEDWidget *yellowLED = new LEDWidget(dialog);
    yellowLED->setColor(LEDWidget::Yellow);
    yellowLED->setState(LEDWidget::Flashing);  // Start with flashing
    
    LEDWidget *greenLED = new LEDWidget(dialog);
    greenLED->setColor(LEDWidget::Green);
    greenLED->setState(LEDWidget::On);
    
    ledLayout->addWidget(redLED);
    ledLayout->addWidget(yellowLED);
    ledLayout->addWidget(greenLED);
    layout->addLayout(ledLayout);
    
    QPushButton *redBtn = new QPushButton(tr("Red On"), dialog);
    redBtn->setCheckable(true);
    connect(redBtn, &QPushButton::toggled, [redLED](bool checked) {
        redLED->setState(checked ? LEDWidget::On : LEDWidget::Off);
    });
    layout->addWidget(redBtn);
    
    QPushButton *yellowBtn = new QPushButton(tr("Yellow Flash"), dialog);
    yellowBtn->setCheckable(true);
    yellowBtn->setChecked(true);  // Start checked
    connect(yellowBtn, &QPushButton::toggled, [yellowLED](bool checked) {
        yellowLED->setState(checked ? LEDWidget::Flashing : LEDWidget::Off);
    });
    layout->addWidget(yellowBtn);
    
    QPushButton *greenBtn = new QPushButton(tr("Green On"), dialog);
    greenBtn->setCheckable(true);
    greenBtn->setChecked(true);
    connect(greenBtn, &QPushButton::toggled, [greenLED](bool checked) {
        greenLED->setState(checked ? LEDWidget::On : LEDWidget::Off);
    });
    layout->addWidget(greenBtn);
    
    // Auto sequence animation
    QTimer *timer = new QTimer(dialog);
    int *step = new int(0);
    connect(timer, &QTimer::timeout, [redLED, yellowLED, greenLED, step]() {
        *step = (*step + 1) % 6;
        switch (*step) {
            case 0:
                redLED->setState(LEDWidget::Off);
                yellowLED->setState(LEDWidget::Off);
                greenLED->setState(LEDWidget::On);
                break;
            case 2:
                greenLED->setState(LEDWidget::Off);
                yellowLED->setState(LEDWidget::Flashing);
                break;
            case 4:
                yellowLED->setState(LEDWidget::Off);
                redLED->setState(LEDWidget::On);
                break;
        }
    });
    timer->start(1000);  // Auto start traffic light sequence
    
    return dialog;
}

QDialog* ComponentDemoWindow::createDigitalMeterDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Digital Meter Demo"));
    dialog->resize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    DigitalMeterWidget *meter = new DigitalMeterWidget(dialog);
    meter->setLabel(tr("Power Output"));
    meter->setUnit("kW");
    meter->setPrecision(2);
    meter->setValue(0);
    layout->addWidget(meter);
    
    QSlider *slider = new QSlider(Qt::Horizontal, dialog);
    slider->setRange(0, 10000);
    connect(slider, &QSlider::valueChanged, [meter](int value) {
        meter->setValue(value / 100.0);
    });
    layout->addWidget(slider);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createLevelIndicatorDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Level Indicator Demo"));
    dialog->resize(400, 500);
    
    QHBoxLayout *layout = new QHBoxLayout(dialog);
    
    LevelIndicatorWidget *level = new LevelIndicatorWidget(dialog);
    level->setRange(0, 100);
    level->setUnit("%");
    level->setLevel(50);
    layout->addWidget(level, 0, Qt::AlignCenter);
    
    QVBoxLayout *controlLayout = new QVBoxLayout();
    QSlider *slider = new QSlider(Qt::Vertical, dialog);
    slider->setRange(0, 100);
    slider->setValue(50);
    connect(slider, &QSlider::valueChanged, [level](int value) {
        level->setLevel(value);
    });
    controlLayout->addWidget(slider, 1);
    
    layout->addLayout(controlLayout);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createSevenSegmentDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Seven Segment Display Demo"));
    dialog->resize(500, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    SevenSegmentWidget *sevenSeg = new SevenSegmentWidget(dialog);
    sevenSeg->setDigitCount(6);
    sevenSeg->setValue(123456);
    layout->addWidget(sevenSeg, 0, Qt::AlignCenter);
    
    QSlider *slider = new QSlider(Qt::Horizontal, dialog);
    slider->setRange(0, 999999);
    slider->setValue(123456);
    connect(slider, &QSlider::valueChanged, [sevenSeg](int value) {
        sevenSeg->setValue(value);
    });
    layout->addWidget(slider);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createBarGraphDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Bar Graph Demo"));
    dialog->resize(600, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    BarGraphWidget *barGraph = new BarGraphWidget(dialog);
    barGraph->addBar("System A", 75, QColor(255, 100, 100));
    barGraph->addBar("System B", 60, QColor(100, 255, 100));
    barGraph->addBar("System C", 85, QColor(100, 100, 255));
    barGraph->addBar("System D", 45, QColor(255, 255, 100));
    layout->addWidget(barGraph);
    
    QTimer *timer = new QTimer(dialog);
    double *time = new double(0);
    connect(timer, &QTimer::timeout, [barGraph, time]() {
        *time += 0.1;
        barGraph->setBarValue("System A", 75 + 20 * qSin(*time));
        barGraph->setBarValue("System B", 60 + 30 * qSin(*time + 1));
        barGraph->setBarValue("System C", 85 + 10 * qSin(*time + 2));
        barGraph->setBarValue("System D", 45 + 40 * qSin(*time + 3));
    });
    
    QPushButton *animateBtn = new QPushButton(tr("Animate"), dialog);
    animateBtn->setCheckable(true);
    connect(animateBtn, &QPushButton::toggled, [timer](bool checked) {
        if (checked) timer->start(100);
        else timer->stop();
    });
    layout->addWidget(animateBtn);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createSwitchButtonDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Switch Button Demo"));
    dialog->resize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    SwitchButtonWidget *switchBtn = new SwitchButtonWidget(dialog);
    layout->addWidget(switchBtn, 0, Qt::AlignCenter);
    
    QLabel *statusLabel = new QLabel(tr("Status: OFF"), dialog);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(statusLabel);
    
    connect(switchBtn, &SwitchButtonWidget::toggled, [statusLabel](bool checked) {
        statusLabel->setText(checked ? tr("Status: ON") : tr("Status: OFF"));
        statusLabel->setStyleSheet(checked ? 
            "font-size: 18px; font-weight: bold; color: #00ff00;" :
            "font-size: 18px; font-weight: bold; color: #ff0000;");
    });
    
    return dialog;
}

QDialog* ComponentDemoWindow::createKnobDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Knob Control Demo"));
    dialog->resize(400, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    KnobWidget *knob = new KnobWidget(dialog);
    knob->setRange(0, 100);
    knob->setStep(1.0);
    knob->setValue(50);
    layout->addWidget(knob, 0, Qt::AlignCenter);
    
    QLabel *valueLabel = new QLabel(tr("Value: 50"), dialog);
    valueLabel->setAlignment(Qt::AlignCenter);
    valueLabel->setStyleSheet("font-size: 16px;");
    layout->addWidget(valueLabel);
    
    connect(knob, &KnobWidget::valueChanged, [valueLabel](double value) {
        valueLabel->setText(tr("Value: %1").arg(value, 0, 'f', 1));
    });
    
    return dialog;
}

QDialog* ComponentDemoWindow::createIndustrialSliderDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Industrial Slider Demo"));
    dialog->resize(400, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    IndustrialSliderWidget *slider = new IndustrialSliderWidget(dialog);
    slider->setRange(0, 100);
    slider->setTickInterval(10);
    slider->setValue(50);
    layout->addWidget(slider, 1, Qt::AlignCenter);
    
    QLabel *valueLabel = new QLabel(tr("Value: 50"), dialog);
    valueLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(valueLabel);
    
    connect(slider, &IndustrialSliderWidget::valueChanged, [valueLabel](double value) {
        valueLabel->setText(tr("Value: %1").arg(value, 0, 'f', 1));
    });
    
    return dialog;
}

QDialog* ComponentDemoWindow::createValveDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Valve Control Demo"));
    dialog->resize(400, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    ValveWidget *valve = new ValveWidget(dialog);
    valve->setType(ValveWidget::BallValve);
    valve->setOpening(0);
    layout->addWidget(valve, 1, Qt::AlignCenter);
    
    QLabel *label = new QLabel(tr("Opening: 0%"), dialog);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    
    QSlider *slider = new QSlider(Qt::Horizontal, dialog);
    slider->setRange(0, 100);
    connect(slider, &QSlider::valueChanged, [valve, label](int value) {
        valve->setOpening(value);
        label->setText(tr("Opening: %1%").arg(value));
    });
    layout->addWidget(slider);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createPumpDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Pump Demo"));
    dialog->resize(400, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    PumpWidget *pump = new PumpWidget(dialog);
    pump->start();  // Auto start
    layout->addWidget(pump, 1, Qt::AlignCenter);
    
    QPushButton *startBtn = new QPushButton(tr("Start Pump"), dialog);
    QPushButton *stopBtn = new QPushButton(tr("Stop Pump"), dialog);
    
    connect(startBtn, &QPushButton::clicked, pump, &PumpWidget::start);
    connect(stopBtn, &QPushButton::clicked, pump, &PumpWidget::stop);
    
    layout->addWidget(startBtn);
    layout->addWidget(stopBtn);
    
    QSlider *speedSlider = new QSlider(Qt::Horizontal, dialog);
    speedSlider->setRange(0, 100);
    speedSlider->setValue(75);
    pump->setSpeed(75);  // Set initial speed
    connect(speedSlider, &QSlider::valueChanged, pump, &PumpWidget::setSpeed);
    
    QLabel *speedLabel = new QLabel(tr("Speed: 75%"), dialog);
    connect(speedSlider, &QSlider::valueChanged, [speedLabel](int value) {
        speedLabel->setText(tr("Speed: %1%").arg(value));
    });
    
    layout->addWidget(speedLabel);
    layout->addWidget(speedSlider);
    
    // Auto speed variation
    QTimer *timer = new QTimer(dialog);
    double *time = new double(0);
    connect(timer, &QTimer::timeout, [pump, speedSlider, speedLabel, time]() {
        *time += 0.05;
        int speed = static_cast<int>(70 + 25 * qSin(*time));
        pump->setSpeed(speed);
        speedSlider->setValue(speed);
        speedLabel->setText(tr("Speed: %1%").arg(speed));
    });
    timer->start(100);  // Auto start speed variation
    
    return dialog;
}

QDialog* ComponentDemoWindow::createTankDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Tank Demo"));
    dialog->resize(400, 600);
    
    QHBoxLayout *layout = new QHBoxLayout(dialog);
    
    TankWidget *tank = new TankWidget(dialog);
    tank->setShape(TankWidget::Cylindrical);
    tank->setCapacity(1000, "L");
    tank->setLevel(50);
    layout->addWidget(tank, 1, Qt::AlignCenter);
    
    QVBoxLayout *controlLayout = new QVBoxLayout();
    
    QLabel *levelLabel = new QLabel(tr("Level:"), dialog);
    controlLayout->addWidget(levelLabel);
    
    QSlider *levelSlider = new QSlider(Qt::Vertical, dialog);
    levelSlider->setRange(0, 100);
    levelSlider->setValue(50);
    connect(levelSlider, &QSlider::valueChanged, [tank](int value) {
        tank->setLevel(value);
    });
    controlLayout->addWidget(levelSlider, 1);
    
    layout->addLayout(controlLayout);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createPipeDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Pipe Flow Demo"));
    dialog->resize(600, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    PipeWidget *pipe = new PipeWidget(dialog);
    pipe->setOrientation(PipeWidget::Horizontal);
    pipe->setFlowDirection(PipeWidget::Forward);
    pipe->setFlowRate(75);
    layout->addWidget(pipe);
    
    QHBoxLayout *controlLayout = new QHBoxLayout();
    
    QPushButton *toggleFlowBtn = new QPushButton(tr("Reverse Flow"), dialog);
    connect(toggleFlowBtn, &QPushButton::clicked, [pipe, toggleFlowBtn]() {
        if (pipe->flowDirection() == PipeWidget::Forward) {
            pipe->setFlowDirection(PipeWidget::Reverse);
            toggleFlowBtn->setText(tr("Forward Flow"));
        } else {
            pipe->setFlowDirection(PipeWidget::Forward);
            toggleFlowBtn->setText(tr("Reverse Flow"));
        }
    });
    controlLayout->addWidget(toggleFlowBtn);
    
    layout->addLayout(controlLayout);
    
    QSlider *flowSlider = new QSlider(Qt::Horizontal, dialog);
    flowSlider->setRange(0, 100);
    flowSlider->setValue(75);
    connect(flowSlider, &QSlider::valueChanged, pipe, &PipeWidget::setFlowRate);
    
    QLabel *flowLabel = new QLabel(tr("Flow Rate: 75%"), dialog);
    connect(flowSlider, &QSlider::valueChanged, [flowLabel](int value) {
        flowLabel->setText(tr("Flow Rate: %1%").arg(value));
    });
    
    layout->addWidget(flowLabel);
    layout->addWidget(flowSlider);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createFlowMeterDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Flow Meter Demo"));
    dialog->resize(500, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    FlowMeterWidget *flowMeter = new FlowMeterWidget(dialog);
    flowMeter->setUnit("L/min");
    flowMeter->setFlowRate(0);
    layout->addWidget(flowMeter);
    
    QSlider *slider = new QSlider(Qt::Horizontal, dialog);
    slider->setRange(0, 100);
    connect(slider, &QSlider::valueChanged, flowMeter, &FlowMeterWidget::setFlowRate);
    layout->addWidget(slider);
    
    QPushButton *resetBtn = new QPushButton(tr("Reset Total"), dialog);
    connect(resetBtn, &QPushButton::clicked, flowMeter, &FlowMeterWidget::resetTotal);
    layout->addWidget(resetBtn);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createTrendChartDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Trend Chart Demo"));
    dialog->resize(800, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    TrendChartWidget *trendChart = new TrendChartWidget(dialog);
    trendChart->setMaxDataPoints(100);
    trendChart->setAutoScale(true);
    trendChart->addSeries("Signal 1", QColor(255, 100, 100));
    trendChart->addSeries("Signal 2", QColor(100, 255, 100));
    trendChart->addSeries("Signal 3", QColor(100, 100, 255));
    layout->addWidget(trendChart);
    
    QTimer *timer = new QTimer(dialog);
    double *time = new double(0);
    connect(timer, &QTimer::timeout, [trendChart, time]() {
        *time += 0.1;
        trendChart->addDataPoint("Signal 1", 50 + 30 * qSin(*time));
        trendChart->addDataPoint("Signal 2", 50 + 30 * qSin(*time + 2));
        trendChart->addDataPoint("Signal 3", 50 + 30 * qSin(*time + 4));
    });
    
    QPushButton *startBtn = new QPushButton(tr("Start Recording"), dialog);
    startBtn->setCheckable(true);
    connect(startBtn, &QPushButton::toggled, [timer, startBtn](bool checked) {
        if (checked) {
            timer->start(100);
            startBtn->setText(tr("Stop Recording"));
        } else {
            timer->stop();
            startBtn->setText(tr("Start Recording"));
        }
    });
    
    QPushButton *clearBtn = new QPushButton(tr("Clear Chart"), dialog);
    connect(clearBtn, &QPushButton::clicked, trendChart, &TrendChartWidget::clearAllSeries);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(startBtn);
    btnLayout->addWidget(clearBtn);
    layout->addLayout(btnLayout);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createSpeedometerDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Speedometer Demo"));
    dialog->resize(500, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    SpeedometerWidget *speedometer = new SpeedometerWidget(dialog);
    speedometer->setRange(0, 200);
    speedometer->setUnit("km/h");
    speedometer->setWarningZone(140);
    speedometer->setRedlineZone(180);
    speedometer->setValue(0);
    layout->addWidget(speedometer, 1, Qt::AlignCenter);
    
    QSlider *slider = new QSlider(Qt::Horizontal, dialog);
    slider->setRange(0, 200);
    connect(slider, &QSlider::valueChanged, [speedometer](int value) {
        speedometer->setValue(value);
    });
    layout->addWidget(slider);
    
    // Auto animation - simulate acceleration and deceleration
    QTimer *timer = new QTimer(dialog);
    double *time = new double(0);
    connect(timer, &QTimer::timeout, [speedometer, slider, time]() {
        *time += 0.05;
        double speed = 100 + 80 * qSin(*time * 0.5);
        speedometer->setValue(speed);
        slider->setValue(static_cast<int>(speed));
    });
    timer->start(50);  // Auto start animation
    
    return dialog;
}

QDialog* ComponentDemoWindow::createAlarmIndicatorDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Alarm Indicator Demo"));
    dialog->resize(600, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    AlarmIndicatorWidget *alarmIndicator = new AlarmIndicatorWidget(dialog);
    layout->addWidget(alarmIndicator);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    QPushButton *criticalBtn = new QPushButton(tr("Add Critical Alarm"), dialog);
    connect(criticalBtn, &QPushButton::clicked, [alarmIndicator]() {
        static int count = 1;
        alarmIndicator->addAlarm(
            QString("Critical Alarm %1").arg(count++), 
            AlarmIndicatorWidget::Critical);
    });
    btnLayout->addWidget(criticalBtn);
    
    QPushButton *warningBtn = new QPushButton(tr("Add Warning"), dialog);
    connect(warningBtn, &QPushButton::clicked, [alarmIndicator]() {
        static int count = 1;
        alarmIndicator->addAlarm(
            QString("Warning %1").arg(count++), 
            AlarmIndicatorWidget::Warning);
    });
    btnLayout->addWidget(warningBtn);
    
    QPushButton *infoBtn = new QPushButton(tr("Add Info"), dialog);
    connect(infoBtn, &QPushButton::clicked, [alarmIndicator]() {
        static int count = 1;
        alarmIndicator->addAlarm(
            QString("Info %1").arg(count++), 
            AlarmIndicatorWidget::Info);
    });
    btnLayout->addWidget(infoBtn);
    
    QPushButton *clearBtn = new QPushButton(tr("Clear All"), dialog);
    connect(clearBtn, &QPushButton::clicked, alarmIndicator, &AlarmIndicatorWidget::clearAllAlarms);
    btnLayout->addWidget(clearBtn);
    
    layout->addLayout(btnLayout);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createAnalogClockDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Analog Clock Demo"));
    dialog->resize(400, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    AnalogClockWidget *clock = new AnalogClockWidget(dialog);
    layout->addWidget(clock, 1, Qt::AlignCenter);
    
    QLabel *label = new QLabel(tr("Real-time system clock display"), dialog);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createDigitalClockDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Digital Clock Demo"));
    dialog->resize(500, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    DigitalClockWidget *clock = new DigitalClockWidget(dialog);
    layout->addWidget(clock);
    
    QLabel *label = new QLabel(tr("Real-time system clock display"), dialog);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createTimerWidgetDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Timer / Stopwatch Demo"));
    dialog->resize(500, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    TimerWidget *timer = new TimerWidget(dialog);
    layout->addWidget(timer);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    QPushButton *startBtn = new QPushButton(tr("Start"), dialog);
    connect(startBtn, &QPushButton::clicked, timer, &TimerWidget::start);
    btnLayout->addWidget(startBtn);
    
    QPushButton *stopBtn = new QPushButton(tr("Stop"), dialog);
    connect(stopBtn, &QPushButton::clicked, timer, &TimerWidget::stop);
    btnLayout->addWidget(stopBtn);
    
    QPushButton *resetBtn = new QPushButton(tr("Reset"), dialog);
    connect(resetBtn, &QPushButton::clicked, timer, &TimerWidget::reset);
    btnLayout->addWidget(resetBtn);
    
    QPushButton *modeBtn = new QPushButton(tr("Mode: Stopwatch"), dialog);
    connect(modeBtn, &QPushButton::clicked, [timer, modeBtn]() {
        if (timer->mode() == TimerWidget::Stopwatch) {
            timer->setMode(TimerWidget::Countdown);
            timer->setTargetSeconds(60);
            modeBtn->setText(tr("Mode: Countdown"));
        } else {
            timer->setMode(TimerWidget::Stopwatch);
            modeBtn->setText(tr("Mode: Stopwatch"));
        }
    });
    btnLayout->addWidget(modeBtn);
    
    layout->addLayout(btnLayout);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createCompassDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Compass Demo"));
    dialog->resize(400, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    CompassWidget *compass = new CompassWidget(dialog);
    compass->setAngle(0);
    layout->addWidget(compass, 1, Qt::AlignCenter);
    
    QLabel *label = new QLabel(tr("Angle: 0 deg"), dialog);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    
    QSlider *slider = new QSlider(Qt::Horizontal, dialog);
    slider->setRange(0, 359);
    connect(slider, &QSlider::valueChanged, [compass, label](int value) {
        compass->setAngle(value);
        label->setText(tr("Angle: %1 deg").arg(value));
    });
    layout->addWidget(slider);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createMotorControllerDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Motor Controller Demo"));
    dialog->resize(500, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    MotorControllerWidget *motor = new MotorControllerWidget(dialog);
    layout->addWidget(motor);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    QPushButton *fwdBtn = new QPushButton(tr("Forward"), dialog);
    connect(fwdBtn, &QPushButton::clicked, [motor]() {
        motor->setState(MotorControllerWidget::RunningForward);
        motor->setRPM(1200);
    });
    btnLayout->addWidget(fwdBtn);
    
    QPushButton *revBtn = new QPushButton(tr("Reverse"), dialog);
    connect(revBtn, &QPushButton::clicked, [motor]() {
        motor->setState(MotorControllerWidget::RunningReverse);
        motor->setRPM(1000);
    });
    btnLayout->addWidget(revBtn);
    
    QPushButton *stopBtn = new QPushButton(tr("Stop"), dialog);
    connect(stopBtn, &QPushButton::clicked, [motor]() {
        motor->setState(MotorControllerWidget::Stopped);
        motor->setRPM(0);
    });
    btnLayout->addWidget(stopBtn);
    
    layout->addLayout(btnLayout);
    
    QLabel *rpmLabel = new QLabel(tr("RPM Control:"), dialog);
    layout->addWidget(rpmLabel);
    
    QSlider *rpmSlider = new QSlider(Qt::Horizontal, dialog);
    rpmSlider->setRange(0, 1500);
    connect(rpmSlider, &QSlider::valueChanged, [motor](int value) {
        motor->setRPM(value);
        double torque = value * 0.05;
        double power = (value * torque) / 9550.0;
        motor->setTorque(torque);
        motor->setPower(power);
    });
    layout->addWidget(rpmSlider);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createBatteryIndicatorDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Battery Indicator Demo"));
    dialog->resize(400, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    BatteryIndicatorWidget *battery = new BatteryIndicatorWidget(dialog);
    battery->setChargeLevel(75);
    battery->setVoltage(12.6);
    battery->setCurrent(2.5);
    layout->addWidget(battery);
    
    QLabel *chargeLabel = new QLabel(tr("Charge Level:"), dialog);
    layout->addWidget(chargeLabel);
    
    QSlider *chargeSlider = new QSlider(Qt::Horizontal, dialog);
    chargeSlider->setRange(0, 100);
    chargeSlider->setValue(75);
    connect(chargeSlider, &QSlider::valueChanged, [battery](int value) {
        battery->setChargeLevel(value);
    });
    layout->addWidget(chargeSlider);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    QPushButton *dischargingBtn = new QPushButton(tr("Discharging"), dialog);
    connect(dischargingBtn, &QPushButton::clicked, [battery]() {
        battery->setChargingState(BatteryIndicatorWidget::Discharging);
        battery->setCurrent(5.0);
    });
    btnLayout->addWidget(dischargingBtn);
    
    QPushButton *chargingBtn = new QPushButton(tr("Charging"), dialog);
    connect(chargingBtn, &QPushButton::clicked, [battery]() {
        battery->setChargingState(BatteryIndicatorWidget::Charging);
        battery->setCurrent(-2.0);
    });
    btnLayout->addWidget(chargingBtn);
    
    layout->addLayout(btnLayout);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createThreePhaseMeterDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Three Phase Meter Demo"));
    dialog->resize(600, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    ThreePhaseMeterWidget *meter = new ThreePhaseMeterWidget(dialog);
    meter->setVoltages(220, 220, 220);
    meter->setCurrents(10, 10, 10);
    meter->setFrequency(50);
    meter->setPowerFactor(0.95);
    layout->addWidget(meter);
    
    QLabel *label = new QLabel(tr("Real-time three-phase power monitoring"), dialog);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    
    // Add animation
    QTimer *timer = new QTimer(dialog);
    double *time = new double(0);
    connect(timer, &QTimer::timeout, [meter, time]() {
        *time += 0.1;
        double baseV = 220;
        meter->setVoltages(
            baseV + 5 * qSin(*time),
            baseV + 5 * qSin(*time + 2.09),
            baseV + 5 * qSin(*time + 4.19)
        );
        
        double baseI = 10;
        meter->setCurrents(
            baseI + 2 * qSin(*time * 0.5),
            baseI + 2 * qSin(*time * 0.5 + 2.09),
            baseI + 2 * qSin(*time * 0.5 + 4.19)
        );
    });
    
    QPushButton *animateBtn = new QPushButton(tr("Animate"), dialog);
    animateBtn->setCheckable(true);
    connect(animateBtn, &QPushButton::toggled, [timer](bool checked) {
        if (checked) timer->start(100);
        else timer->stop();
    });
    layout->addWidget(animateBtn);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createGearDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Gear Train Demo"));
    dialog->resize(600, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    GearWidget *gear = new GearWidget(dialog);
    gear->setRPM(0);
    gear->setRunning(false);
    layout->addWidget(gear, 1);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    QPushButton *startBtn = new QPushButton(tr("Start"), dialog);
    connect(startBtn, &QPushButton::clicked, [gear]() {
        gear->setRPM(300);
        gear->setRunning(true);
    });
    btnLayout->addWidget(startBtn);
    
    QPushButton *stopBtn = new QPushButton(tr("Stop"), dialog);
    connect(stopBtn, &QPushButton::clicked, [gear]() {
        gear->setRunning(false);
    });
    btnLayout->addWidget(stopBtn);
    
    layout->addLayout(btnLayout);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createBeltDriveDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Belt Drive Demo"));
    dialog->resize(600, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    BeltDriveWidget *belt = new BeltDriveWidget(dialog);
    belt->setRPM(0);
    belt->setRunning(false);
    layout->addWidget(belt, 1);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    QPushButton *startBtn = new QPushButton(tr("Start"), dialog);
    connect(startBtn, &QPushButton::clicked, [belt]() {
        belt->setRPM(400);
        belt->setRunning(true);
    });
    btnLayout->addWidget(startBtn);
    
    QPushButton *stopBtn = new QPushButton(tr("Stop"), dialog);
    connect(stopBtn, &QPushButton::clicked, [belt]() {
        belt->setRunning(false);
    });
    btnLayout->addWidget(stopBtn);
    
    layout->addLayout(btnLayout);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createPistonDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Piston Engine Demo"));
    dialog->resize(600, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    PistonWidget *piston = new PistonWidget(dialog);
    piston->setRPM(0);
    piston->setPressure(0);
    piston->setRunning(false);
    layout->addWidget(piston, 1);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    QPushButton *startBtn = new QPushButton(tr("Start"), dialog);
    connect(startBtn, &QPushButton::clicked, [piston]() {
        piston->setRPM(600);
        piston->setPressure(30);
        piston->setRunning(true);
    });
    btnLayout->addWidget(startBtn);
    
    QPushButton *stopBtn = new QPushButton(tr("Stop"), dialog);
    connect(stopBtn, &QPushButton::clicked, [piston]() {
        piston->setRunning(false);
    });
    btnLayout->addWidget(stopBtn);
    
    layout->addLayout(btnLayout);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createPendulumDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Pendulum Demo"));
    dialog->resize(400, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    PendulumWidget *pendulum = new PendulumWidget(dialog);
    pendulum->setAmplitude(45);
    pendulum->setDamping(0.1);
    pendulum->setSwinging(false);
    layout->addWidget(pendulum, 1);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    QPushButton *startBtn = new QPushButton(tr("Start Swing"), dialog);
    connect(startBtn, &QPushButton::clicked, [pendulum]() {
        pendulum->setSwinging(true);
    });
    btnLayout->addWidget(startBtn);
    
    QPushButton *stopBtn = new QPushButton(tr("Stop"), dialog);
    connect(stopBtn, &QPushButton::clicked, [pendulum]() {
        pendulum->setSwinging(false);
    });
    btnLayout->addWidget(stopBtn);
    
    layout->addLayout(btnLayout);
    
    QLabel *amplitudeLabel = new QLabel(tr("Amplitude:"), dialog);
    layout->addWidget(amplitudeLabel);
    
    QSlider *amplitudeSlider = new QSlider(Qt::Horizontal, dialog);
    amplitudeSlider->setRange(10, 85);
    amplitudeSlider->setValue(45);
    connect(amplitudeSlider, &QSlider::valueChanged, pendulum, &PendulumWidget::setAmplitude);
    layout->addWidget(amplitudeSlider);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createSpringDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Spring Oscillation Demo"));
    dialog->resize(400, 550);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    SpringWidget *spring = new SpringWidget(dialog);
    spring->setAmplitude(50);
    spring->setFrequency(0.5);
    spring->setDamping(0.1);
    spring->setOscillating(false);
    layout->addWidget(spring, 1);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    
    QPushButton *startBtn = new QPushButton(tr("Start Oscillation"), dialog);
    connect(startBtn, &QPushButton::clicked, [spring]() {
        spring->setOscillating(true);
    });
    btnLayout->addWidget(startBtn);
    
    QPushButton *stopBtn = new QPushButton(tr("Stop"), dialog);
    connect(stopBtn, &QPushButton::clicked, [spring]() {
        spring->setOscillating(false);
    });
    btnLayout->addWidget(stopBtn);
    
    layout->addLayout(btnLayout);
    
    QLabel *amplitudeLabel = new QLabel(tr("Amplitude (mm):"), dialog);
    layout->addWidget(amplitudeLabel);
    
    QSlider *amplitudeSlider = new QSlider(Qt::Horizontal, dialog);
    amplitudeSlider->setRange(10, 100);
    amplitudeSlider->setValue(50);
    connect(amplitudeSlider, &QSlider::valueChanged, [spring, amplitudeLabel](int value) {
        spring->setAmplitude(value);
        amplitudeLabel->setText(tr("Amplitude (mm): %1").arg(value));
    });
    layout->addWidget(amplitudeSlider);
    
    QLabel *frequencyLabel = new QLabel(tr("Frequency (Hz): 0.5"), dialog);
    layout->addWidget(frequencyLabel);
    
    QSlider *frequencySlider = new QSlider(Qt::Horizontal, dialog);
    frequencySlider->setRange(10, 300);
    frequencySlider->setValue(50);
    connect(frequencySlider, &QSlider::valueChanged, [spring, frequencyLabel](int value) {
        double freq = value / 100.0;
        spring->setFrequency(freq);
        frequencyLabel->setText(tr("Frequency (Hz): %1").arg(freq, 0, 'f', 2));
    });
    layout->addWidget(frequencySlider);
    
    QLabel *dampingLabel = new QLabel(tr("Damping: 0.1"), dialog);
    layout->addWidget(dampingLabel);
    
    QSlider *dampingSlider = new QSlider(Qt::Horizontal, dialog);
    dampingSlider->setRange(0, 100);
    dampingSlider->setValue(10);
    connect(dampingSlider, &QSlider::valueChanged, [spring, dampingLabel](int value) {
        double damping = value / 100.0;
        spring->setDamping(damping);
        dampingLabel->setText(tr("Damping: %1").arg(damping, 0, 'f', 2));
    });
    layout->addWidget(dampingSlider);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createSliderCrankDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Slider-Crank Mechanism Demo"));
    dialog->resize(500, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    SliderCrankWidget *widget = new SliderCrankWidget(dialog);
    widget->setRPM(60);
    widget->setRunning(true);
    layout->addWidget(widget, 1);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *startBtn = new QPushButton(tr("Start"), dialog);
    connect(startBtn, &QPushButton::clicked, [widget]() { widget->setRunning(true); });
    btnLayout->addWidget(startBtn);
    
    QPushButton *stopBtn = new QPushButton(tr("Stop"), dialog);
    connect(stopBtn, &QPushButton::clicked, [widget]() { widget->setRunning(false); });
    btnLayout->addWidget(stopBtn);
    layout->addLayout(btnLayout);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createCamDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Cam Mechanism Demo"));
    dialog->resize(450, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    CamWidget *widget = new CamWidget(dialog);
    widget->setRPM(60);
    widget->setCamProfile(1);
    widget->setRunning(true);
    layout->addWidget(widget, 1);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *startBtn = new QPushButton(tr("Start"), dialog);
    connect(startBtn, &QPushButton::clicked, [widget]() { widget->setRunning(true); });
    btnLayout->addWidget(startBtn);
    
    QPushButton *stopBtn = new QPushButton(tr("Stop"), dialog);
    connect(stopBtn, &QPushButton::clicked, [widget]() { widget->setRunning(false); });
    btnLayout->addWidget(stopBtn);
    layout->addLayout(btnLayout);
    
    return dialog;
}

// Chart component demos - to be continued
QDialog* ComponentDemoWindow::createWaveformChartDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Waveform Chart Demo"));
    dialog->resize(800, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    WaveformChart *chart = new WaveformChart(dialog);
    chart->setMaxPoints(200);
    chart->setYRange(-1.5, 1.5);
    chart->setPlotColor(QColor(255, 100, 100));
    chart->setGridEnabled(true);
    layout->addWidget(chart);
    
    QTimer *timer = new QTimer(dialog);
    double *time = new double(0);
    connect(timer, &QTimer::timeout, [chart, time]() {
        *time += 0.1;
        chart->addDataPoint(qSin(*time));
    });
    
    QPushButton *startBtn = new QPushButton(tr("Stop"), dialog);
    startBtn->setCheckable(true);
    startBtn->setChecked(true);  // Start with animation on
    timer->start(50);  // Auto start
    connect(startBtn, &QPushButton::toggled, [timer, startBtn](bool checked) {
        if (checked) {
            timer->start(50);
            startBtn->setText(tr("Stop"));
        } else {
            timer->stop();
            startBtn->setText(tr("Start"));
        }
    });
    layout->addWidget(startBtn);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createXYGraphDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("XY Graph Demo"));
    dialog->resize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    XYGraph *graph = new XYGraph(dialog);
    graph->setXRange(-10, 10);
    graph->setYRange(-10, 10);
    graph->setGridEnabled(true);
    graph->setPlotColor(QColor(100, 255, 100));
    graph->setPlotStyle(2);  // Line and points
    layout->addWidget(graph);
    
    // Animation
    QTimer *timer = new QTimer(dialog);
    double *phase = new double(0);
    connect(timer, &QTimer::timeout, [graph, phase]() {
        *phase += 0.1;
        QVector<QPointF> data;
        for (double t = 0; t < 6.28; t += 0.1) {
            double radius = 5 + 2 * qSin(t * 3 + *phase);
            data.append(QPointF(radius * qCos(t), radius * qSin(t)));
        }
        graph->setData(data);
    });
    
    QPushButton *animateBtn = new QPushButton(tr("Start Animation"), dialog);
    animateBtn->setCheckable(true);
    connect(animateBtn, &QPushButton::toggled, [timer, animateBtn](bool checked) {
        if (checked) {
            timer->start(50);
            animateBtn->setText(tr("Stop Animation"));
        } else {
            timer->stop();
            animateBtn->setText(tr("Start Animation"));
        }
    });
    layout->addWidget(animateBtn);
    
    // Initial static data
    QVector<QPointF> data;
    for (double t = 0; t < 6.28; t += 0.1) {
        data.append(QPointF(5 * qCos(t), 5 * qSin(t)));
    }
    graph->setData(data);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createIntensityChartDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Intensity Chart Demo"));
    dialog->resize(700, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    IntensityChart *chart = new IntensityChart(dialog);
    chart->setSize(100, 100);  // Increase resolution for smoother display
    chart->setColorMap(1);  // Use Heat map (1) instead of Rainbow (0)
    chart->setGridEnabled(false);  // Disable grid lines
    layout->addWidget(chart);
    
    // Animation
    QTimer *timer = new QTimer(dialog);
    double *time = new double(0);
    connect(timer, &QTimer::timeout, [chart, time]() {
        *time += 0.1;
        QVector<QVector<double>> data(100, QVector<double>(100));
        for (int i = 0; i < 100; ++i) {
            for (int j = 0; j < 100; ++j) {
                // Animated 2D pattern with moving center
                double centerX = 50.0 + 10.0 * qSin(*time);
                double centerY = 50.0 + 10.0 * qCos(*time * 0.7);
                double x = (i - centerX) / 25.0;
                double y = (j - centerY) / 25.0;
                double value = 100.0 * qExp(-(x * x + y * y));
                
                // Add animated wave pattern
                double wave = 15.0 * qSin(i * 0.2 + *time) * qCos(j * 0.2 + *time * 0.8);
                
                // Ensure value is always positive and in good range
                data[i][j] = qMax(5.0, qMin(100.0, value + wave + 25.0));
            }
        }
        chart->setData(data);
    });
    
    QPushButton *animateBtn = new QPushButton(tr("Start Animation"), dialog);
    animateBtn->setCheckable(true);
    connect(animateBtn, &QPushButton::toggled, [timer, animateBtn](bool checked) {
        if (checked) {
            timer->start(100);
            animateBtn->setText(tr("Stop Animation"));
        } else {
            timer->stop();
            animateBtn->setText(tr("Start Animation"));
        }
    });
    layout->addWidget(animateBtn);
    
    // Generate initial static data with smooth gradient
    QVector<QVector<double>> data(100, QVector<double>(100));
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            // Simple 2D Gaussian-like pattern
            double x = (i - 50.0) / 25.0;
            double y = (j - 50.0) / 25.0;
            double value = 100.0 * qExp(-(x * x + y * y));
            
            // Add some wave pattern
            double wave = 15.0 * qSin(i * 0.2) * qCos(j * 0.2);
            
            // Ensure value is always positive and in good range
            data[i][j] = qMax(5.0, qMin(100.0, value + wave + 25.0));
        }
    }
    chart->setData(data);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createPieChartDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Pie Chart Demo"));
    dialog->resize(600, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    PieChart *chart = new PieChart(dialog);
    chart->addSlice("Category A", 30, QColor(255, 100, 100));
    chart->addSlice("Category B", 25, QColor(100, 255, 100));
    chart->addSlice("Category C", 20, QColor(100, 100, 255));
    chart->addSlice("Category D", 15, QColor(255, 255, 100));
    chart->addSlice("Category E", 10, QColor(255, 100, 255));
    layout->addWidget(chart);
    
    // Animation
    QTimer *timer = new QTimer(dialog);
    double *time = new double(0);
    connect(timer, &QTimer::timeout, [chart, time]() {
        *time += 0.1;
        chart->setSliceValue("Category A", 30 + 10 * qSin(*time));
        chart->setSliceValue("Category B", 25 + 8 * qSin(*time + 1));
        chart->setSliceValue("Category C", 20 + 6 * qSin(*time + 2));
        chart->setSliceValue("Category D", 15 + 5 * qSin(*time + 3));
        chart->setSliceValue("Category E", 10 + 4 * qSin(*time + 4));
    });
    
    QPushButton *animateBtn = new QPushButton(tr("Start Animation"), dialog);
    animateBtn->setCheckable(true);
    connect(animateBtn, &QPushButton::toggled, [timer, animateBtn](bool checked) {
        if (checked) {
            timer->start(100);
            animateBtn->setText(tr("Stop Animation"));
        } else {
            timer->stop();
            animateBtn->setText(tr("Start Animation"));
        }
    });
    layout->addWidget(animateBtn);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createLineChartDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Line Chart Demo"));
    dialog->resize(800, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    LineChart *chart = new LineChart(dialog);
    chart->addSeries("Series 1", QColor(255, 100, 100));
    chart->addSeries("Series 2", QColor(100, 255, 100));
    chart->addSeries("Series 3", QColor(100, 100, 255));
    chart->setGridEnabled(true);
    chart->setShowLegend(true);
    layout->addWidget(chart);
    
    // Animation - continuously add new data points
    QTimer *timer = new QTimer(dialog);
    double *time = new double(0);
    connect(timer, &QTimer::timeout, [chart, time]() {
        *time += 0.2;
        chart->addDataPoint("Series 1", 50 + 30 * qSin(*time * 0.3));
        chart->addDataPoint("Series 2", 50 + 20 * qCos(*time * 0.4));
        chart->addDataPoint("Series 3", 50 + 25 * qSin(*time * 0.5 + 1));
    });
    
    QPushButton *animateBtn = new QPushButton(tr("Start Animation"), dialog);
    animateBtn->setCheckable(true);
    connect(animateBtn, &QPushButton::toggled, [timer, animateBtn](bool checked) {
        if (checked) {
            timer->start(100);
            animateBtn->setText(tr("Stop Animation"));
        } else {
            timer->stop();
            animateBtn->setText(tr("Start Animation"));
        }
    });
    
    QPushButton *clearBtn = new QPushButton(tr("Clear"), dialog);
    connect(clearBtn, &QPushButton::clicked, [chart]() {
        chart->clearAllSeries();
        chart->addSeries("Series 1", QColor(255, 100, 100));
        chart->addSeries("Series 2", QColor(100, 255, 100));
        chart->addSeries("Series 3", QColor(100, 100, 255));
    });
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(animateBtn);
    btnLayout->addWidget(clearBtn);
    layout->addLayout(btnLayout);
    
    // Start with empty data - click animation button to begin
    
    return dialog;
}

QDialog* ComponentDemoWindow::createHistogramDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Histogram Demo"));
    dialog->resize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    Histogram *chart = new Histogram(dialog);
    chart->setBinCount(20);
    chart->setShowStatistics(false);  // Disable statistics to avoid text overlap
    
    // Generate random data
    QVector<double> data;
    for (int i = 0; i < 1000; ++i) {
        data.append(50 + 15 * qSin(i * 0.1) + (std::rand() % 20 - 10));
    }
    chart->setData(data);
    
    layout->addWidget(chart);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createPolarPlotDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Polar Plot Demo"));
    dialog->resize(600, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    PolarPlot *chart = new PolarPlot(dialog);
    chart->setPlotColor(QColor(100, 255, 100));
    chart->setShowGrid(true);
    chart->setFillArea(true);
    layout->addWidget(chart);
    
    // Animation
    QTimer *timer = new QTimer(dialog);
    double *phase = new double(0);
    connect(timer, &QTimer::timeout, [chart, phase]() {
        *phase += 0.1;
        QVector<double> angles;
        QVector<double> magnitudes;
        for (double angle = 0; angle < 360; angle += 5) {
            angles.append(angle);
            magnitudes.append(5 + 3 * qSin(angle * 0.1 + *phase));
        }
        chart->setData(angles, magnitudes);
    });
    
    QPushButton *animateBtn = new QPushButton(tr("Start Animation"), dialog);
    animateBtn->setCheckable(true);
    connect(animateBtn, &QPushButton::toggled, [timer, animateBtn](bool checked) {
        if (checked) {
            timer->start(50);
            animateBtn->setText(tr("Stop Animation"));
        } else {
            timer->stop();
            animateBtn->setText(tr("Start Animation"));
        }
    });
    layout->addWidget(animateBtn);
    
    // Initial data
    QVector<double> angles;
    QVector<double> magnitudes;
    for (double angle = 0; angle < 360; angle += 5) {
        angles.append(angle);
        magnitudes.append(5 + 3 * qSin(angle * 0.1));
    }
    chart->setData(angles, magnitudes);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createSpectrumAnalyzerDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Spectrum Analyzer Demo"));
    dialog->resize(800, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    SpectrumAnalyzer *analyzer = new SpectrumAnalyzer(dialog);
    analyzer->setFrequencyRange(0, 20000);
    analyzer->setBarColor(QColor(100, 200, 255));
    analyzer->setPeakHoldEnabled(true);
    layout->addWidget(analyzer);
    
    QTimer *timer = new QTimer(dialog);
    connect(timer, &QTimer::timeout, [analyzer]() {
        QVector<double> frequencies;
        QVector<double> amplitudes;
        for (int i = 0; i < 64; ++i) {
            frequencies.append(i * 20000.0 / 64.0);
            amplitudes.append(std::rand() % 100);
        }
        analyzer->setSpectrumData(frequencies, amplitudes);
    });
    
    QPushButton *startBtn = new QPushButton(tr("Start Animation"), dialog);
    startBtn->setCheckable(true);
    connect(startBtn, &QPushButton::toggled, [timer, startBtn](bool checked) {
        if (checked) {
            timer->start(100);
            startBtn->setText(tr("Stop Animation"));
        } else {
            timer->stop();
            startBtn->setText(tr("Start Animation"));
        }
    });
    layout->addWidget(startBtn);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createScatterPlotDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Scatter Plot Demo"));
    dialog->resize(700, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    ScatterPlot *chart = new ScatterPlot(dialog);
    chart->setXRange(-10, 10);
    chart->setYRange(-10, 10);
    chart->setPointColor(QColor(100, 255, 100));
    chart->setPointSize(6);
    chart->setShowTrendLine(true);
    
    // Generate random scatter data
    QVector<QPointF> data;
    for (int i = 0; i < 100; ++i) {
        double x = (std::rand() % 200 - 100) / 10.0;
        double y = (std::rand() % 200 - 100) / 10.0;
        data.append(QPointF(x, y));
    }
    chart->setData(data);
    
    layout->addWidget(chart);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createAreaChartDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Area Chart Demo"));
    dialog->resize(800, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    AreaChart *chart = new AreaChart(dialog);
    chart->addSeries("Area 1", QColor(255, 100, 100, 150));
    chart->addSeries("Area 2", QColor(100, 255, 100, 150));
    chart->setStacked(false);
    chart->setShowLegend(true);
    layout->addWidget(chart);
    
    // Animation
    QTimer *timer = new QTimer(dialog);
    double *time = new double(0);
    connect(timer, &QTimer::timeout, [chart, time]() {
        *time += 0.2;
        chart->addDataPoint("Area 1", 30 + 20 * qSin(*time * 0.3));
        chart->addDataPoint("Area 2", 50 + 15 * qCos(*time * 0.4));
    });
    
    QPushButton *animateBtn = new QPushButton(tr("Start Animation"), dialog);
    animateBtn->setCheckable(true);
    connect(animateBtn, &QPushButton::toggled, [timer, animateBtn](bool checked) {
        if (checked) {
            timer->start(100);
            animateBtn->setText(tr("Stop Animation"));
        } else {
            timer->stop();
            animateBtn->setText(tr("Start Animation"));
        }
    });
    
    QPushButton *clearBtn = new QPushButton(tr("Clear"), dialog);
    connect(clearBtn, &QPushButton::clicked, [chart]() {
        chart->clearAllSeries();
        chart->addSeries("Area 1", QColor(255, 100, 100, 150));
        chart->addSeries("Area 2", QColor(100, 255, 100, 150));
    });
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(animateBtn);
    btnLayout->addWidget(clearBtn);
    layout->addLayout(btnLayout);
    
    // Start with empty data - click animation button to begin
    
    return dialog;
}

QDialog* ComponentDemoWindow::createRadarChartDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Radar Chart Demo"));
    dialog->resize(600, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    RadarChart *chart = new RadarChart(dialog);
    
    QVector<QString> categories;
    categories << "Speed" << "Power" << "Accuracy" << "Efficiency" << "Reliability";
    chart->setCategories(categories);
    
    QVector<double> values;
    values << 80 << 65 << 90 << 75 << 85;
    chart->addDataSeries("System A", values, QColor(100, 200, 255, 150));
    chart->setFillArea(true);
    chart->setShowLegend(true);
    
    layout->addWidget(chart);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createBubbleChartDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Bubble Chart Demo"));
    dialog->resize(700, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    BubbleChart *chart = new BubbleChart(dialog);
    chart->setXRange(0, 100);
    chart->setYRange(0, 100);
    chart->setShowLabels(true);
    
    // Add sample bubbles (x, y, size, label, color)
    for (int i = 0; i < 20; ++i) {
        double x = std::rand() % 100;
        double y = std::rand() % 100;
        double size = std::rand() % 50 + 10;
        QString label = QString("B%1").arg(i + 1);
        QColor color = QColor(std::rand() % 200 + 55, std::rand() % 200 + 55, std::rand() % 200 + 55, 150);
        chart->addBubble(x, y, size, label, color);
    }
    
    layout->addWidget(chart);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createDigitalWaveformDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Digital Waveform Demo"));
    dialog->resize(800, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    DigitalWaveform *chart = new DigitalWaveform(dialog);
    chart->addChannel("CLK", QColor(255, 100, 100));
    chart->addChannel("DATA", QColor(100, 255, 100));
    chart->addChannel("ENABLE", QColor(100, 100, 255));
    chart->setMaxTimeSteps(100);
    chart->setShowLabels(true);
    
    // Add sample digital data using transitions
    for (int i = 0; i < 100; ++i) {
        chart->addChannelTransition("CLK", i % 2 == 0);
        chart->addChannelTransition("DATA", (i / 4) % 2 == 0);
        chart->addChannelTransition("ENABLE", i > 20 && i < 80);
    }
    
    layout->addWidget(chart);
    
    return dialog;
}

QDialog* ComponentDemoWindow::createStackedBarChartDemo()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Stacked Bar Chart Demo"));
    dialog->resize(700, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    StackedBarChart *chart = new StackedBarChart(dialog);
    
    // Add categories
    QVector<QString> categories;
    categories << "Q1" << "Q2" << "Q3" << "Q4";
    chart->setCategories(categories);
    
    // Add series
    chart->addSeries("Product A", QColor(255, 100, 100));
    chart->addSeries("Product B", QColor(100, 255, 100));
    chart->addSeries("Product C", QColor(100, 100, 255));
    chart->setShowLegend(true);
    chart->setShowValues(true);
    
    // Add data for each category
    QMap<QString, double> q1Data;
    q1Data["Product A"] = 30;
    q1Data["Product B"] = 25;
    q1Data["Product C"] = 20;
    chart->setBarData("Q1", q1Data);
    
    QMap<QString, double> q2Data;
    q2Data["Product A"] = 35;
    q2Data["Product B"] = 28;
    q2Data["Product C"] = 22;
    chart->setBarData("Q2", q2Data);
    
    QMap<QString, double> q3Data;
    q3Data["Product A"] = 32;
    q3Data["Product B"] = 30;
    q3Data["Product C"] = 25;
    chart->setBarData("Q3", q3Data);
    
    QMap<QString, double> q4Data;
    q4Data["Product A"] = 38;
    q4Data["Product B"] = 27;
    q4Data["Product C"] = 23;
    chart->setBarData("Q4", q4Data);
    
    layout->addWidget(chart);
    
    return dialog;
}

