#include "XYGraph.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <algorithm>

XYGraph::XYGraph(QWidget *parent)
    : QWidget(parent)
    , m_xMin(0.0)
    , m_xMax(100.0)
    , m_yMin(0.0)
    , m_yMax(100.0)
    , m_autoScale(true)
    , m_plotColor(QColor(255, 0, 0))  // Red
    , m_plotStyle(2)  // Line and points
    , m_gridEnabled(true)
    , m_title("XY Graph")
    , m_xLabel("X Axis")
    , m_yLabel("Y Axis")
    , m_autoXMin(0.0)
    , m_autoXMax(100.0)
    , m_autoYMin(0.0)
    , m_autoYMax(100.0)
{
}

XYGraph::~XYGraph()
{
}

void XYGraph::setData(const QVector<QPointF> &points)
{
    m_data = points;
    
    if (m_autoScale) {
        updateAutoScale();
    }
    
    update();
}

void XYGraph::addPoint(const QPointF &point)
{
    m_data.append(point);
    
    if (m_autoScale) {
        updateAutoScale();
    }
    
    update();
}

void XYGraph::addPoint(double x, double y)
{
    addPoint(QPointF(x, y));
}

void XYGraph::clearData()
{
    m_data.clear();
    update();
}

void XYGraph::setXRange(double min, double max)
{
    m_xMin = min;
    m_xMax = max;
    m_autoScale = false;
    update();
}

void XYGraph::setYRange(double min, double max)
{
    m_yMin = min;
    m_yMax = max;
    m_autoScale = false;
    update();
}

void XYGraph::setAutoScale(bool autoScale)
{
    m_autoScale = autoScale;
    if (autoScale) {
        updateAutoScale();
    }
    update();
}

void XYGraph::setPlotColor(const QColor &color)
{
    m_plotColor = color;
    update();
}

void XYGraph::setPlotStyle(int style)
{
    m_plotStyle = qBound(0, style, 2);
    update();
}

void XYGraph::setGridEnabled(bool enabled)
{
    m_gridEnabled = enabled;
    update();
}

void XYGraph::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void XYGraph::setXLabel(const QString &label)
{
    m_xLabel = label;
    update();
}

void XYGraph::setYLabel(const QString &label)
{
    m_yLabel = label;
    update();
}

void XYGraph::updateAutoScale()
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
    
    // Add 10% margin
    double xRange = m_autoXMax - m_autoXMin;
    if (xRange < 0.001) xRange = 1.0;
    m_autoXMin -= xRange * 0.1;
    m_autoXMax += xRange * 0.1;
    
    double yRange = m_autoYMax - m_autoYMin;
    if (yRange < 0.001) yRange = 1.0;
    m_autoYMin -= yRange * 0.1;
    m_autoYMax += yRange * 0.1;
}

void XYGraph::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Define plot area
    QRect plotArea(70, 40, width() - 90, height() - 80);

    drawBackground(painter);
    drawGrid(painter, plotArea);
    drawAxes(painter, plotArea);
    
    // Draw center marker (Earth) for satellite orbit demo
    int centerX = plotArea.left() + plotArea.width() / 2;
    int centerY = plotArea.top() + plotArea.height() / 2;
    painter.setPen(QPen(QColor(100, 150, 255), 2));
    painter.setBrush(QColor(50, 100, 200));
    painter.drawEllipse(QPointF(centerX, centerY), 8, 8);
    painter.setPen(QColor(150, 150, 150));
    QFont earthFont = painter.font();
    earthFont.setPointSize(8);
    painter.setFont(earthFont);
    painter.drawText(centerX - 20, centerY + 15, 40, 15, Qt::AlignCenter, "Earth");
    
    drawPlot(painter, plotArea);
    drawLabels(painter, plotArea);
}

void XYGraph::drawBackground(QPainter &painter)
{
    // Dark theme background
    painter.fillRect(rect(), QColor(43, 43, 43));  // #2b2b2b
    
    // Title
    painter.setPen(QColor(224, 224, 224));  // Light gray text
    QFont titleFont = painter.font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 25, Qt::AlignCenter, m_title);
}

