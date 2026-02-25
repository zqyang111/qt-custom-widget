#include "ChartDemoWindow.h"
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

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QTabWidget>
#include <QtMath>

ChartDemoWindow::ChartDemoWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_waveformChart1(nullptr)
    , m_waveformChart2(nullptr)
    , m_xyGraph(nullptr)
    , m_intensityChart(nullptr)
    , m_pieChart(nullptr)
    , m_lineChart(nullptr)
    , m_histogram(nullptr)
    , m_polarPlot(nullptr)
    , m_spectrumAnalyzer(nullptr)
    , m_scatterPlot(nullptr)
    , m_areaChart(nullptr)
    , m_radarChart(nullptr)
    , m_bubbleChart(nullptr)
    , m_digitalWaveform(nullptr)
    , m_stackedBarChart(nullptr)
    , m_tabWidget(nullptr)
    , m_simulationTimer(new QTimer(this))
    , m_simulationRunning(false)
    , m_simulationTime(0.0)
{
    setWindowTitle(tr("LabVIEW-Style Chart Components Demo"));
    resize(1200, 800);

    setupUi();
    createMenuBar();

    connect(m_simulationTimer, &QTimer::timeout, this, &ChartDemoWindow::onSimulationTimer);
    m_simulationTimer->setInterval(50); // 20 Hz update rate
}

ChartDemoWindow::~ChartDemoWindow()
{
}

void ChartDemoWindow::setupUi()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Title
    QLabel *titleLabel = new QLabel(tr("LabVIEW-Style Chart Components Demo"), this);
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
    createWaveformTab();
    createXYGraphTab();  // Contains both XY Graph and Intensity Chart
    createStatisticsTab();
    createAdvancedTab();
    createSpecializedTab();

    // Control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    mainLayout->addLayout(buttonLayout);

    QPushButton *startBtn = new QPushButton(tr("Start Simulation"), this);
    QPushButton *stopBtn = new QPushButton(tr("Stop Simulation"), this);
    QPushButton *resetBtn = new QPushButton(tr("Reset All"), this);

    buttonLayout->addStretch();
    buttonLayout->addWidget(startBtn);
    buttonLayout->addWidget(stopBtn);
    buttonLayout->addWidget(resetBtn);
    buttonLayout->addStretch();

    connect(startBtn, &QPushButton::clicked, this, &ChartDemoWindow::startSimulation);
    connect(stopBtn, &QPushButton::clicked, this, &ChartDemoWindow::stopSimulation);
    connect(resetBtn, &QPushButton::clicked, this, &ChartDemoWindow::resetCharts);

    // Set dark theme (Photoshop-style)
    setStyleSheet(
        "QMainWindow { background-color: #2b2b2b; }"
        "QWidget { background-color: #2b2b2b; color: #e0e0e0; }"
        "QLabel { color: #e0e0e0; }"
        "QTabWidget::pane { border: 2px solid #3a3a3a; background-color: #2b2b2b; }"
        "QTabBar::tab { background-color: #3a3a3a; color: #e0e0e0; padding: 8px 20px; "
        "              border: 1px solid #1a1a1a; margin-right: 2px; }"
        "QTabBar::tab:selected { background-color: #4a4a4a; border-bottom: 3px solid #0078d7; }"
        "QTabBar::tab:hover { background-color: #454545; }"
        "QGroupBox { border: 2px solid #3a3a3a; border-radius: 5px; "
        "           margin-top: 10px; padding-top: 10px; background-color: #353535; color: #e0e0e0; }"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; "
        "                  padding: 5px; background-color: #3a3a3a; border-radius: 3px; color: #e0e0e0; }"
        "QPushButton { background-color: #4a4a4a; border: 1px solid #5a5a5a; "
        "             border-radius: 4px; padding: 8px; min-width: 120px; color: #e0e0e0; }"
        "QPushButton:hover { background-color: #5a5a5a; }"
        "QPushButton:pressed { background-color: #3a3a3a; }"
        "QMenuBar { background-color: #2b2b2b; color: #e0e0e0; }"
        "QMenuBar::item { background-color: #2b2b2b; color: #e0e0e0; }"
        "QMenuBar::item:selected { background-color: #4a4a4a; }"
        "QMenu { background-color: #2b2b2b; color: #e0e0e0; border: 1px solid #3a3a3a; }"
        "QMenu::item:selected { background-color: #4a4a4a; }"
    );
}

