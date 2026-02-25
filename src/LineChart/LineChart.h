#ifndef LINECHART_H
#define LINECHART_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QColor>
#include <QMap>

/**
 * @brief Multi-Line Chart - Multiple line series comparison
 */
class LineChart : public QWidget
{
    Q_OBJECT

public:
    explicit LineChart(QWidget *parent = nullptr);
    ~LineChart();

    // Series management
    void addSeries(const QString &name, const QColor &color);
    void setSeriesData(const QString &name, const QVector<double> &data);
    void addDataPoint(const QString &name, double value);
    void clearSeries(const QString &name);
    void clearAllSeries();
    
    // Configuration
    void setXRange(double min, double max);
    void setYRange(double min, double max);
    void setAutoScale(bool autoScale);
    void setGridEnabled(bool enabled);
    void setTitle(const QString &title);
    void setXLabel(const QString &label);
    void setYLabel(const QString &label);
    void setShowLegend(bool show);
    
    QSize sizeHint() const override { return QSize(600, 400); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    struct SeriesData {
        QString name;
        QColor color;
        QVector<double> data;
    };

    void drawBackground(QPainter &painter);
    void drawGrid(QPainter &painter, const QRect &plotArea);
    void drawAxes(QPainter &painter, const QRect &plotArea);
    void drawLines(QPainter &painter, const QRect &plotArea);
    void drawLegend(QPainter &painter, const QRect &legendArea);
    void drawLabels(QPainter &painter, const QRect &plotArea);
    void updateAutoScale();

    QMap<QString, SeriesData> m_series;
    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
    bool m_autoScale;
    bool m_gridEnabled;
    bool m_showLegend;
    QString m_title;
    QString m_xLabel;
    QString m_yLabel;
    
    double m_autoYMin;
    double m_autoYMax;
    int m_maxPoints;
};

#endif // LINECHART_H









