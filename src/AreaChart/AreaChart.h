#ifndef AREACHART_H
#define AREACHART_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QColor>
#include <QMap>

/**
 * @brief Area Chart - Stacked or overlapping area chart
 * Shows trends with filled areas under curves
 */
class AreaChart : public QWidget
{
    Q_OBJECT

public:
    explicit AreaChart(QWidget *parent = nullptr);
    ~AreaChart();

    // Series management
    void addSeries(const QString &name, const QColor &color);
    void addDataPoint(const QString &name, double value);
    void setSeriesData(const QString &name, const QVector<double> &data);
    void clearSeries(const QString &name);
    void clearAllSeries();
    
    // Configuration
    void setYRange(double min, double max);
    void setAutoScale(bool autoScale);
    void setStacked(bool stacked);
    void setTitle(const QString &title);
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
    void drawAreas(QPainter &painter, const QRect &plotArea);
    void drawLegend(QPainter &painter, const QRect &legendArea);
    void drawLabels(QPainter &painter, const QRect &plotArea);
    void updateAutoScale();

    QMap<QString, SeriesData> m_series;
    QVector<QString> m_seriesOrder;  // Preserve insertion order
    double m_yMin;
    double m_yMax;
    bool m_autoScale;
    bool m_stacked;
    QString m_title;
    QString m_yLabel;
    bool m_showLegend;
    int m_maxPoints;
    
    double m_autoYMin;
    double m_autoYMax;
};

#endif // AREACHART_H









