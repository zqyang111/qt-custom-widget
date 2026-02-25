#include "Histogram.h"
#include <QPainter>
#include <QtMath>
#include <algorithm>

Histogram::Histogram(QWidget *parent)
    : QWidget(parent)
    , m_binCount(20)
    , m_rangeMin(0.0)
    , m_rangeMax(100.0)
    , m_autoRange(true)
    , m_barColor(QColor(100, 150, 255))
    , m_title("Histogram")
    , m_xLabel("Value")
    , m_yLabel("Frequency")
    , m_showStatistics(true)
    , m_actualMin(0.0)
    , m_actualMax(100.0)
    , m_mean(0.0)
    , m_stdDev(0.0)
    , m_maxBinCount(0)
{
    m_bins.resize(m_binCount);
}

Histogram::~Histogram()
{
}

void Histogram::setData(const QVector<double> &data)
{
    m_data = data;
    calculateHistogram();
    calculateStatistics();
    update();
}

void Histogram::addValue(double value)
{
    m_data.append(value);
    calculateHistogram();
    calculateStatistics();
    update();
}

void Histogram::clearData()
{
    m_data.clear();
    m_bins.fill(0);
    update();
}

void Histogram::setBinCount(int bins)
{
    m_binCount = qBound(5, bins, 100);
    m_bins.resize(m_binCount);
    calculateHistogram();
    update();
}

void Histogram::setRange(double min, double max)
{
    m_rangeMin = min;
    m_rangeMax = max;
    m_autoRange = false;
    calculateHistogram();
    update();
}

void Histogram::setAutoRange(bool autoRange)
{
    m_autoRange = autoRange;
    calculateHistogram();
    update();
}

void Histogram::setBarColor(const QColor &color)
{
    m_barColor = color;
    update();
}

void Histogram::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void Histogram::setXLabel(const QString &label)
{
    m_xLabel = label;
    update();
}

void Histogram::setYLabel(const QString &label)
{
    m_yLabel = label;
    update();
}

void Histogram::setShowStatistics(bool show)
{
    m_showStatistics = show;
    update();
}

void Histogram::calculateHistogram()
{
    m_bins.fill(0);
    
    if (m_data.isEmpty()) return;
    
    // Determine range
    if (m_autoRange) {
        auto minMax = std::minmax_element(m_data.begin(), m_data.end());
        m_actualMin = *minMax.first;
        m_actualMax = *minMax.second;
    } else {
        m_actualMin = m_rangeMin;
        m_actualMax = m_rangeMax;
    }
    
    double range = m_actualMax - m_actualMin;
    if (range < 0.001) range = 1.0;
    
    // Fill bins
    for (double value : m_data) {
        if (value >= m_actualMin && value <= m_actualMax) {
            int binIndex = static_cast<int>((value - m_actualMin) / range * m_binCount);
            if (binIndex >= m_binCount) binIndex = m_binCount - 1;
            if (binIndex < 0) binIndex = 0;
            m_bins[binIndex]++;
        }
    }
    
    // Find max bin count for scaling
    m_maxBinCount = 0;
    for (int count : m_bins) {
        m_maxBinCount = qMax(m_maxBinCount, count);
    }
}

void Histogram::calculateStatistics()
{
    if (m_data.isEmpty()) {
        m_mean = 0.0;
        m_stdDev = 0.0;
        return;
    }
    
    // Mean
    double sum = 0.0;
    for (double value : m_data) {
        sum += value;
    }
    m_mean = sum / m_data.size();
    
    // Standard deviation
    double variance = 0.0;
    for (double value : m_data) {
        double diff = value - m_mean;
        variance += diff * diff;
    }
    m_stdDev = qSqrt(variance / m_data.size());
}

void Histogram::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect plotArea(60, 40, width() - 80, height() - 120);

    drawBackground(painter);
    drawAxes(painter, plotArea);
    drawHistogram(painter, plotArea);
    drawLabels(painter, plotArea);
    
    if (m_showStatistics) {
        drawStatistics(painter);
    }
}

