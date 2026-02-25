#include "ScatterPlot.h"
#include <QPainter>
#include <QtMath>
#include <algorithm>

ScatterPlot::ScatterPlot(QWidget *parent)
    : QWidget(parent)
    , m_xMin(0.0)
    , m_xMax(100.0)
    , m_yMin(0.0)
    , m_yMax(100.0)
    , m_autoScale(true)
    , m_pointColor(QColor(100, 200, 255))
    , m_pointSize(4)
    , m_showTrendLine(true)
    , m_title("Scatter Plot")
    , m_xLabel("X Variable")
    , m_yLabel("Y Variable")
    , m_autoXMin(0.0)
    , m_autoXMax(100.0)
    , m_autoYMin(0.0)
    , m_autoYMax(100.0)
{
}

ScatterPlot::~ScatterPlot()
{
}

void ScatterPlot::setData(const QVector<QPointF> &points)
{
    m_data = points;
    
    if (m_autoScale) {
        updateAutoScale();
    }
    
    update();
}

void ScatterPlot::addPoint(double x, double y)
{
    m_data.append(QPointF(x, y));
    
    if (m_autoScale) {
        updateAutoScale();
    }
    
    update();
}

void ScatterPlot::addPoint(const QPointF &point)
{
    addPoint(point.x(), point.y());
}

void ScatterPlot::clearData()
{
    m_data.clear();
    update();
}

void ScatterPlot::setXRange(double min, double max)
{
    m_xMin = min;
    m_xMax = max;
    m_autoScale = false;
    update();
}

void ScatterPlot::setYRange(double min, double max)
{
    m_yMin = min;
    m_yMax = max;
    m_autoScale = false;
    update();
}

void ScatterPlot::setAutoScale(bool autoScale)
{
    m_autoScale = autoScale;
    if (autoScale) {
        updateAutoScale();
    }
    update();
}

void ScatterPlot::setPointColor(const QColor &color)
{
    m_pointColor = color;
    update();
}

void ScatterPlot::setPointSize(int size)
{
    m_pointSize = qBound(2, size, 20);
    update();
}

void ScatterPlot::setShowTrendLine(bool show)
{
    m_showTrendLine = show;
    update();
}

void ScatterPlot::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void ScatterPlot::setXLabel(const QString &label)
{
    m_xLabel = label;
    update();
}

void ScatterPlot::setYLabel(const QString &label)
{
    m_yLabel = label;
    update();
}

void ScatterPlot::updateAutoScale()
{
    if (m_data.isEmpty()) {
        m_autoXMin = 0.0;
        m_autoXMax = 100.0;
        m_autoYMin = 0.0;
        m_autoYMax = 100.0;
        return;
    }
    
    m_autoXMin = m_data[0].x();
    m_autoXMax = m_data[0].x();
    m_autoYMin = m_data[0].y();
    m_autoYMax = m_data[0].y();
    
    for (const QPointF &point : m_data) {
        m_autoXMin = qMin(m_autoXMin, point.x());
        m_autoXMax = qMax(m_autoXMax, point.x());
        m_autoYMin = qMin(m_autoYMin, point.y());
        m_autoYMax = qMax(m_autoYMax, point.y());
    }
    
    // Add margin
    double xRange = m_autoXMax - m_autoXMin;
    if (xRange < 0.001) xRange = 1.0;
    m_autoXMin -= xRange * 0.1;
    m_autoXMax += xRange * 0.1;
    
    double yRange = m_autoYMax - m_autoYMin;
    if (yRange < 0.001) yRange = 1.0;
    m_autoYMin -= yRange * 0.1;
    m_autoYMax += yRange * 0.1;
}

void ScatterPlot::calculateTrendLine(double &slope, double &intercept)
{
    if (m_data.size() < 2) {
        slope = 0.0;
        intercept = 0.0;
        return;
    }
    
    // Linear regression: y = slope * x + intercept
    double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumXX = 0.0;
    int n = m_data.size();
    
    for (const QPointF &point : m_data) {
        sumX += point.x();
        sumY += point.y();
        sumXY += point.x() * point.y();
        sumXX += point.x() * point.x();
    }
    
    double denominator = n * sumXX - sumX * sumX;
    if (qAbs(denominator) < 0.001) {
        slope = 0.0;
        intercept = sumY / n;
    } else {
        slope = (n * sumXY - sumX * sumY) / denominator;
        intercept = (sumY - slope * sumX) / n;
    }
}

void ScatterPlot::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect plotArea(70, 40, width() - 90, height() - 80);

    drawBackground(painter);
    drawGrid(painter, plotArea);
    
    if (m_showTrendLine) {
        drawTrendLine(painter, plotArea);
    }
    
    drawScatter(painter, plotArea);
    drawLabels(painter, plotArea);
}

void ScatterPlot::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(43, 43, 43));
    
    painter.setPen(QColor(224, 224, 224));
    QFont titleFont = painter.font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 25, Qt::AlignCenter, m_title);
}

