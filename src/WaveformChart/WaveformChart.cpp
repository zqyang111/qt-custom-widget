#include "WaveformChart.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <algorithm>

WaveformChart::WaveformChart(QWidget *parent)
    : QWidget(parent)
    , m_maxPoints(100)
    , m_yMin(0.0)
    , m_yMax(100.0)
    , m_autoScale(true)
    , m_plotColor(QColor(0, 255, 0))  // Green like LabVIEW
    , m_gridEnabled(true)
    , m_title("Waveform Chart")
    , m_yLabel("Value")
    , m_autoYMin(0.0)
    , m_autoYMax(100.0)
{
}

WaveformChart::~WaveformChart()
{
}

void WaveformChart::addDataPoint(double value)
{
    m_data.append(value);
    
    // Keep only recent points (strip chart mode)
    if (m_data.size() > m_maxPoints) {
        m_data.removeFirst();
    }
    
    if (m_autoScale) {
        updateAutoScale();
    }
    
    update();
}

void WaveformChart::addDataPoints(const QVector<double> &values)
{
    for (double value : values) {
        addDataPoint(value);
    }
}

void WaveformChart::setMaxPoints(int maxPoints)
{
    m_maxPoints = qMax(10, maxPoints);
    while (m_data.size() > m_maxPoints) {
        m_data.removeFirst();
    }
    update();
}

void WaveformChart::setYRange(double min, double max)
{
    m_yMin = min;
    m_yMax = max;
    m_autoScale = false;
    update();
}

void WaveformChart::setAutoScale(bool autoScale)
{
    m_autoScale = autoScale;
    if (autoScale) {
        updateAutoScale();
    }
    update();
}

void WaveformChart::setPlotColor(const QColor &color)
{
    m_plotColor = color;
    update();
}

void WaveformChart::setGridEnabled(bool enabled)
{
    m_gridEnabled = enabled;
    update();
}

void WaveformChart::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void WaveformChart::setYLabel(const QString &label)
{
    m_yLabel = label;
    update();
}

void WaveformChart::clearData()
{
    m_data.clear();
    update();
}

void WaveformChart::updateAutoScale()
{
    if (m_data.isEmpty()) {
        m_autoYMin = 0.0;
        m_autoYMax = 100.0;
        return;
    }
    
    auto minMax = std::minmax_element(m_data.begin(), m_data.end());
    m_autoYMin = *minMax.first;
    m_autoYMax = *minMax.second;
    
    // Add 10% margin
    double range = m_autoYMax - m_autoYMin;
    if (range < 0.001) range = 1.0;
    m_autoYMin -= range * 0.1;
    m_autoYMax += range * 0.1;
}

void WaveformChart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Define plot area (leave margins for labels)
    QRect plotArea(60, 40, width() - 80, height() - 80);

    drawBackground(painter);
    drawGrid(painter, plotArea);
    drawAxes(painter, plotArea);
    drawWaveform(painter, plotArea);
    drawLabels(painter, plotArea);
}

void WaveformChart::drawBackground(QPainter &painter)
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

void WaveformChart::drawGrid(QPainter &painter, const QRect &plotArea)
{
    if (!m_gridEnabled) return;
    
    // Dark plot background like LabVIEW
    painter.fillRect(plotArea, QColor(0, 0, 0));
    
    painter.setPen(QPen(QColor(40, 40, 40), 1));
    
    // Vertical grid lines
    int numVLines = 10;
    for (int i = 0; i <= numVLines; ++i) {
        int x = plotArea.left() + plotArea.width() * i / numVLines;
        painter.drawLine(x, plotArea.top(), x, plotArea.bottom());
    }
    
    // Horizontal grid lines
    int numHLines = 8;
    for (int i = 0; i <= numHLines; ++i) {
        int y = plotArea.top() + plotArea.height() * i / numHLines;
        painter.drawLine(plotArea.left(), y, plotArea.right(), y);
    }
}

void WaveformChart::drawAxes(QPainter &painter, const QRect &plotArea)
{
    // Axes
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawRect(plotArea);
}

void WaveformChart::drawWaveform(QPainter &painter, const QRect &plotArea)
{
    if (m_data.size() < 2) return;
    
    double yMin = m_autoScale ? m_autoYMin : m_yMin;
    double yMax = m_autoScale ? m_autoYMax : m_yMax;
    double yRange = yMax - yMin;
    if (yRange < 0.001) yRange = 1.0;
    
    // Create path for waveform
    QPainterPath path;
    bool firstPoint = true;
    
    for (int i = 0; i < m_data.size(); ++i) {
        // Map data point to screen coordinates
        double x = plotArea.left() + plotArea.width() * i / (m_maxPoints - 1.0);
        double normalizedY = (m_data[i] - yMin) / yRange;
        double y = plotArea.bottom() - normalizedY * plotArea.height();
        
        if (firstPoint) {
            path.moveTo(x, y);
            firstPoint = false;
        } else {
            path.lineTo(x, y);
        }
    }
    
    // Draw waveform line
    painter.setPen(QPen(m_plotColor, 2));
    painter.drawPath(path);
    
    // Draw data points
    painter.setBrush(m_plotColor);
    for (int i = 0; i < m_data.size(); ++i) {
        double x = plotArea.left() + plotArea.width() * i / (m_maxPoints - 1.0);
        double normalizedY = (m_data[i] - yMin) / yRange;
        double y = plotArea.bottom() - normalizedY * plotArea.height();
        painter.drawEllipse(QPointF(x, y), 2, 2);
    }
}

void WaveformChart::drawLabels(QPainter &painter, const QRect &plotArea)
{
    double yMin = m_autoScale ? m_autoYMin : m_yMin;
    double yMax = m_autoScale ? m_autoYMax : m_yMax;
    
    painter.setPen(QColor(200, 200, 200));  // Light gray for dark theme
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    
    // Y axis labels
    int numLabels = 5;
    for (int i = 0; i <= numLabels; ++i) {
        double value = yMin + (yMax - yMin) * (numLabels - i) / numLabels;
        int y = plotArea.top() + plotArea.height() * i / numLabels;
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
                     plotArea.width(), 20, Qt::AlignCenter, "Time");
}