void Histogram::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(43, 43, 43));
    
    painter.setPen(QColor(224, 224, 224));
    QFont titleFont = painter.font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 25, Qt::AlignCenter, m_title);
}

void Histogram::drawHistogram(QPainter &painter, const QRect &plotArea)
{
    if (m_bins.isEmpty() || m_maxBinCount == 0) return;
    
    // Draw grid
    painter.fillRect(plotArea, QColor(0, 0, 0));
    painter.setPen(QPen(QColor(40, 40, 40), 1));
    for (int i = 0; i <= 8; ++i) {
        int y = plotArea.top() + plotArea.height() * i / 8;
        painter.drawLine(plotArea.left(), y, plotArea.right(), y);
    }
    
    // Draw bars
    double barWidth = static_cast<double>(plotArea.width()) / m_binCount;
    
    for (int i = 0; i < m_binCount; ++i) {
        if (m_bins[i] == 0) continue;
        
        double barHeight = plotArea.height() * m_bins[i] / static_cast<double>(m_maxBinCount);
        
        QRectF barRect(
            plotArea.left() + i * barWidth,
            plotArea.bottom() - barHeight,
            barWidth - 1,
            barHeight
        );
        
        // Gradient fill
        QLinearGradient gradient(barRect.topLeft(), barRect.bottomLeft());
        gradient.setColorAt(0.0, m_barColor.lighter(120));
        gradient.setColorAt(1.0, m_barColor);
        
        painter.setPen(QPen(m_barColor.darker(120), 1));
        painter.setBrush(gradient);
        painter.drawRect(barRect);
    }
}

void Histogram::drawAxes(QPainter &painter, const QRect &plotArea)
{
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawRect(plotArea);
}

void Histogram::drawLabels(QPainter &painter, const QRect &plotArea)
{
    painter.setPen(QColor(200, 200, 200));
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    
    // X axis labels
    for (int i = 0; i <= 5; ++i) {
        double value = m_actualMin + (m_actualMax - m_actualMin) * i / 5;
        int x = plotArea.left() + plotArea.width() * i / 5;
        QString label = QString::number(value, 'f', 1);
        painter.drawText(x - 30, plotArea.bottom() + 5, 60, 20, Qt::AlignCenter, label);
    }
    
    // Y axis labels (frequency)
    for (int i = 0; i <= 5; ++i) {
        int value = m_maxBinCount * (5 - i) / 5;
        int y = plotArea.top() + plotArea.height() * i / 5;
        painter.drawText(5, y - 10, 50, 20, Qt::AlignRight | Qt::AlignVCenter, QString::number(value));
    }
    
    // Axis labels
    painter.drawText(plotArea.left(), plotArea.bottom() + 25, 
                     plotArea.width(), 20, Qt::AlignCenter, m_xLabel);
    
    painter.save();
    painter.translate(15, plotArea.center().y());
    painter.rotate(-90);
    painter.drawText(-50, -10, 100, 20, Qt::AlignCenter, m_yLabel);
    painter.restore();
}

void Histogram::drawStatistics(QPainter &painter)
{
    painter.setPen(QColor(200, 200, 200));
    QFont statsFont = painter.font();
    statsFont.setPointSize(9);
    painter.setFont(statsFont);
    
    int y = height() - 80;
    
    QString stats = QString("N=%1  Mean=%2  StdDev=%3  Min=%4  Max=%5")
        .arg(m_data.size())
        .arg(m_mean, 0, 'f', 2)
        .arg(m_stdDev, 0, 'f', 2)
        .arg(m_actualMin, 0, 'f', 2)
        .arg(m_actualMax, 0, 'f', 2);
    
    painter.drawText(10, y, width() - 20, 30, Qt::AlignCenter, stats);
}