void ScatterPlot::drawGrid(QPainter &painter, const QRect &plotArea)
{
    painter.fillRect(plotArea, QColor(0, 0, 0));
    
    painter.setPen(QPen(QColor(40, 40, 40), 1));
    
    for (int i = 0; i <= 10; ++i) {
        int x = plotArea.left() + plotArea.width() * i / 10;
        painter.drawLine(x, plotArea.top(), x, plotArea.bottom());
    }
    
    for (int i = 0; i <= 8; ++i) {
        int y = plotArea.top() + plotArea.height() * i / 8;
        painter.drawLine(plotArea.left(), y, plotArea.right(), y);
    }
    
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawRect(plotArea);
}

void ScatterPlot::drawScatter(QPainter &painter, const QRect &plotArea)
{
    if (m_data.isEmpty()) return;
    
    double xMin = m_autoScale ? m_autoXMin : m_xMin;
    double xMax = m_autoScale ? m_autoXMax : m_xMax;
    double yMin = m_autoScale ? m_autoYMin : m_yMin;
    double yMax = m_autoScale ? m_autoYMax : m_yMax;
    
    double xRange = xMax - xMin;
    double yRange = yMax - yMin;
    if (xRange < 0.001) xRange = 1.0;
    if (yRange < 0.001) yRange = 1.0;
    
    // Draw points
    painter.setPen(QPen(m_pointColor.darker(120), 1));
    painter.setBrush(m_pointColor);
    
    for (const QPointF &point : m_data) {
        double screenX = plotArea.left() + (point.x() - xMin) / xRange * plotArea.width();
        double screenY = plotArea.bottom() - (point.y() - yMin) / yRange * plotArea.height();
        
        painter.drawEllipse(QPointF(screenX, screenY), m_pointSize, m_pointSize);
    }
}

void ScatterPlot::drawTrendLine(QPainter &painter, const QRect &plotArea)
{
    if (m_data.size() < 2) return;
    
    double slope, intercept;
    calculateTrendLine(slope, intercept);
    
    double xMin = m_autoScale ? m_autoXMin : m_xMin;
    double xMax = m_autoScale ? m_autoXMax : m_xMax;
    double yMin = m_autoScale ? m_autoYMin : m_yMin;
    double yMax = m_autoScale ? m_autoYMax : m_yMax;
    
    double xRange = xMax - xMin;
    double yRange = yMax - yMin;
    if (xRange < 0.001) xRange = 1.0;
    if (yRange < 0.001) yRange = 1.0;
    
    // Calculate line endpoints at data range boundaries
    double y1 = slope * xMin + intercept;
    double y2 = slope * xMax + intercept;
    
    // Clamp Y values to the visible range
    y1 = qBound(yMin, y1, yMax);
    y2 = qBound(yMin, y2, yMax);
    
    // Convert to screen coordinates
    double screenX1 = plotArea.left();
    double screenY1 = plotArea.bottom() - (y1 - yMin) / yRange * plotArea.height();
    double screenX2 = plotArea.right();
    double screenY2 = plotArea.bottom() - (y2 - yMin) / yRange * plotArea.height();
    
    // Clamp screen coordinates to plot area (safety check)
    screenY1 = qBound(static_cast<double>(plotArea.top()), screenY1, static_cast<double>(plotArea.bottom()));
    screenY2 = qBound(static_cast<double>(plotArea.top()), screenY2, static_cast<double>(plotArea.bottom()));
    
    // Set clipping region to plot area
    painter.save();
    painter.setClipRect(plotArea);
    
    // Draw trend line
    painter.setPen(QPen(QColor(255, 100, 100), 2, Qt::DashLine));
    painter.drawLine(QPointF(screenX1, screenY1), QPointF(screenX2, screenY2));
    
    painter.restore();
}

void ScatterPlot::drawLabels(QPainter &painter, const QRect &plotArea)
{
    double xMin = m_autoScale ? m_autoXMin : m_xMin;
    double xMax = m_autoScale ? m_autoXMax : m_xMax;
    double yMin = m_autoScale ? m_autoYMin : m_yMin;
    double yMax = m_autoScale ? m_autoYMax : m_yMax;
    
    painter.setPen(QColor(200, 200, 200));
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    
    // X axis labels
    for (int i = 0; i <= 5; ++i) {
        double value = xMin + (xMax - xMin) * i / 5;
        int x = plotArea.left() + plotArea.width() * i / 5;
        QString label = QString::number(value, 'f', 1);
        painter.drawText(x - 30, plotArea.bottom() + 5, 60, 20, Qt::AlignCenter, label);
    }
    
    // Y axis labels
    for (int i = 0; i <= 5; ++i) {
        double value = yMin + (yMax - yMin) * (5 - i) / 5;
        int y = plotArea.top() + plotArea.height() * i / 5;
        QString label = QString::number(value, 'f', 1);
        painter.drawText(5, y - 10, 60, 20, Qt::AlignRight | Qt::AlignVCenter, label);
    }
    
    // Axis labels
    painter.drawText(plotArea.left(), plotArea.bottom() + 25, 
                     plotArea.width(), 20, Qt::AlignCenter, m_xLabel);
    
    painter.save();
    painter.translate(20, plotArea.center().y());
    painter.rotate(-90);
    painter.drawText(-50, -10, 100, 20, Qt::AlignCenter, m_yLabel);
    painter.restore();
}