void ChartDemoWindow::createWaveformTab()
{
    QWidget *waveformTab = new QWidget();
    QVBoxLayout *tabLayout = new QVBoxLayout(waveformTab);

    // Waveform Chart 1
    QGroupBox *chart1Group = new QGroupBox(tr("Waveform Chart 1 - Sine Wave"), waveformTab);
    QVBoxLayout *chart1Layout = new QVBoxLayout(chart1Group);
    m_waveformChart1 = new WaveformChart(waveformTab);
    m_waveformChart1->setTitle("Sine Wave");
    m_waveformChart1->setYLabel("Amplitude");
    m_waveformChart1->setMaxPoints(100);
    m_waveformChart1->setPlotColor(QColor(0, 255, 0));  // Green
    chart1Layout->addWidget(m_waveformChart1);
    tabLayout->addWidget(chart1Group);

    // Waveform Chart 2
    QGroupBox *chart2Group = new QGroupBox(tr("Waveform Chart 2 - Cosine Wave"), waveformTab);
    QVBoxLayout *chart2Layout = new QVBoxLayout(chart2Group);
    m_waveformChart2 = new WaveformChart(waveformTab);
    m_waveformChart2->setTitle("Cosine Wave");
    m_waveformChart2->setYLabel("Amplitude");
    m_waveformChart2->setMaxPoints(100);
    m_waveformChart2->setPlotColor(QColor(255, 255, 0));  // Yellow
    chart2Layout->addWidget(m_waveformChart2);
    tabLayout->addWidget(chart2Group);

    m_tabWidget->addTab(waveformTab, tr("Waveform Charts"));
}

void ChartDemoWindow::createXYGraphTab()
{
    QWidget *xyTab = new QWidget();
    QHBoxLayout *tabLayout = new QHBoxLayout(xyTab);

    // XY Graph - left side
    QGroupBox *xyGroup = new QGroupBox(tr("XY Graph - Satellite Orbit"), xyTab);
    QVBoxLayout *xyLayout = new QVBoxLayout(xyGroup);
    
    m_xyGraph = new XYGraph(xyTab);
    m_xyGraph->setTitle("Satellite Orbital Trajectory");
    m_xyGraph->setXLabel("X (km)");
    m_xyGraph->setYLabel("Y (km)");
    m_xyGraph->setPlotColor(QColor(0, 200, 255));  // Cyan for satellite
    m_xyGraph->setPlotStyle(0);  // Line only
    m_xyGraph->setXRange(-150, 150);
    m_xyGraph->setYRange(-150, 150);
    m_xyGraph->setAutoScale(false);
    xyLayout->addWidget(m_xyGraph);
    
    QLabel *xyInfo = new QLabel(tr("XY Graph displays satellite elliptical orbit around Earth"), xyTab);
    xyInfo->setAlignment(Qt::AlignCenter);
    xyLayout->addWidget(xyInfo);
    
    tabLayout->addWidget(xyGroup);

    // Intensity Chart - right side
    QGroupBox *intensityGroup = new QGroupBox(tr("Intensity Chart - 2D Heat Map"), xyTab);
    QVBoxLayout *intensityLayout = new QVBoxLayout(intensityGroup);
    
    m_intensityChart = new IntensityChart(xyTab);
    m_intensityChart->setTitle("Temperature Distribution");
    m_intensityChart->setSize(30, 40);  // 30 rows, 40 columns
    m_intensityChart->setColorMap(1);  // Heat map
    intensityLayout->addWidget(m_intensityChart);
    
    QLabel *intensityInfo = new QLabel(tr("Intensity Chart displays 2D data as color-coded heat map"), xyTab);
    intensityInfo->setAlignment(Qt::AlignCenter);
    intensityLayout->addWidget(intensityInfo);
    
    tabLayout->addWidget(intensityGroup);

    m_tabWidget->addTab(xyTab, tr("XY && Intensity Charts"));
}

void ChartDemoWindow::createIntensityChartTab()
{
    // This function is no longer used - combined with XY Graph tab
}

