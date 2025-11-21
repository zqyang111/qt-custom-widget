#include "AreaChart.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <algorithm>

AreaChart::AreaChart(QWidget *parent)
    : QWidget(parent)
    , m_yMin(0.0)
    , m_yMax(100.0)
    , m_autoScale(true)
    , m_stacked(false)
    , m_title("Area Chart")
    , m_yLabel("Value")
    , m_showLegend(true)
    , m_maxPoints(100)
    , m_autoYMin(0.0)
    , m_autoYMax(100.0)
{
}

AreaChart::~AreaChart()
{
}

void AreaChart::addSeries(const QString &name, const QColor &color)
{
    SeriesData series;
    series.name = name;
    series.color = color;
    m_series[name] = series;
    m_seriesOrder.append(name);
    update();
}

void AreaChart::addDataPoint(const QString &name, double value)
{
    if (m_series.contains(name)) {
        m_series[name].data.append(value);
        
        if (m_series[name].data.size() > m_maxPoints) {
            m_series[name].data.removeFirst();
        }
        
        if (m_autoScale) {
            updateAutoScale();
        }
        
        update();
    }
}

void AreaChart::setSeriesData(const QString &name, const QVector<double> &data)
{
    if (m_series.contains(name)) {
        m_series[name].data = data;
        
        if (m_autoScale) {
            updateAutoScale();
        }
        
        update();
    }
}

void AreaChart::clearSeries(const QString &name)
{
    if (m_series.contains(name)) {
        m_series[name].data.clear();
        update();
    }
}

void AreaChart::clearAllSeries()
{
    for (auto &series : m_series) {
        series.data.clear();
    }
    update();
}

void AreaChart::setYRange(double min, double max)
{
    m_yMin = min;
    m_yMax = max;
    m_autoScale = false;
    update();
}

void AreaChart::setAutoScale(bool autoScale)
{
    m_autoScale = autoScale;
    if (autoScale) {
        updateAutoScale();
    }
    update();
}

void AreaChart::setStacked(bool stacked)
{
    m_stacked = stacked;
    update();
}

void AreaChart::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void AreaChart::setYLabel(const QString &label)
{
    m_yLabel = label;
    update();
}

void AreaChart::setShowLegend(bool show)
{
    m_showLegend = show;
    update();
}

void AreaChart::updateAutoScale()
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
    
    double range = m_autoYMax - m_autoYMin;
    if (range < 0.001) range = 1.0;
    m_autoYMin -= range * 0.1;
    m_autoYMax += range * 0.1;
}

void AreaChart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int legendWidth = m_showLegend ? 120 : 0;
    QRect plotArea(60, 40, width() - 80 - legendWidth, height() - 80);
    QRect legendArea(width() - legendWidth - 10, 50, legendWidth, 200);

    drawBackground(painter);
    drawGrid(painter, plotArea);
    drawAreas(painter, plotArea);
    drawLabels(painter, plotArea);
    
    if (m_showLegend) {
        drawLegend(painter, legendArea);
    }
}

void AreaChart::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(43, 43, 43));
    
    painter.setPen(QColor(224, 224, 224));
    QFont titleFont = painter.font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 25, Qt::AlignCenter, m_title);
}

void AreaChart::drawGrid(QPainter &painter, const QRect &plotArea)
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

void AreaChart::drawAreas(QPainter &painter, const QRect &plotArea)
{
    double yMin = m_autoScale ? m_autoYMin : m_yMin;
    double yMax = m_autoScale ? m_autoYMax : m_yMax;
    double yRange = yMax - yMin;
    if (yRange < 0.001) yRange = 1.0;
    
    QVector<double> stackedValues(m_maxPoints, 0.0);
    
    // Draw each series
    for (const QString &seriesName : m_seriesOrder) {
        const SeriesData &series = m_series[seriesName];
        if (series.data.size() < 2) continue;
        
        QPainterPath path;
        bool firstPoint = true;
        
        // Top line
        for (int i = 0; i < series.data.size(); ++i) {
            double x = plotArea.left() + plotArea.width() * i / (m_maxPoints - 1.0);
            
            double value = series.data[i];
            if (m_stacked) {
                value += stackedValues[i];
                stackedValues[i] = value;
            }
            
            double normalizedY = (value - yMin) / yRange;
            double y = plotArea.bottom() - normalizedY * plotArea.height();
            
            if (firstPoint) {
                path.moveTo(x, y);
                firstPoint = false;
            } else {
                path.lineTo(x, y);
            }
        }
        
        // Close path at bottom
        if (!series.data.isEmpty()) {
            path.lineTo(plotArea.left() + plotArea.width() * (series.data.size() - 1) / (m_maxPoints - 1.0), 
                       plotArea.bottom());
            path.lineTo(plotArea.left(), plotArea.bottom());
            path.closeSubpath();
        }
        
        // Draw filled area
        QColor fillColor = series.color;
        fillColor.setAlpha(m_stacked ? 180 : 80);
        painter.setBrush(fillColor);
        painter.setPen(QPen(series.color, 2));
        painter.drawPath(path);
    }
}

void AreaChart::drawLegend(QPainter &painter, const QRect &legendArea)
{
    painter.setPen(QColor(200, 200, 200));
    QFont legendFont = painter.font();
    legendFont.setPointSize(9);
    painter.setFont(legendFont);
    
    int y = legendArea.top();
    int itemHeight = 25;
    
    for (const QString &seriesName : m_seriesOrder) {
        const SeriesData &series = m_series[seriesName];
        
        // Color box
        painter.setBrush(series.color);
        painter.setPen(QPen(QColor(100, 100, 100), 1));
        painter.drawRect(legendArea.left(), y, 20, 15);
        
        // Label
        painter.setPen(QColor(200, 200, 200));
        painter.drawText(legendArea.left() + 25, y, legendArea.width() - 25, 20, 
                        Qt::AlignLeft | Qt::AlignVCenter, series.name);
        
        y += itemHeight;
        
        if (y > legendArea.bottom()) break;
    }
}

void AreaChart::drawLabels(QPainter &painter, const QRect &plotArea)
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
    
    // Axis labels
    painter.drawText(plotArea.left(), plotArea.bottom() + 5, 
                     plotArea.width(), 20, Qt::AlignCenter, "Time");
    
    painter.save();
    painter.translate(15, plotArea.center().y());
    painter.rotate(-90);
    painter.drawText(-50, -10, 100, 20, Qt::AlignCenter, m_yLabel);
    painter.restore();
}









