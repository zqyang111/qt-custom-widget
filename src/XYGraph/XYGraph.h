#ifndef XYGRAPH_H
#define XYGRAPH_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QColor>

/**
 * @brief LabVIEW-style XY Graph
 * Displays data as X-Y coordinate plots
 */
class XYGraph : public QWidget
{
    Q_OBJECT

public:
    explicit XYGraph(QWidget *parent = nullptr);
    ~XYGraph();

    // Data management
    void setData(const QVector<QPointF> &points);
    void addPoint(const QPointF &point);
    void addPoint(double x, double y);
    void clearData();
    
    // Configuration
    void setXRange(double min, double max);
    void setYRange(double min, double max);
    void setAutoScale(bool autoScale);
    void setPlotColor(const QColor &color);
    void setPlotStyle(int style);  // 0=Line, 1=Points, 2=LineAndPoints
    void setGridEnabled(bool enabled);
    void setTitle(const QString &title);
    void setXLabel(const QString &label);
    void setYLabel(const QString &label);
    
    QSize sizeHint() const override { return QSize(500, 400); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBackground(QPainter &painter);
    void drawGrid(QPainter &painter, const QRect &plotArea);
    void drawAxes(QPainter &painter, const QRect &plotArea);
    void drawPlot(QPainter &painter, const QRect &plotArea);
    void drawLabels(QPainter &painter, const QRect &plotArea);
    void updateAutoScale();

    QVector<QPointF> m_data;
    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
    bool m_autoScale;
    QColor m_plotColor;
    int m_plotStyle;
    bool m_gridEnabled;
    QString m_title;
    QString m_xLabel;
    QString m_yLabel;
    
    double m_autoXMin;
    double m_autoXMax;
    double m_autoYMin;
    double m_autoYMax;
};

#endif // XYGRAPH_H