void ChartDemoWindow::createStatisticsTab()
{
    QWidget *statsTab = new QWidget();
    QGridLayout *tabLayout = new QGridLayout(statsTab);

    // Pie Chart
    QGroupBox *pieGroup = new QGroupBox(tr("Pie Chart - Resource Distribution"), statsTab);
    QVBoxLayout *pieLayout = new QVBoxLayout(pieGroup);
    
    m_pieChart = new PieChart(statsTab);
    m_pieChart->setTitle("System Resource Usage");
    m_pieChart->addSlice("CPU", 35, QColor(255, 100, 100));
    m_pieChart->addSlice("Memory", 45, QColor(100, 200, 255));
    m_pieChart->addSlice("Disk", 15, QColor(100, 255, 100));
    m_pieChart->addSlice("Network", 5, QColor(255, 255, 100));
    pieLayout->addWidget(m_pieChart);
    
    tabLayout->addWidget(pieGroup, 0, 0);

    // Line Chart
    QGroupBox *lineGroup = new QGroupBox(tr("Multi-Line Chart - Temperature Trends"), statsTab);
    QVBoxLayout *lineLayout = new QVBoxLayout(lineGroup);
    
    m_lineChart = new LineChart(statsTab);
    m_lineChart->setTitle("Temperature Sensors");
    m_lineChart->setYLabel("Temperature (C)");
    m_lineChart->setXLabel("Time");
    m_lineChart->addSeries("Sensor 1", QColor(255, 100, 100));
    m_lineChart->addSeries("Sensor 2", QColor(100, 255, 100));
    m_lineChart->addSeries("Sensor 3", QColor(100, 150, 255));
    lineLayout->addWidget(m_lineChart);
    
    tabLayout->addWidget(lineGroup, 0, 1);

    // Histogram
    QGroupBox *histGroup = new QGroupBox(tr("Histogram - Data Distribution"), statsTab);
    QVBoxLayout *histLayout = new QVBoxLayout(histGroup);
    
    m_histogram = new Histogram(statsTab);
    m_histogram->setTitle("Measurement Distribution");
    m_histogram->setXLabel("Measurement Value");
    m_histogram->setYLabel("Count");
    m_histogram->setBinCount(25);
    histLayout->addWidget(m_histogram);
    
    tabLayout->addWidget(histGroup, 1, 0, 1, 2);

    m_tabWidget->addTab(statsTab, tr("Statistical Charts"));
}

void ChartDemoWindow::createAdvancedTab()
{
    QWidget *advTab = new QWidget();
    QGridLayout *tabLayout = new QGridLayout(advTab);

    // Polar Plot
    QGroupBox *polarGroup = new QGroupBox(tr("Polar Plot - Radar Scan"), advTab);
    QVBoxLayout *polarLayout = new QVBoxLayout(polarGroup);
    
    m_polarPlot = new PolarPlot(advTab);
    m_polarPlot->setTitle("Radar Detection Pattern");
    m_polarPlot->setPlotColor(QColor(0, 255, 100));
    m_polarPlot->setFillArea(true);
    polarLayout->addWidget(m_polarPlot);
    
    tabLayout->addWidget(polarGroup, 0, 0);

    // Spectrum Analyzer
    QGroupBox *spectrumGroup = new QGroupBox(tr("Spectrum Analyzer - Frequency Domain"), advTab);
    QVBoxLayout *spectrumLayout = new QVBoxLayout(spectrumGroup);
    
    m_spectrumAnalyzer = new SpectrumAnalyzer(advTab);
    m_spectrumAnalyzer->setTitle("Signal Spectrum");
    m_spectrumAnalyzer->setFrequencyRange(0, 1000);
    spectrumLayout->addWidget(m_spectrumAnalyzer);
    
    tabLayout->addWidget(spectrumGroup, 0, 1);

    // Scatter Plot
    QGroupBox *scatterGroup = new QGroupBox(tr("Scatter Plot - Correlation Analysis"), advTab);
    QVBoxLayout *scatterLayout = new QVBoxLayout(scatterGroup);
    
    m_scatterPlot = new ScatterPlot(advTab);
    m_scatterPlot->setTitle("Temperature vs Pressure");
    m_scatterPlot->setXLabel("Temperature (C)");
    m_scatterPlot->setYLabel("Pressure (kPa)");
    m_scatterPlot->setPointColor(QColor(255, 150, 100));
    m_scatterPlot->setShowTrendLine(true);
    m_scatterPlot->setXRange(35, 85);  // Temperature range
    m_scatterPlot->setYRange(80, 130);  // Pressure range
    m_scatterPlot->setAutoScale(false);  // Use fixed range
    scatterLayout->addWidget(m_scatterPlot);
    
    tabLayout->addWidget(scatterGroup, 1, 0);

    // Area Chart
    QGroupBox *areaGroup = new QGroupBox(tr("Area Chart - Cumulative Trends"), advTab);
    QVBoxLayout *areaLayout = new QVBoxLayout(areaGroup);
    
    m_areaChart = new AreaChart(advTab);
    m_areaChart->setTitle("Production Levels");
    m_areaChart->setYLabel("Units");
    m_areaChart->addSeries("Product A", QColor(255, 100, 100));
    m_areaChart->addSeries("Product B", QColor(100, 255, 100));
    m_areaChart->addSeries("Product C", QColor(100, 150, 255));
    m_areaChart->setStacked(false);
    areaLayout->addWidget(m_areaChart);
    
    tabLayout->addWidget(areaGroup, 1, 1);

    m_tabWidget->addTab(advTab, tr("Advanced Charts"));
}

