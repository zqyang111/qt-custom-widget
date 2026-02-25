#include "PolarPlot.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <algorithm>

PolarPlot::PolarPlot(QWidget *parent)
    : QWidget(parent)
    , m_radiusMin(0.0)
    , m_radiusMax(100.0)
    , m_autoScale(true)
    , m_plotColor(QColor(0, 255, 100))
    , m_title("Polar Plot")
    , m_showGrid(true)
    , m_fillArea(false)
{
}

PolarPlot::~PolarPlot()
{
}

void PolarPlot::setData(const QVector<double> &angles, const QVector<double> &magnitudes)
{
    m_angles = angles;
    m_magnitudes = magnitudes;
    update();
}

void PolarPlot::addDataPoint(double angle, double magnitude)
{
    m_angles.append(angle);
    m_magnitudes.append(magnitude);
    update();
}

void PolarPlot::clearData()
{
    m_angles.clear();
    m_magnitudes.clear();
    update();
}

void PolarPlot::setRadiusRange(double min, double max)
{
    m_radiusMin = min;
    m_radiusMax = max;
    m_autoScale = false;
    update();
}

void PolarPlot::setAutoScale(bool autoScale)
{
    m_autoScale = autoScale;
    update();
}

void PolarPlot::setPlotColor(const QColor &color)
{
    m_plotColor = color;
    update();
}

void PolarPlot::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void PolarPlot::setShowGrid(bool show)
{
    m_showGrid = show;
    update();
}

void PolarPlot::setFillArea(bool fill)
{
    m_fillArea = fill;
    update();
}

void PolarPlot::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint center(width() / 2, height() / 2 + 20);
    int radius = qMin(width(), height() - 80) / 2 - 40;

    drawBackground(painter);
    drawPolarGrid(painter, center, radius);
    drawPlot(painter, center, radius);
    drawLabels(painter, center, radius);
}

void PolarPlot::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(43, 43, 43));
    
    painter.setPen(QColor(224, 224, 224));
    QFont titleFont = painter.font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 25, Qt::AlignCenter, m_title);
}

void PolarPlot::drawPolarGrid(QPainter &painter, const QPoint &center, int radius)
{
    if (!m_showGrid) return;
    
    painter.setPen(QPen(QColor(80, 80, 80), 1));
    
    // Draw concentric circles
    for (int i = 1; i <= 5; ++i) {
        int r = radius * i / 5;
        painter.drawEllipse(center, r, r);
    }
    
    // Draw radial lines (every 30 degrees)
    for (int angle = 0; angle < 360; angle += 30) {
        double angleRad = qDegreesToRadians(static_cast<double>(angle));
        int x = center.x() + static_cast<int>(radius * qCos(angleRad));
        int y = center.y() - static_cast<int>(radius * qSin(angleRad));
        painter.drawLine(center, QPoint(x, y));
    }
    
    // Draw outer circle
    painter.setPen(QPen(QColor(120, 120, 120), 2));
    painter.drawEllipse(center, radius, radius);
}

void PolarPlot::drawPlot(QPainter &painter, const QPoint &center, int radius)
{
    if (m_angles.isEmpty() || m_magnitudes.isEmpty()) return;
    
    // Auto scale
    double maxMag = m_radiusMax;
    if (m_autoScale && !m_magnitudes.isEmpty()) {
        maxMag = *std::max_element(m_magnitudes.begin(), m_magnitudes.end());
        if (maxMag < 0.001) maxMag = 1.0;
    }
    
    // Convert to screen coordinates
    QPainterPath path;
    bool firstPoint = true;
    
    for (int i = 0; i < qMin(m_angles.size(), m_magnitudes.size()); ++i) {
        double angle = m_angles[i];
        double magnitude = m_magnitudes[i];
        
        double angleRad = qDegreesToRadians(angle);
        double r = radius * magnitude / maxMag;
        
        int x = center.x() + static_cast<int>(r * qCos(angleRad));
        int y = center.y() - static_cast<int>(r * qSin(angleRad));
        
        if (firstPoint) {
            path.moveTo(x, y);
            firstPoint = false;
        } else {
            path.lineTo(x, y);
        }
    }
    
    // Close the path if filling
    if (m_fillArea && !m_angles.isEmpty()) {
        path.closeSubpath();
    }
    
    // Draw the plot
    if (m_fillArea) {
        QColor fillColor = m_plotColor;
        fillColor.setAlpha(100);
        painter.setBrush(fillColor);
        painter.setPen(QPen(m_plotColor, 2));
        painter.drawPath(path);
    } else {
        painter.setPen(QPen(m_plotColor, 2));
        painter.drawPath(path);
    }
}

void PolarPlot::drawLabels(QPainter &painter, const QPoint &center, int radius)
{
    painter.setPen(QColor(200, 200, 200));
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    
    // Cardinal direction labels
    painter.drawText(center.x() - 10, center.y() - radius - 15, 20, 15, Qt::AlignCenter, QString::fromUtf8("0\xC2\xB0"));
    painter.drawText(center.x() + radius + 5, center.y() - 10, 30, 15, Qt::AlignCenter, QString::fromUtf8("90\xC2\xB0"));
    painter.drawText(center.x() - 15, center.y() + radius + 5, 30, 15, Qt::AlignCenter, QString::fromUtf8("180\xC2\xB0"));
    painter.drawText(center.x() - radius - 35, center.y() - 10, 30, 15, Qt::AlignCenter, QString::fromUtf8("270\xC2\xB0"));
}

