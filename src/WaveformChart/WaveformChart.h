#ifndef WAVEFORMCHART_H
#define WAVEFORMCHART_H

#include <QWidget>
#include <QVector>
#include <QColor>
#include <QTimer>

/**
 * @brief LabVIEW-style Waveform Chart - Real-time scrolling waveform display
 * Similar to LabVIEW's Waveform Chart with strip chart mode
 */
class WaveformChart : public QWidget
{
    Q_OBJECT

public:
    explicit WaveformChart(QWidget *parent = nullptr);
    ~WaveformChart();

    // Add data point
    void addDataPoint(double value);
    void addDataPoints(const QVector<double> &values);
    
    // Configuration
    void setMaxPoints(int maxPoints);
    void setYRange(double min, double max);
    void setAutoScale(bool autoScale);
    void setPlotColor(const QColor &color);
    void setGridEnabled(bool enabled);
    void setTitle(const QString &title);
    void setYLabel(const QString &label);
    
    // Clear data
    void clearData();
    
    QSize sizeHint() const override { return QSize(600, 400); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBackground(QPainter &painter);
    void drawGrid(QPainter &painter, const QRect &plotArea);
    void drawAxes(QPainter &painter, const QRect &plotArea);
    void drawWaveform(QPainter &painter, const QRect &plotArea);
    void drawLabels(QPainter &painter, const QRect &plotArea);
    void updateAutoScale();

    QVector<double> m_data;
    int m_maxPoints;
    double m_yMin;
    double m_yMax;
    bool m_autoScale;
    QColor m_plotColor;
    bool m_gridEnabled;
    QString m_title;
    QString m_yLabel;
    
    double m_autoYMin;
    double m_autoYMax;
};

#endif // WAVEFORMCHART_H