void ChartDemoWindow::createSpecializedTab()
{
    QWidget *specializedWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(specializedWidget);
    layout->setSpacing(10);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // Radar Chart - Multi-dimensional comparison
    m_radarChart = new RadarChart(this);
    m_radarChart->setMinimumSize(500, 500);
    m_radarChart->setTitle("Product Performance Radar");
    m_radarChart->setMaxValue(100);
    QVector<QString> radarCategories = {"Speed", "Quality", "Cost", "Reliability", "Efficiency"};
    m_radarChart->setCategories(radarCategories);
    
    QVector<double> product1 = {85, 90, 70, 88, 92};
    QVector<double> product2 = {75, 85, 90, 80, 78};
    m_radarChart->addDataSeries("Product A", product1, QColor(255, 100, 100));
    m_radarChart->addDataSeries("Product B", product2, QColor(100, 150, 255));
    layout->addWidget(m_radarChart, 0, 0);
    
    // Bubble Chart - Three-variable visualization
    m_bubbleChart = new BubbleChart(this);
    m_bubbleChart->setMinimumSize(500, 400);
    m_bubbleChart->setTitle("Product Analysis (Price vs Quality)");
    m_bubbleChart->setXLabel("Price (USD)");
    m_bubbleChart->setYLabel("Quality Score");
    m_bubbleChart->setXRange(0, 100);
    m_bubbleChart->setYRange(0, 100);
    m_bubbleChart->setSizeRange(10, 50);
    
    // Static bubbles for initial display
    m_bubbleChart->addBubble(30, 70, 25, "A", QColor(255, 100, 100));
    m_bubbleChart->addBubble(60, 80, 40, "B", QColor(100, 150, 255));
    m_bubbleChart->addBubble(45, 55, 30, "C", QColor(100, 255, 150));
    m_bubbleChart->addBubble(75, 90, 45, "D", QColor(255, 200, 100));
    layout->addWidget(m_bubbleChart, 0, 1);
    
    // Digital Waveform - Logic signal timing
    m_digitalWaveform = new DigitalWaveform(this);
    m_digitalWaveform->setMinimumSize(600, 300);
    m_digitalWaveform->setTitle("Digital Signal Timing Diagram");
    m_digitalWaveform->setMaxTimeSteps(50);
    m_digitalWaveform->addChannel("CLK", QColor(100, 255, 100));
    m_digitalWaveform->addChannel("DATA", QColor(255, 150, 100));
    m_digitalWaveform->addChannel("ENABLE", QColor(100, 200, 255));
    layout->addWidget(m_digitalWaveform, 1, 0);
    
    // Stacked Bar Chart - Composition comparison
    m_stackedBarChart = new StackedBarChart(this);
    m_stackedBarChart->setMinimumSize(500, 350);
    m_stackedBarChart->setTitle("Quarterly Revenue by Product");
    m_stackedBarChart->setYLabel("Revenue (Million USD)");
    m_stackedBarChart->addSeries("Product A", QColor(255, 100, 100));
    m_stackedBarChart->addSeries("Product B", QColor(100, 150, 255));
    m_stackedBarChart->addSeries("Product C", QColor(100, 255, 150));
    
    QVector<QString> quarters = {"Q1", "Q2", "Q3", "Q4"};
    m_stackedBarChart->setCategories(quarters);
    
    // Q1 data
    QMap<QString, double> q1Data;
    q1Data["Product A"] = 25;
    q1Data["Product B"] = 30;
    q1Data["Product C"] = 20;
    m_stackedBarChart->setBarData("Q1", q1Data);
    
    // Q2 data
    QMap<QString, double> q2Data;
    q2Data["Product A"] = 30;
    q2Data["Product B"] = 35;
    q2Data["Product C"] = 25;
    m_stackedBarChart->setBarData("Q2", q2Data);
    
    // Q3 data
    QMap<QString, double> q3Data;
    q3Data["Product A"] = 35;
    q3Data["Product B"] = 40;
    q3Data["Product C"] = 30;
    m_stackedBarChart->setBarData("Q3", q3Data);
    
    // Q4 data
    QMap<QString, double> q4Data;
    q4Data["Product A"] = 40;
    q4Data["Product B"] = 45;
    q4Data["Product C"] = 35;
    m_stackedBarChart->setBarData("Q4", q4Data);
    
    layout->addWidget(m_stackedBarChart, 1, 1);
    
    m_tabWidget->addTab(specializedWidget, tr("Specialized Charts"));
}

