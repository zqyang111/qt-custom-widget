#include "LineChart.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <algorithm>

LineChart::LineChart(QWidget *parent)
    : QWidget(parent)
    , m_xMin(0.0)
    , m_xMax(100.0)
    , m_yMin(0.0)
    , m_yMax(100.0)
    , m_autoScale(true)
    , m_gridEnabled(true)
    , m_showLegend(true)
    , m_title("Line Chart")
    , m_xLabel("X Axis")
    , m_yLabel("Y Axis")
    , m_autoYMin(0.0)
    , m_autoYMax(100.0)
    , m_maxPoints(100)
{
}

LineChart::~LineChart()
{
}

void LineChart::addSeries(const QString &name, const QColor &color)
{
    SeriesData series;
    series.name = name;
    series.color = color;
    m_series[name] = series;
    update();
}

void LineChart::setSeriesData(const QString &name, const QVector<double> &data)
{
    if (m_series.contains(name)) {
        m_series[name].data = data;
        
        if (m_autoScale) {
            updateAutoScale();
        }
        
        update();
    }
}

void LineChart::addDataPoint(const QString &name, double value)
{
    if (m_series.contains(name)) {
        m_series[name].data.append(value);
        
        // Keep only recent points
        if (m_series[name].data.size() > m_maxPoints) {
            m_series[name].data.removeFirst();
        }
        
        if (m_autoScale) {
            updateAutoScale();
        }
        
        update();
    }
}

void LineChart::clearSeries(const QString &name)
{
    if (m_series.contains(name)) {
        m_series[name].data.clear();
        update();
    }
}

void LineChart::clearAllSeries()
{
    for (auto &series : m_series) {
        series.data.clear();
    }
    update();
}

void LineChart::setXRange(double min, double max)
{
    m_xMin = min;
    m_xMax = max;
    update();
}

void LineChart::setYRange(double min, double max)
{
    m_yMin = min;
    m_yMax = max;
    m_autoScale = false;
    update();
}

void LineChart::setAutoScale(bool autoScale)
{
    m_autoScale = autoScale;
    if (autoScale) {
        updateAutoScale();
    }
    update();
}

void LineChart::setGridEnabled(bool enabled)
{
    m_gridEnabled = enabled;
    update();
}

void LineChart::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void LineChart::setXLabel(const QString &label)
{
    m_xLabel = label;
    update();
}

void LineChart::setYLabel(const QString &label)
{
    m_yLabel = label;
    update();
}

void LineChart::setShowLegend(bool show)
{
    m_showLegend = show;
    update();
}

void LineChart::updateAutoScale()
{
    m_autoYMin = 1e10;
    m_autoYMax = -1e10;
    
    for (const SeriesData &series : m_series) {
        if (!series.data.isEmpty()) {
            auto minMax = std::minmax_element(series.data.begin(), series.data.end());
            m_autoYMin = qMin(m_autoYMin, *minMax.first);
            m_autoYMax = qMax(m_autoYMax, *minMax.second);
        }
    }
    
    if (m_autoYMin > m_autoYMax) {
        m_autoYMin = 0.0;
        m_autoYMax = 100.0;
    }
    
    // Add margin
    double range = m_autoYMax - m_autoYMin;
    if (range < 0.001) range = 1.0;
    m_autoYMin -= range * 0.1;
    m_autoYMax += range * 0.1;
}

void LineChart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int legendWidth = m_showLegend ? 120 : 0;
    QRect plotArea(60, 40, width() - 80 - legendWidth, height() - 80);
    QRect legendArea(width() - legendWidth - 10, 50, legendWidth, 200);

    drawBackground(painter);
    drawGrid(painter, plotArea);
    drawAxes(painter, plotArea);
    drawLines(painter, plotArea);
    drawLabels(painter, plotArea);
    
    if (m_showLegend) {
        drawLegend(painter, legendArea);
    }
}

void LineChart::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(43, 43, 43));
    
    painter.setPen(QColor(224, 224, 224));
    QFont titleFont = painter.font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 25, Qt::AlignCenter, m_title);
}

void LineChart::drawGrid(QPainter &painter, const QRect &plotArea)
{
    if (!m_gridEnabled) return;
    
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
}

void LineChart::drawAxes(QPainter &painter, const QRect &plotArea)
{
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawRect(plotArea);
}

void LineChart::drawLines(QPainter &painter, const QRect &plotArea)
{
    double yMin = m_autoScale ? m_autoYMin : m_yMin;
    double yMax = m_autoScale ? m_autoYMax : m_yMax;
    double yRange = yMax - yMin;
    if (yRange < 0.001) yRange = 1.0;
    
    // Draw each series
    for (const SeriesData &series : m_series) {
        if (series.data.size() < 2) continue;
        
        QPainterPath path;
        bool firstPoint = true;
        
        for (int i = 0; i < series.data.size(); ++i) {
            double x = plotArea.left() + plotArea.width() * i / (m_maxPoints - 1.0);
            double normalizedY = (series.data[i] - yMin) / yRange;
            double y = plotArea.bottom() - normalizedY * plotArea.height();
            
            if (firstPoint) {
                path.moveTo(x, y);
                firstPoint = false;
            } else {
                path.lineTo(x, y);
            }
        }
        
        painter.setPen(QPen(series.color, 2));
        painter.drawPath(path);
    }
}

void LineChart::drawLegend(QPainter &painter, const QRect &legendArea)
{
    painter.setPen(QColor(200, 200, 200));
    QFont legendFont = painter.font();
    legendFont.setPointSize(9);
    painter.setFont(legendFont);
    
    int y = legendArea.top();
    int itemHeight = 25;
    
    for (const SeriesData &series : m_series) {
        // Color line
        painter.setPen(QPen(series.color, 3));
        painter.drawLine(legendArea.left(), y + 7, legendArea.left() + 25, y + 7);
        
        // Label
        painter.setPen(QColor(200, 200, 200));
        painter.drawText(legendArea.left() + 30, y, legendArea.width() - 30, 20, 
                        Qt::AlignLeft | Qt::AlignVCenter, series.name);
        
        y += itemHeight;
        
        if (y > legendArea.bottom()) break;
    }
}

void LineChart::drawLabels(QPainter &painter, const QRect &plotArea)
{
    double yMin = m_autoScale ? m_autoYMin : m_yMin;
    double yMax = m_autoScale ? m_autoYMax : m_yMax;
    
    painter.setPen(QColor(200, 200, 200));
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    
    // Y axis labels
    for (int i = 0; i <= 5; ++i) {
        double value = yMin + (yMax - yMin) * (5 - i) / 5;
        int y = plotArea.top() + plotArea.height() * i / 5;
        QString label = QString::number(value, 'f', 1);
        painter.drawText(5, y - 10, 50, 20, Qt::AlignRight | Qt::AlignVCenter, label);
    }
    
    // Y axis label
    painter.save();
    painter.translate(15, plotArea.center().y());
    painter.rotate(-90);
    painter.drawText(-50, -10, 100, 20, Qt::AlignCenter, m_yLabel);
    painter.restore();
    
    // X axis label
    painter.drawText(plotArea.left(), plotArea.bottom() + 5, 
                     plotArea.width(), 20, Qt::AlignCenter, m_xLabel);
}









