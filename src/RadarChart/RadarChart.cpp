#include "RadarChart.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

RadarChart::RadarChart(QWidget *parent)
    : QWidget(parent)
    , m_maxValue(100.0)
    , m_title("Radar Chart")
    , m_showLegend(true)
    , m_fillArea(true)
{
}

RadarChart::~RadarChart()
{
}

void RadarChart::setCategories(const QVector<QString> &categories)
{
    m_categories = categories;
    update();
}

void RadarChart::addDataSeries(const QString &name, const QVector<double> &values, const QColor &color)
{
    DataSeries series;
    series.name = name;
    series.values = values;
    series.color = color;
    m_dataSeries.append(series);
    update();
}

void RadarChart::clearAllSeries()
{
    m_dataSeries.clear();
    update();
}

void RadarChart::setMaxValue(double maxValue)
{
    m_maxValue = maxValue;
    update();
}

void RadarChart::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void RadarChart::setShowLegend(bool show)
{
    m_showLegend = show;
    update();
}

void RadarChart::setFillArea(bool fill)
{
    m_fillArea = fill;
    update();
}

void RadarChart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int legendHeight = m_showLegend ? 60 : 0;
    QPoint center(width() / 2, (height() - legendHeight) / 2 + 40);
    int radius = qMin(width(), height() - legendHeight - 80) / 2 - 40;

    drawBackground(painter);
    drawRadarGrid(painter, center, radius);
    drawCategoryLabels(painter, center, radius);
    drawDataSeries(painter, center, radius);
    
    if (m_showLegend) {
        drawLegend(painter);
    }
}

void RadarChart::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(43, 43, 43));
    
    painter.setPen(QColor(224, 224, 224));
    QFont titleFont = painter.font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 25, Qt::AlignCenter, m_title);
}

void RadarChart::drawRadarGrid(QPainter &painter, const QPoint &center, int radius)
{
    if (m_categories.isEmpty()) return;
    
    painter.setPen(QPen(QColor(80, 80, 80), 1));
    
    // Draw concentric circles
    for (int i = 1; i <= 5; ++i) {
        int r = radius * i / 5;
        painter.drawEllipse(center, r, r);
    }
    
    // Draw radial lines for each category
    int numCategories = m_categories.size();
    for (int i = 0; i < numCategories; ++i) {
        double angle = 90.0 - (360.0 * i / numCategories);  // Start from top
        double angleRad = qDegreesToRadians(angle);
        int x = center.x() + static_cast<int>(radius * qCos(angleRad));
        int y = center.y() - static_cast<int>(radius * qSin(angleRad));
        painter.drawLine(center, QPoint(x, y));
    }
}

void RadarChart::drawCategoryLabels(QPainter &painter, const QPoint &center, int radius)
{
    if (m_categories.isEmpty()) return;
    
    painter.setPen(QColor(200, 200, 200));
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    
    int numCategories = m_categories.size();
    int labelDistance = radius + 25;
    
    for (int i = 0; i < numCategories; ++i) {
        double angle = 90.0 - (360.0 * i / numCategories);
        double angleRad = qDegreesToRadians(angle);
        int x = center.x() + static_cast<int>(labelDistance * qCos(angleRad));
        int y = center.y() - static_cast<int>(labelDistance * qSin(angleRad));
        
        painter.drawText(x - 40, y - 10, 80, 20, Qt::AlignCenter, m_categories[i]);
    }
}

void RadarChart::drawDataSeries(QPainter &painter, const QPoint &center, int radius)
{
    if (m_categories.isEmpty()) return;
    
    int numCategories = m_categories.size();
    
    for (const DataSeries &series : m_dataSeries) {
        if (series.values.size() != numCategories) continue;
        
        QPainterPath path;
        bool firstPoint = true;
        
        for (int i = 0; i < numCategories; ++i) {
            double angle = 90.0 - (360.0 * i / numCategories);
            double angleRad = qDegreesToRadians(angle);
            
            double value = qBound(0.0, series.values[i], m_maxValue);
            double r = radius * value / m_maxValue;
            
            int x = center.x() + static_cast<int>(r * qCos(angleRad));
            int y = center.y() - static_cast<int>(r * qSin(angleRad));
            
            if (firstPoint) {
                path.moveTo(x, y);
                firstPoint = false;
            } else {
                path.lineTo(x, y);
            }
        }
        
        path.closeSubpath();
        
        // Draw filled area
        if (m_fillArea) {
            QColor fillColor = series.color;
            fillColor.setAlpha(100);
            painter.setBrush(fillColor);
        } else {
            painter.setBrush(Qt::NoBrush);
        }
        
        painter.setPen(QPen(series.color, 2));
        painter.drawPath(path);
        
        // Draw data points
        painter.setBrush(series.color);
        for (int i = 0; i < numCategories; ++i) {
            double angle = 90.0 - (360.0 * i / numCategories);
            double angleRad = qDegreesToRadians(angle);
            
            double value = qBound(0.0, series.values[i], m_maxValue);
            double r = radius * value / m_maxValue;
            
            int x = center.x() + static_cast<int>(r * qCos(angleRad));
            int y = center.y() - static_cast<int>(r * qSin(angleRad));
            
            painter.drawEllipse(QPoint(x, y), 4, 4);
        }
    }
}

void RadarChart::drawLegend(QPainter &painter)
{
    painter.setPen(QColor(200, 200, 200));
    QFont legendFont = painter.font();
    legendFont.setPointSize(9);
    painter.setFont(legendFont);
    
    int startX = 20;
    int y = height() - 40;
    
    for (const DataSeries &series : m_dataSeries) {
        // Color box
        painter.setBrush(series.color);
        painter.setPen(QPen(QColor(100, 100, 100), 1));
        painter.drawRect(startX, y, 15, 15);
        
        // Label
        painter.setPen(QColor(200, 200, 200));
        painter.drawText(startX + 20, y, 100, 20, Qt::AlignLeft | Qt::AlignVCenter, series.name);
        
        startX += 130;
    }
}