void ChartDemoWindow::createMenuBar()
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
        // About dialog can be added here
    });
}

void ChartDemoWindow::startSimulation()
{
    if (!m_simulationRunning) {
        m_simulationRunning = true;
        m_simulationTimer->start();
    }
}

void ChartDemoWindow::stopSimulation()
{
    if (m_simulationRunning) {
        m_simulationRunning = false;
        m_simulationTimer->stop();
    }
}

void ChartDemoWindow::resetCharts()
{
    stopSimulation();
    m_simulationTime = 0.0;
    
    m_waveformChart1->clearData();
    m_waveformChart2->clearData();
    m_xyGraph->clearData();
    m_intensityChart->clearData();
    m_lineChart->clearAllSeries();
    m_histogram->clearData();
    m_polarPlot->clearData();
    m_spectrumAnalyzer->clearData();
    m_scatterPlot->clearData();
    m_areaChart->clearAllSeries();
    m_xyData.clear();
    
    // Re-add line chart series
    m_lineChart->addSeries("Sensor 1", QColor(255, 100, 100));
    m_lineChart->addSeries("Sensor 2", QColor(100, 255, 100));
    m_lineChart->addSeries("Sensor 3", QColor(100, 150, 255));
    
    // Re-add area chart series
    m_areaChart->addSeries("Product A", QColor(255, 100, 100));
    m_areaChart->addSeries("Product B", QColor(100, 255, 100));
    m_areaChart->addSeries("Product C", QColor(100, 150, 255));
    
    // Reset pie chart
    m_pieChart->clearSlices();
    m_pieChart->addSlice("CPU", 35, QColor(255, 100, 100));
    m_pieChart->addSlice("Memory", 45, QColor(100, 200, 255));
    m_pieChart->addSlice("Disk", 15, QColor(100, 255, 100));
    m_pieChart->addSlice("Network", 5, QColor(255, 255, 100));
}

