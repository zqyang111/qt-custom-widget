#include "BubbleChart.h"
#include <QPainter>
#include <QtMath>

BubbleChart::BubbleChart(QWidget *parent)
    : QWidget(parent)
    , m_xMin(0.0)
    , m_xMax(100.0)
    , m_yMin(0.0)
    , m_yMax(100.0)
    , m_sizeMin(5.0)
    , m_sizeMax(50.0)
    , m_autoScale(false)
    , m_title("Bubble Chart")
    , m_xLabel("X Variable")
    , m_yLabel("Y Variable")
    , m_showLabels(true)
{
}

BubbleChart::~BubbleChart()
{
}

void BubbleChart::addBubble(double x, double y, double size, const QString &label, const QColor &color)
{
    Bubble bubble;
    bubble.x = x;
    bubble.y = y;
    bubble.size = size;
    bubble.label = label;
    bubble.color = color;
    m_bubbles.append(bubble);
    
    if (m_autoScale) {
        updateAutoScale();
    }
    
    update();
}

void BubbleChart::clearBubbles()
{
    m_bubbles.clear();
    update();
}

void BubbleChart::setXRange(double min, double max)
{
    m_xMin = min;
    m_xMax = max;
    update();
}

void BubbleChart::setYRange(double min, double max)
{
    m_yMin = min;
    m_yMax = max;
    update();
}

void BubbleChart::setSizeRange(double min, double max)
{
    m_sizeMin = min;
    m_sizeMax = max;
    update();
}

void BubbleChart::setAutoScale(bool autoScale)
{
    m_autoScale = autoScale;
    if (autoScale) {
        updateAutoScale();
    }
    update();
}

void BubbleChart::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void BubbleChart::setXLabel(const QString &label)
{
    m_xLabel = label;
    update();
}

void BubbleChart::setYLabel(const QString &label)
{
    m_yLabel = label;
    update();
}

void BubbleChart::setShowLabels(bool show)
{
    m_showLabels = show;
    update();
}

void BubbleChart::updateAutoScale()
{
    // Auto scaling implementation if needed
}

void BubbleChart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect plotArea(80, 40, width() - 100, height() - 80);

    drawBackground(painter);
    drawGrid(painter, plotArea);
    drawBubbles(painter, plotArea);
    drawLabels(painter, plotArea);
}

void BubbleChart::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(43, 43, 43));
    
    painter.setPen(QColor(224, 224, 224));
    QFont titleFont = painter.font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 25, Qt::AlignCenter, m_title);
}

void BubbleChart::drawGrid(QPainter &painter, const QRect &plotArea)
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

void BubbleChart::drawBubbles(QPainter &painter, const QRect &plotArea)
{
    if (m_bubbles.isEmpty()) return;
    
    double xRange = m_xMax - m_xMin;
    double yRange = m_yMax - m_yMin;
    if (xRange < 0.001) xRange = 1.0;
    if (yRange < 0.001) yRange = 1.0;
    
    // Draw bubbles
    for (const Bubble &bubble : m_bubbles) {
        // Map position to screen
        double screenX = plotArea.left() + (bubble.x - m_xMin) / xRange * plotArea.width();
        double screenY = plotArea.bottom() - (bubble.y - m_yMin) / yRange * plotArea.height();
        
        // Map size
        double sizeRange = m_sizeMax - m_sizeMin;
        if (sizeRange < 0.001) sizeRange = 1.0;
        double normalizedSize = (bubble.size - m_sizeMin) / sizeRange;
        int bubbleRadius = 5 + static_cast<int>(normalizedSize * 30);
        
        // Draw bubble with transparency
        QColor fillColor = bubble.color;
        fillColor.setAlpha(150);
        painter.setBrush(fillColor);
        painter.setPen(QPen(bubble.color, 2));
        painter.drawEllipse(QPointF(screenX, screenY), bubbleRadius, bubbleRadius);
        
        // Draw label
        if (m_showLabels && !bubble.label.isEmpty()) {
            painter.setPen(QColor(255, 255, 255));
            QFont labelFont = painter.font();
            labelFont.setPointSize(7);
            labelFont.setBold(true);
            painter.setFont(labelFont);
            painter.drawText(static_cast<int>(screenX) - 30, static_cast<int>(screenY) - 8, 
                           60, 16, Qt::AlignCenter, bubble.label);
        }
    }
}

void BubbleChart::drawLabels(QPainter &painter, const QRect &plotArea)
{
    painter.setPen(QColor(200, 200, 200));
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    
    // X axis labels
    for (int i = 0; i <= 5; ++i) {
        double value = m_xMin + (m_xMax - m_xMin) * i / 5;
        int x = plotArea.left() + plotArea.width() * i / 5;
        QString label = QString::number(value, 'f', 0);
        painter.drawText(x - 30, plotArea.bottom() + 5, 60, 20, Qt::AlignCenter, label);
    }
    
    // Y axis labels
    for (int i = 0; i <= 5; ++i) {
        double value = m_yMin + (m_yMax - m_yMin) * (5 - i) / 5;
        int y = plotArea.top() + plotArea.height() * i / 5;
        QString label = QString::number(value, 'f', 0);
        painter.drawText(5, y - 10, 70, 20, Qt::AlignRight | Qt::AlignVCenter, label);
    }
    
    // Axis labels
    painter.drawText(plotArea.left(), plotArea.bottom() + 25, 
                     plotArea.width(), 20, Qt::AlignCenter, m_xLabel);
    
    painter.save();
    painter.translate(25, plotArea.center().y());
    painter.rotate(-90);
    painter.drawText(-60, -10, 120, 20, Qt::AlignCenter, m_yLabel);
    painter.restore();
}









