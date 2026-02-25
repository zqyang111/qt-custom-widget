#ifndef CHARTDEMOWINDOW_H
#define CHARTDEMOWINDOW_H

#include <QMainWindow>
#include <QTimer>

class WaveformChart;
class XYGraph;
class IntensityChart;
class PieChart;
class LineChart;
class Histogram;
class PolarPlot;
class SpectrumAnalyzer;
class ScatterPlot;
class AreaChart;
class RadarChart;
class BubbleChart;
class DigitalWaveform;
class StackedBarChart;
class QTabWidget;
class QPushButton;

/**
 * @brief LabVIEW-style Chart Demo Window
 * Virtual Instrument demonstration with chart components
 */
class ChartDemoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChartDemoWindow(QWidget *parent = nullptr);
    ~ChartDemoWindow() override;

private slots:
    void onSimulationTimer();
    void startSimulation();
    void stopSimulation();
    void resetCharts();

private:
    void setupUi();
    void createWaveformTab();
    void createXYGraphTab();
    void createIntensityChartTab();
    void createStatisticsTab();
    void createAdvancedTab();
    void createSpecializedTab();
    void createMenuBar();

    // Chart widgets
    WaveformChart *m_waveformChart1;
    WaveformChart *m_waveformChart2;
    XYGraph *m_xyGraph;
    IntensityChart *m_intensityChart;
    PieChart *m_pieChart;
    LineChart *m_lineChart;
    Histogram *m_histogram;
    PolarPlot *m_polarPlot;
    SpectrumAnalyzer *m_spectrumAnalyzer;
    ScatterPlot *m_scatterPlot;
    AreaChart *m_areaChart;
    RadarChart *m_radarChart;
    BubbleChart *m_bubbleChart;
    DigitalWaveform *m_digitalWaveform;
    StackedBarChart *m_stackedBarChart;
    
    QTabWidget *m_tabWidget;
    QTimer *m_simulationTimer;
    bool m_simulationRunning;
    double m_simulationTime;
    
    // For XY Graph demo
    QVector<QPointF> m_xyData;
};

#endif // CHARTDEMOWINDOW_H

