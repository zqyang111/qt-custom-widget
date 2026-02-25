#ifndef POLARPLOT_H
#define POLARPLOT_H

#include <QWidget>
#include <QVector>
#include <QColor>

/**
 * @brief Polar Plot - Radar/Polar coordinate chart
 * Used for radar scans, antenna patterns, directional data
 */
class PolarPlot : public QWidget
{
    Q_OBJECT

public:
    explicit PolarPlot(QWidget *parent = nullptr);
    ~PolarPlot();

    // Data management
    void setData(const QVector<double> &angles, const QVector<double> &magnitudes);
    void addDataPoint(double angle, double magnitude);
    void clearData();
    
    // Configuration
    void setRadiusRange(double min, double max);
    void setAutoScale(bool autoScale);
    void setPlotColor(const QColor &color);
    void setTitle(const QString &title);
    void setShowGrid(bool show);
    void setFillArea(bool fill);
    
    QSize sizeHint() const override { return QSize(500, 500); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBackground(QPainter &painter);
    void drawPolarGrid(QPainter &painter, const QPoint &center, int radius);
    void drawPlot(QPainter &painter, const QPoint &center, int radius);
    void drawLabels(QPainter &painter, const QPoint &center, int radius);

    QVector<double> m_angles;      // In degrees
    QVector<double> m_magnitudes;
    double m_radiusMin;
    double m_radiusMax;
    bool m_autoScale;
    QColor m_plotColor;
    QString m_title;
    bool m_showGrid;
    bool m_fillArea;
};

#endif // POLARPLOT_H