void XYGraph::drawGrid(QPainter &painter, const QRect &plotArea)
{
    if (!m_gridEnabled) return;
    
    // Dark plot background
    painter.fillRect(plotArea, QColor(0, 0, 0));
    
    painter.setPen(QPen(QColor(40, 40, 40), 1));
    
    // Vertical grid
    for (int i = 0; i <= 10; ++i) {
        int x = plotArea.left() + plotArea.width() * i / 10;
        painter.drawLine(x, plotArea.top(), x, plotArea.bottom());
    }
    
    // Horizontal grid
    for (int i = 0; i <= 8; ++i) {
        int y = plotArea.top() + plotArea.height() * i / 8;
        painter.drawLine(plotArea.left(), y, plotArea.right(), y);
    }
}

void XYGraph::drawAxes(QPainter &painter, const QRect &plotArea)
{
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawRect(plotArea);
}

void XYGraph::drawPlot(QPainter &painter, const QRect &plotArea)
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
    
    // Convert points to screen coordinates
    QVector<QPointF> screenPoints;
    for (const QPointF &point : m_data) {
        double screenX = plotArea.left() + (point.x() - xMin) / xRange * plotArea.width();
        double screenY = plotArea.bottom() - (point.y() - yMin) / yRange * plotArea.height();
        screenPoints.append(QPointF(screenX, screenY));
    }
    
    // Draw lines
    if (m_plotStyle == 0 || m_plotStyle == 2) {
        painter.setPen(QPen(m_plotColor, 2));
        for (int i = 1; i < screenPoints.size(); ++i) {
            painter.drawLine(screenPoints[i-1], screenPoints[i]);
        }
    }
    
    // Draw points
    if (m_plotStyle == 1 || m_plotStyle == 2) {
        painter.setPen(QPen(m_plotColor, 1));
        painter.setBrush(m_plotColor);
        for (const QPointF &point : screenPoints) {
            painter.drawEllipse(point, 3, 3);
        }
    }
    
    // Draw satellite marker at current position (last point)
    if (!screenPoints.isEmpty()) {
        QPointF currentPos = screenPoints.last();
        
        // Draw satellite icon
        painter.setPen(QPen(QColor(255, 200, 0), 2));
        painter.setBrush(QColor(255, 200, 0));
        
        // Satellite body
        painter.drawRect(currentPos.x() - 4, currentPos.y() - 3, 8, 6);
        
        // Solar panels
        painter.setPen(QPen(QColor(100, 150, 255), 2));
        painter.drawLine(currentPos.x() - 10, currentPos.y(), currentPos.x() - 4, currentPos.y());
        painter.drawLine(currentPos.x() + 4, currentPos.y(), currentPos.x() + 10, currentPos.y());
        
        // Antenna
        painter.setPen(QPen(QColor(200, 200, 200), 1));
        painter.drawLine(currentPos.x(), currentPos.y() - 3, currentPos.x(), currentPos.y() - 8);
        painter.drawEllipse(QPointF(currentPos.x(), currentPos.y() - 8), 2, 2);
    }
}

void XYGraph::drawLabels(QPainter &painter, const QRect &plotArea)
{
    double xMin = m_autoScale ? m_autoXMin : m_xMin;
    double xMax = m_autoScale ? m_autoXMax : m_xMax;
    double yMin = m_autoScale ? m_autoYMin : m_yMin;
    double yMax = m_autoScale ? m_autoYMax : m_yMax;
    
    painter.setPen(QColor(200, 200, 200));  // Light gray for dark theme
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
    
    // X axis label
    painter.drawText(plotArea.left(), plotArea.bottom() + 25, 
                     plotArea.width(), 20, Qt::AlignCenter, m_xLabel);
    
    // Y axis label
    painter.save();
    painter.translate(20, plotArea.center().y());
    painter.rotate(-90);
    painter.drawText(-50, -10, 100, 20, Qt::AlignCenter, m_yLabel);
    painter.restore();
}

