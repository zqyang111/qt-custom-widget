#ifndef SCATTERPLOT_H
#define SCATTERPLOT_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QColor>

/**
 * @brief Scatter Plot - Statistical scatter chart
 * Displays correlation between two variables
 */
class ScatterPlot : public QWidget
{
    Q_OBJECT

public:
    explicit ScatterPlot(QWidget *parent = nullptr);
    ~ScatterPlot();

    // Data management
    void setData(const QVector<QPointF> &points);
    void addPoint(double x, double y);
    void addPoint(const QPointF &point);
    void clearData();
    
    // Configuration
    void setXRange(double min, double max);
    void setYRange(double min, double max);
    void setAutoScale(bool autoScale);
    void setPointColor(const QColor &color);
    void setPointSize(int size);
    void setShowTrendLine(bool show);
    void setTitle(const QString &title);
    void setXLabel(const QString &label);
    void setYLabel(const QString &label);
    
    QSize sizeHint() const override { return QSize(500, 400); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBackground(QPainter &painter);
    void drawGrid(QPainter &painter, const QRect &plotArea);
    void drawScatter(QPainter &painter, const QRect &plotArea);
    void drawTrendLine(QPainter &painter, const QRect &plotArea);
    void drawLabels(QPainter &painter, const QRect &plotArea);
    void calculateTrendLine(double &slope, double &intercept);
    void updateAutoScale();

    QVector<QPointF> m_data;
    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
    bool m_autoScale;
    QColor m_pointColor;
    int m_pointSize;
    bool m_showTrendLine;
    QString m_title;
    QString m_xLabel;
    QString m_yLabel;
    
    double m_autoXMin;
    double m_autoXMax;
    double m_autoYMin;
    double m_autoYMax;
};

#endif // SCATTERPLOT_H









