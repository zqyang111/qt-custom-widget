#ifndef COMPONENTDEMOWINDOW_H
#define COMPONENTDEMOWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QGridLayout>
#include <QMap>
#include <QString>

class QPushButton;
class QDialog;

/**
 * @brief Component Demo Window - Displays all available components
 * 
 * This demo window provides a button grid interface where each button
 * opens a separate window showcasing a specific component with its features.
 * Designed for customer demonstrations.
 */
class ComponentDemoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ComponentDemoWindow(QWidget *parent = nullptr);
    ~ComponentDemoWindow() override;

private slots:
    void showComponentDemo(const QString &componentName);

private:
    void setupUi();
    void createComponentButtons();
    QPushButton* createDemoButton(const QString &name, const QString &category, int row, int col);
    
    // Component demo window creators
    QDialog* createGaugeDemo();
    QDialog* createThermometerDemo();
    QDialog* createProgressBarDemo();
    QDialog* createLEDDemo();
    QDialog* createDigitalMeterDemo();
    QDialog* createLevelIndicatorDemo();
    QDialog* createSevenSegmentDemo();
    QDialog* createSwitchButtonDemo();
    QDialog* createKnobDemo();
    QDialog* createValveDemo();
    QDialog* createPumpDemo();
    QDialog* createTankDemo();
    QDialog* createTrendChartDemo();
    QDialog* createIndustrialSliderDemo();
    QDialog* createFlowMeterDemo();
    QDialog* createSpeedometerDemo();
    QDialog* createAlarmIndicatorDemo();
    QDialog* createPipeDemo();
    QDialog* createBarGraphDemo();
    QDialog* createAnalogClockDemo();
    QDialog* createDigitalClockDemo();
    QDialog* createTimerWidgetDemo();
    QDialog* createCompassDemo();
    QDialog* createMotorControllerDemo();
    QDialog* createBatteryIndicatorDemo();
    QDialog* createThreePhaseMeterDemo();
    QDialog* createGearDemo();
    QDialog* createBeltDriveDemo();
    QDialog* createPistonDemo();
    QDialog* createPendulumDemo();
    QDialog* createSpringDemo();
    QDialog* createSliderCrankDemo();
    QDialog* createCamDemo();
    
    // Chart component demos
    QDialog* createWaveformChartDemo();
    QDialog* createXYGraphDemo();
    QDialog* createIntensityChartDemo();
    QDialog* createPieChartDemo();
    QDialog* createLineChartDemo();
    QDialog* createHistogramDemo();
    QDialog* createPolarPlotDemo();
    QDialog* createSpectrumAnalyzerDemo();
    QDialog* createScatterPlotDemo();
    QDialog* createAreaChartDemo();
    QDialog* createRadarChartDemo();
    QDialog* createBubbleChartDemo();
    QDialog* createDigitalWaveformDemo();
    QDialog* createStackedBarChartDemo();

    QScrollArea *m_scrollArea;
    QWidget *m_centralWidget;
    QGridLayout *m_gridLayout;
    
    // Track open dialogs to prevent multiple instances
    QMap<QString, QDialog*> m_openDialogs;
};

#endif // COMPONENTDEMOWINDOW_H

