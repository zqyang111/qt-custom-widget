#include "TrendChartWidget.h"
#include <QtMath>
#include <QPainterPath>
#include <QRect>
#include <limits>

TrendChartWidget::TrendChartWidget(QWidget *parent)
    : QWidget(parent)
    , m_maxDataPoints(100)
    , m_yMin(0.0)
    , m_yMax(100.0)
    , m_autoScale(true)
    , m_backgroundColor(QColor(30, 30, 30))
    , m_gridColor(QColor(60, 60, 60))
    , m_axisColor(QColor(150, 150, 150))
{
    setMinimumSize(200, 100);
}

TrendChartWidget::~TrendChartWidget()
{
}

void TrendChartWidget::addSeries(const QString &name, const QColor &color)
{
    if (!m_series.contains(name)) {
        DataSeries series;
        series.name = name;
        series.color = color;
        series.visible = true;
        series.minValue = 0.0;
        series.maxValue = 0.0;
        m_series[name] = series;
        update();
    }
}

void TrendChartWidget::addDataPoint(const QString &seriesName, double value)
{
    if (m_series.contains(seriesName)) {
        DataSeries &series = m_series[seriesName];  // Non-const access is OK for modification
        series.data.append(value);

        // Update min/max for this series
        if (series.data.size() == 1) {
            series.minValue = value;
            series.maxValue = value;
        } else {
            series.minValue = qMin(series.minValue, value);
            series.maxValue = qMax(series.maxValue, value);
        }

        // Limit data points
        if (series.data.size() > m_maxDataPoints) {
            series.data.removeFirst();
            
            // Recalculate min/max
            if (!series.data.isEmpty()) {
                series.minValue = *std::min_element(series.data.begin(), series.data.end());
                series.maxValue = *std::max_element(series.data.begin(), series.data.end());
            }
        }

        // Auto-scale Y axis
        if (m_autoScale) {
            double globalMin = std::numeric_limits<double>::max();
            double globalMax = std::numeric_limits<double>::lowest();

            for (auto it = m_series.begin(); it != m_series.end(); ++it) {
                const DataSeries &s = it.value();
                if (s.visible && !s.data.isEmpty()) {
                    globalMin = qMin(globalMin, s.minValue);
                    globalMax = qMax(globalMax, s.maxValue);
                }
            }

            if (globalMin < std::numeric_limits<double>::max()) {
                double range = globalMax - globalMin;
                if (range < 1.0) range = 1.0;  // Minimum range
                
                m_yMin = globalMin - range * 0.1;
                m_yMax = globalMax + range * 0.1;
            }
        }

        update();
    }
}

void TrendChartWidget::clearSeries(const QString &seriesName)
{
    if (m_series.contains(seriesName)) {
        m_series[seriesName].data.clear();  // Non-const access for modification
        update();
    }
}

void TrendChartWidget::clearAllSeries()
{
    for (auto it = m_series.begin(); it != m_series.end(); ++it) {
        it.value().data.clear();
    }
    update();
}

void TrendChartWidget::setMaxDataPoints(int maxPoints)
{
    if (maxPoints > 0 && m_maxDataPoints != maxPoints) {
        m_maxDataPoints = maxPoints;
        
        // Trim existing data if necessary
        for (auto it = m_series.begin(); it != m_series.end(); ++it) {
            DataSeries &series = it.value();
            while (series.data.size() > m_maxDataPoints) {
                series.data.removeFirst();
            }
        }
        
        update();
    }
}

void TrendChartWidget::setYAxisRange(double minValue, double maxValue)
{
    if (minValue < maxValue) {
        m_yMin = minValue;
        m_yMax = maxValue;
        m_autoScale = false;
        update();
    }
}

void TrendChartWidget::setAutoScale(bool enabled)
{
    if (m_autoScale != enabled) {
        m_autoScale = enabled;
        update();
    }
}

void TrendChartWidget::setSeriesVisible(const QString &seriesName, bool visible)
{
    if (m_series.contains(seriesName)) {
        m_series[seriesName].visible = visible;  // Non-const access for modification
        update();
    }
}

void TrendChartWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawGrid(painter);
    drawAxes(painter);
    drawSeries(painter);
    drawLegend(painter);
}

void TrendChartWidget::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), m_backgroundColor);
}

void TrendChartWidget::drawGrid(QPainter &painter)
{
    int marginLeft = 50;
    int marginRight = 20;
    int marginTop = 20;
    int marginBottom = 30;

    int chartWidth = width() - marginLeft - marginRight;
    int chartHeight = height() - marginTop - marginBottom;

    painter.setPen(QPen(m_gridColor, 1, Qt::DotLine));

    // Vertical grid lines
    int numVLines = 10;
    for (int i = 0; i <= numVLines; ++i) {
        int x = marginLeft + (i * chartWidth / numVLines);
        painter.drawLine(x, marginTop, x, marginTop + chartHeight);
    }

    // Horizontal grid lines
    int numHLines = 5;
    for (int i = 0; i <= numHLines; ++i) {
        int y = marginTop + (i * chartHeight / numHLines);
        painter.drawLine(marginLeft, y, marginLeft + chartWidth, y);
    }
}