void ChartDemoWindow::onSimulationTimer()
{
    m_simulationTime += 0.05;

    // Waveform Chart 1 - Sine wave
    double sineValue = 50 + 40 * qSin(m_simulationTime * 2);
    m_waveformChart1->addDataPoint(sineValue);

    // Waveform Chart 2 - Cosine wave
    double cosineValue = 50 + 40 * qCos(m_simulationTime * 2);
    m_waveformChart2->addDataPoint(cosineValue);

    // XY Graph - Satellite elliptical orbit
    // Elliptical orbit parameters (semi-major axis a=100, semi-minor axis b=80)
    double a = 100.0;  // Semi-major axis (km)
    double b = 80.0;   // Semi-minor axis (km)
    double orbitalSpeed = 0.3;  // Orbital angular velocity
    
    double x = a * qCos(m_simulationTime * orbitalSpeed);
    double y = b * qSin(m_simulationTime * orbitalSpeed);
    m_xyGraph->addPoint(x, y);
    
    // Keep full orbit trajectory (about 400 points for complete orbit)
    m_xyData.append(QPointF(x, y));
    if (m_xyData.size() > 400) {
        m_xyData.removeFirst();
    }
    m_xyGraph->setData(m_xyData);

    // Intensity Chart - 2D wave pattern
    static int updateCounter = 0;
    if (++updateCounter % 5 == 0) {  // Update less frequently
        for (int i = 0; i < 30; ++i) {
            for (int j = 0; j < 40; ++j) {
                double value = 50 + 30 * qSin(i * 0.3 + m_simulationTime) * qCos(j * 0.2 + m_simulationTime);
                m_intensityChart->setDataAt(i, j, value);
            }
        }
    }
    
    // Line Chart - Multiple temperature sensors
    double temp1 = 60 + 20 * qSin(m_simulationTime * 0.5);
    double temp2 = 55 + 15 * qSin(m_simulationTime * 0.7 + 1.0);
    double temp3 = 65 + 25 * qSin(m_simulationTime * 0.3 + 2.0);
    
    m_lineChart->addDataPoint("Sensor 1", temp1);
    m_lineChart->addDataPoint("Sensor 2", temp2);
    m_lineChart->addDataPoint("Sensor 3", temp3);
    
    // Pie Chart - Update resource usage dynamically
    double cpuUsage = 35 + 15 * qSin(m_simulationTime * 0.4);
    double memUsage = 45 + 10 * qSin(m_simulationTime * 0.3);
    double diskUsage = 15 + 5 * qSin(m_simulationTime * 0.5);
    double netUsage = 5 + 3 * qSin(m_simulationTime * 0.6);
    
    m_pieChart->setSliceValue("CPU", cpuUsage);
    m_pieChart->setSliceValue("Memory", memUsage);
    m_pieChart->setSliceValue("Disk", diskUsage);
    m_pieChart->setSliceValue("Network", netUsage);
    
    // Histogram - Generate random normal distribution data
    if (static_cast<int>(m_simulationTime * 10) % 10 == 0) {  // Add data every second
        // Generate random value with normal distribution
        double randomValue = 50 + 20 * (qSin(m_simulationTime * 13.5) + qCos(m_simulationTime * 7.3));
        m_histogram->addValue(randomValue);
    }
    
    // Polar Plot - Generate new shape every second
    static int lastSecond = -1;
    int currentSecond = static_cast<int>(m_simulationTime);
    
    if (currentSecond != lastSecond) {
        lastSecond = currentSecond;
        
        // Clear old data and generate new radar pattern
        m_polarPlot->clearData();
        
        QVector<double> angles;
        QVector<double> magnitudes;
        
        // Generate a complete radar pattern (360 degrees)
        for (int angle = 0; angle < 360; angle += 10) {
            double mag = 50 + 30 * qSin(angle * 0.1 + currentSecond) + 
                        20 * qSin(angle * 0.3 + currentSecond * 0.5);
            angles.append(angle);
            magnitudes.append(mag);
        }
        
        m_polarPlot->setData(angles, magnitudes);
    }
    
    // Spectrum Analyzer - Simulated FFT spectrum
    for (int i = 0; i < 50; ++i) {
        double freq = i * 20.0;
        double amplitude = 30 + 50 * qExp(-0.01 * qAbs(i - 15)) * (1 + 0.3 * qSin(m_simulationTime + i * 0.2));
        m_spectrumAnalyzer->setAmplitudeAt(i, amplitude);
    }
    
    // Scatter Plot - Temperature vs Pressure correlation
    if (static_cast<int>(m_simulationTime * 10) % 2 == 0) {  // Add point every 100ms
        double temp = 60 + 18 * qSin(m_simulationTime * 0.8);  // Range: 42-78
        double pressure = 105 + 0.25 * temp + 6 * qSin(m_simulationTime * 1.3);  // Range: ~85-125
        m_scatterPlot->addPoint(temp, pressure);
    }
    
    // Area Chart - Production levels
    double prodA = 30 + 20 * qSin(m_simulationTime * 0.6);
    double prodB = 25 + 15 * qSin(m_simulationTime * 0.4 + 1.0);
    double prodC = 20 + 10 * qSin(m_simulationTime * 0.5 + 2.0);
    
    m_areaChart->addDataPoint("Product A", prodA);
    m_areaChart->addDataPoint("Product B", prodB);
    m_areaChart->addDataPoint("Product C", prodC);
    
    // Digital Waveform - Simple high/low transitions
    static int digitalCounter = 0;
    static bool clkState = false;
    static int dataPattern = 0b11010010;  // 8-bit pattern
    static int dataBitIndex = 0;
    static bool enableState = false;
    
    if (++digitalCounter >= 4) {  // Update every 4 ticks
        digitalCounter = 0;
        
        // Clock signal - regular toggle
        clkState = !clkState;
        m_digitalWaveform->addChannelTransition("CLK", clkState);
        
        // Data signal - change on every clock rising edge
        if (clkState) {
            bool dataState = (dataPattern >> dataBitIndex) & 1;
            dataBitIndex = (dataBitIndex + 1) % 8;
            m_digitalWaveform->addChannelTransition("DATA", dataState);
        }
        
        // Enable signal - toggle less frequently
        static int enableToggleCount = 0;
        enableToggleCount++;
        if (enableToggleCount >= 12) {
            enableToggleCount = 0;
            enableState = !enableState;
            m_digitalWaveform->addChannelTransition("ENABLE", enableState);
        }
    }
}

