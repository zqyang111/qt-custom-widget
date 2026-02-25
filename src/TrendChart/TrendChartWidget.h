#ifndef TRENDCHARTWIDGET_H
#define TRENDCHARTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QTimer>
#include <QMap>

/**
 * @brief Industrial trend chart widget
 * 
 * This widget displays real-time data trends with multiple traces,
 * suitable for monitoring time-series data.
 */
class TrendChartWidget : public QWidget
{
    Q_OBJECT

public:
    struct DataSeries {
        QString name;
        QColor color;
        QVector<double> data;
        bool visible;
        double minValue;
        double maxValue;
    };

    explicit TrendChartWidget(QWidget *parent = nullptr);
    ~TrendChartWidget() override;

    // Data management
    void addSeries(const QString &name, const QColor &color);
    void addDataPoint(const QString &seriesName, double value);
    void clearSeries(const QString &seriesName);
    void clearAllSeries();

    // Setters
    void setMaxDataPoints(int maxPoints);
    void setYAxisRange(double minValue, double maxValue);
    void setAutoScale(bool enabled);
    void setSeriesVisible(const QString &seriesName, bool visible);

    // Getters
    int maxDataPoints() const { return m_maxDataPoints; }
    bool autoScale() const { return m_autoScale; }

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(400, 200); }
    QSize minimumSizeHint() const override { return QSize(200, 100); }

private:
    void drawBackground(QPainter &painter);
    void drawGrid(QPainter &painter);
    void drawAxes(QPainter &painter);
    void drawSeries(QPainter &painter);
    void drawLegend(QPainter &painter);
    QPointF mapToWidget(int index, double value, int seriesIndex);

    QMap<QString, DataSeries> m_series;
    int m_maxDataPoints;
    double m_yMin;
    double m_yMax;
    bool m_autoScale;

    QColor m_backgroundColor;
    QColor m_gridColor;
    QColor m_axisColor;
};

#endif // TRENDCHARTWIDGET_H