void TrendChartWidget::drawAxes(QPainter &painter)
{
    int marginLeft = 50;
    int marginRight = 20;
    int marginTop = 20;
    int marginBottom = 30;

    int chartWidth = width() - marginLeft - marginRight;
    int chartHeight = height() - marginTop - marginBottom;

    painter.setPen(QPen(m_axisColor, 2));
    
    // Draw axes
    painter.drawLine(marginLeft, marginTop, marginLeft, marginTop + chartHeight);
    painter.drawLine(marginLeft, marginTop + chartHeight, 
                    marginLeft + chartWidth, marginTop + chartHeight);

    // Draw Y-axis labels
    painter.setPen(m_axisColor);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    int numYLabels = 5;
    for (int i = 0; i <= numYLabels; ++i) {
        int y = marginTop + chartHeight - (i * chartHeight / numYLabels);
        double value = m_yMin + (m_yMax - m_yMin) * i / numYLabels;
        
        QString label = QString::number(value, 'f', 1);
        painter.drawText(5, y - 8, marginLeft - 10, 16,
                        Qt::AlignRight | Qt::AlignVCenter, label);
    }

    // Draw X-axis labels (time points)
    int numXLabels = 5;
    for (int i = 0; i <= numXLabels; ++i) {
        int x = marginLeft + (i * chartWidth / numXLabels);
        int timePoint = m_maxDataPoints * i / numXLabels;
        
        QString label = QString::number(timePoint);
        painter.drawText(x - 20, marginTop + chartHeight + 5, 40, 20,
                        Qt::AlignCenter, label);
    }
}

void TrendChartWidget::drawSeries(QPainter &painter)
{
    int marginLeft = 50;
    int marginRight = 20;
    int marginTop = 20;
    int marginBottom = 30;

    painter.setClipRect(marginLeft, marginTop,
                       width() - marginLeft - marginRight,
                       height() - marginTop - marginBottom);

    int seriesIndex = 0;
    for (auto it = m_series.begin(); it != m_series.end(); ++it) {
        const DataSeries &series = it.value();
        if (!series.visible || series.data.isEmpty())
            continue;

        painter.setPen(QPen(series.color, 2));
        painter.setBrush(Qt::NoBrush);

        QPainterPath path;
        bool firstPoint = true;

        for (int i = 0; i < series.data.size(); ++i) {
            QPointF point = mapToWidget(i, series.data[i], seriesIndex);

            if (firstPoint) {
                path.moveTo(point);
                firstPoint = false;
            } else {
                path.lineTo(point);
            }
        }

        painter.drawPath(path);

        // Draw data points
        painter.setBrush(series.color);
        for (int i = 0; i < series.data.size(); ++i) {
            if (i % 5 == 0 || i == series.data.size() - 1) {  // Draw every 5th point
                QPointF point = mapToWidget(i, series.data[i], seriesIndex);
                painter.drawEllipse(point, 3, 3);
            }
        }

        seriesIndex++;
    }

    painter.setClipping(false);
}

void TrendChartWidget::drawLegend(QPainter &painter)
{
    if (m_series.isEmpty())
        return;

    int legendX = width() - 120;
    int legendY = 25;
    int legendWidth = 110;
    int lineHeight = 20;
    int legendHeight = m_series.size() * lineHeight + 10;

    // Draw legend background
    painter.setBrush(QColor(40, 40, 40, 200));
    painter.setPen(QPen(m_axisColor, 1));
    painter.drawRoundedRect(legendX, legendY, legendWidth, legendHeight, 5, 5);

    // Draw series names
    painter.setFont(QFont(painter.font().family(), 8));
    int y = legendY + 15;

    for (auto it = m_series.begin(); it != m_series.end(); ++it) {
        const DataSeries &series = it.value();
        // Color indicator
        painter.setBrush(series.color);
        painter.setPen(Qt::NoPen);
        painter.drawRect(legendX + 5, y - 6, 12, 12);

        // Series name
        painter.setPen(series.visible ? Qt::white : QColor(100, 100, 100));
        painter.drawText(QRect(legendX + 22, y - 8, legendWidth - 27, 16),
                        Qt::AlignLeft | Qt::AlignVCenter, series.name);

        y += lineHeight;
    }
}

QPointF TrendChartWidget::mapToWidget(int index, double value, int seriesIndex)
{
    Q_UNUSED(seriesIndex);

    int marginLeft = 50;
    int marginRight = 20;
    int marginTop = 20;
    int marginBottom = 30;

    int chartWidth = width() - marginLeft - marginRight;
    int chartHeight = height() - marginTop - marginBottom;

    // Get the series to determine data size
    int dataSize = m_maxDataPoints;
    for (auto it = m_series.begin(); it != m_series.end(); ++it) {
        const DataSeries &series = it.value();
        if (!series.data.isEmpty()) {
            dataSize = qMax(dataSize, static_cast<int>(series.data.size()));
        }
    }

    double x = marginLeft + (index * chartWidth / static_cast<double>(m_maxDataPoints - 1));
    
    double yRange = m_yMax - m_yMin;
    if (yRange == 0) yRange = 1.0;
    
    double yNormalized = (value - m_yMin) / yRange;
    double y = marginTop + chartHeight - (yNormalized * chartHeight);

    return QPointF(x, y);
}

