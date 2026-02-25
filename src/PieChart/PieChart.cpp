#include "PieChart.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

PieChart::PieChart(QWidget *parent)
    : QWidget(parent)
    , m_title("Pie Chart")
    , m_showPercentage(true)
    , m_showLegend(true)
    , m_3dEffect(false)
{
}

PieChart::~PieChart()
{
}

void PieChart::addSlice(const QString &label, double value, const QColor &color)
{
    PieSlice slice;
    slice.label = label;
    slice.value = qMax(0.0, value);
    slice.color = color;
    m_slices.append(slice);
    update();
}

void PieChart::setSliceValue(const QString &label, double value)
{
    for (PieSlice &slice : m_slices) {
        if (slice.label == label) {
            slice.value = qMax(0.0, value);
            update();
            return;
        }
    }
}

void PieChart::clearSlices()
{
    m_slices.clear();
    update();
}

void PieChart::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void PieChart::setShowPercentage(bool show)
{
    m_showPercentage = show;
    update();
}

void PieChart::setShowLegend(bool show)
{
    m_showLegend = show;
    update();
}

void PieChart::set3DEffect(bool enabled)
{
    m_3dEffect = enabled;
    update();
}

double PieChart::getTotalValue() const
{
    double total = 0.0;
    for (const PieSlice &slice : m_slices) {
        total += slice.value;
    }
    return total;
}

void PieChart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Define areas
    int legendWidth = m_showLegend ? 150 : 0;
    QRect pieArea(20, 50, width() - 40 - legendWidth, height() - 70);
    QRect legendArea(width() - legendWidth - 10, 50, legendWidth, height() - 70);

    drawBackground(painter);
    drawPie(painter, pieArea);
    
    if (m_showLegend) {
        drawLegend(painter, legendArea);
    }
}

void PieChart::drawBackground(QPainter &painter)
{
    // Dark theme background
    painter.fillRect(rect(), QColor(43, 43, 43));
    
    // Title
    painter.setPen(QColor(224, 224, 224));
    QFont titleFont = painter.font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 30, Qt::AlignCenter, m_title);
}

void PieChart::drawPie(QPainter &painter, const QRect &pieArea)
{
    if (m_slices.isEmpty()) return;
    
    double total = getTotalValue();
    if (total < 0.001) return;
    
    // Calculate pie center and radius
    int centerX = pieArea.center().x();
    int centerY = pieArea.center().y();
    int radius = qMin(pieArea.width(), pieArea.height()) / 2 - 20;
    
    // Draw 3D effect base if enabled
    if (m_3dEffect) {
        int depth = 15;
        painter.setPen(Qt::NoPen);
        
        double startAngle = 90.0;
        for (const PieSlice &slice : m_slices) {
            double percentage = slice.value / total;
            double spanAngle = percentage * 360.0;
            
            // Draw darker shade for 3D depth
            QColor darkColor = slice.color.darker(150);
            painter.setBrush(darkColor);
            painter.drawPie(centerX - radius, centerY - radius + depth, 
                          radius * 2, radius * 2, 
                          static_cast<int>(startAngle * 16), 
                          static_cast<int>(spanAngle * 16));
            
            startAngle += spanAngle;
        }
    }
    
    // Draw pie slices
    double startAngle = 90.0;  // Start from top
    
    for (int i = 0; i < m_slices.size(); ++i) {
        const PieSlice &slice = m_slices[i];
        double percentage = slice.value / total;
        double spanAngle = percentage * 360.0;
        
        // Draw slice
        painter.setPen(QPen(QColor(60, 60, 60), 2));
        painter.setBrush(slice.color);
        painter.drawPie(centerX - radius, centerY - radius, 
                       radius * 2, radius * 2, 
                       static_cast<int>(startAngle * 16), 
                       static_cast<int>(spanAngle * 16));
        
        // Draw percentage text if enabled
        if (m_showPercentage && percentage > 0.05) {  // Only show if > 5%
            double midAngle = startAngle + spanAngle / 2.0;
            double midAngleRad = qDegreesToRadians(midAngle);
            
            int textX = centerX + static_cast<int>(radius * 0.7 * qCos(midAngleRad));
            int textY = centerY - static_cast<int>(radius * 0.7 * qSin(midAngleRad));
            
            QString percentText = QString::number(percentage * 100, 'f', 1) + "%";
            
            painter.setPen(QColor(255, 255, 255));
            QFont percentFont = painter.font();
            percentFont.setBold(true);
            percentFont.setPointSize(9);
            painter.setFont(percentFont);
            painter.drawText(textX - 30, textY - 10, 60, 20, Qt::AlignCenter, percentText);
        }
        
        startAngle += spanAngle;
    }
}

void PieChart::drawLegend(QPainter &painter, const QRect &legendArea)
{
    if (m_slices.isEmpty()) return;
    
    painter.setPen(QColor(200, 200, 200));
    QFont legendFont = painter.font();
    legendFont.setPointSize(9);
    painter.setFont(legendFont);
    
    int y = legendArea.top();
    int itemHeight = 25;
    
    for (const PieSlice &slice : m_slices) {
        // Color box
        painter.setBrush(slice.color);
        painter.setPen(QPen(QColor(100, 100, 100), 1));
        painter.drawRect(legendArea.left(), y, 20, 15);
        
        // Label and value
        painter.setPen(QColor(200, 200, 200));
        QString text = QString("%1: %2").arg(slice.label).arg(slice.value, 0, 'f', 1);
        painter.drawText(legendArea.left() + 25, y, legendArea.width() - 25, 20, 
                        Qt::AlignLeft | Qt::AlignVCenter, text);
        
        y += itemHeight;
        
        if (y > legendArea.bottom()) break;
    }
}










