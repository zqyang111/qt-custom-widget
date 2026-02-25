#include "StackedBarChart.h"
#include <QPainter>
#include <QtMath>

StackedBarChart::StackedBarChart(QWidget *parent)
    : QWidget(parent)
    , m_maxValue(100.0)
    , m_autoScale(true)
    , m_title("Stacked Bar Chart")
    , m_yLabel("Value")
    , m_showLegend(true)
    , m_showValues(false)
    , m_autoMaxValue(100.0)
{
}

StackedBarChart::~StackedBarChart()
{
}

void StackedBarChart::addSeries(const QString &name, const QColor &color)
{
    SeriesInfo series;
    series.name = name;
    series.color = color;
    m_series.append(series);
    update();
}

void StackedBarChart::setBarData(const QString &categoryName, const QMap<QString, double> &seriesValues)
{
    m_data[categoryName] = seriesValues;
    
    if (m_autoScale) {
        updateAutoScale();
    }
    
    update();
}

void StackedBarChart::clearAllData()
{
    m_data.clear();
    update();
}

void StackedBarChart::setCategories(const QVector<QString> &categories)
{
    m_categories = categories;
    update();
}

void StackedBarChart::setMaxValue(double maxValue)
{
    m_maxValue = maxValue;
    m_autoScale = false;
    update();
}

void StackedBarChart::setAutoScale(bool autoScale)
{
    m_autoScale = autoScale;
    if (autoScale) {
        updateAutoScale();
    }
    update();
}

void StackedBarChart::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void StackedBarChart::setYLabel(const QString &label)
{
    m_yLabel = label;
    update();
}

void StackedBarChart::setShowLegend(bool show)
{
    m_showLegend = show;
    update();
}

void StackedBarChart::setShowValues(bool show)
{
    m_showValues = show;
    update();
}

void StackedBarChart::updateAutoScale()
{
    m_autoMaxValue = 0.0;
    
    for (const auto &categoryData : m_data) {
        double total = 0.0;
        for (double value : categoryData) {
            total += value;
        }
        m_autoMaxValue = qMax(m_autoMaxValue, total);
    }
    
    if (m_autoMaxValue < 1.0) m_autoMaxValue = 100.0;
}

void StackedBarChart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int legendHeight = m_showLegend ? 50 : 0;
    QRect plotArea(80, 40, width() - 100, height() - 80 - legendHeight);

    drawBackground(painter);
    drawGrid(painter, plotArea);
    drawBars(painter, plotArea);
    drawLabels(painter, plotArea);
    
    if (m_showLegend) {
        drawLegend(painter);
    }
}

void StackedBarChart::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(43, 43, 43));
    
    painter.setPen(QColor(224, 224, 224));
    QFont titleFont = painter.font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 25, Qt::AlignCenter, m_title);
}

void StackedBarChart::drawGrid(QPainter &painter, const QRect &plotArea)
{
    painter.fillRect(plotArea, QColor(0, 0, 0));
    
    painter.setPen(QPen(QColor(40, 40, 40), 1));
    
    // Horizontal grid
    for (int i = 0; i <= 8; ++i) {
        int y = plotArea.top() + plotArea.height() * i / 8;
        painter.drawLine(plotArea.left(), y, plotArea.right(), y);
    }
    
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawRect(plotArea);
}

void StackedBarChart::drawBars(QPainter &painter, const QRect &plotArea)
{
    if (m_categories.isEmpty()) return;
    
    double maxVal = m_autoScale ? m_autoMaxValue : m_maxValue;
    int numCategories = m_categories.size();
    double barWidth = plotArea.width() / (numCategories * 1.5);
    double spacing = barWidth * 0.5;
    
    for (int i = 0; i < numCategories; ++i) {
        const QString &category = m_categories[i];
        
        if (!m_data.contains(category)) continue;
        
        double barX = plotArea.left() + i * (barWidth + spacing) + spacing / 2;
        double currentY = plotArea.bottom();
        
        // Draw stacked segments
        for (const SeriesInfo &series : m_series) {
            if (!m_data[category].contains(series.name)) continue;
            
            double value = m_data[category][series.name];
            double segmentHeight = plotArea.height() * value / maxVal;
            
            QRectF segmentRect(barX, currentY - segmentHeight, barWidth, segmentHeight);
            
            // Gradient fill
            QLinearGradient gradient(segmentRect.topLeft(), segmentRect.bottomLeft());
            gradient.setColorAt(0.0, series.color.lighter(120));
            gradient.setColorAt(1.0, series.color);
            
            painter.setPen(QPen(series.color.darker(120), 1));
            painter.setBrush(gradient);
            painter.drawRect(segmentRect);
            
            currentY -= segmentHeight;
        }
    }
}

void StackedBarChart::drawLegend(QPainter &painter)
{
    painter.setPen(QColor(200, 200, 200));
    QFont legendFont = painter.font();
    legendFont.setPointSize(8);
    painter.setFont(legendFont);
    
    int startX = 20;
    int y = height() - 35;
    
    for (const SeriesInfo &series : m_series) {
        // Color box
        painter.setBrush(series.color);
        painter.setPen(QPen(QColor(100, 100, 100), 1));
        painter.drawRect(startX, y, 15, 15);
        
        // Label
        painter.setPen(QColor(200, 200, 200));
        painter.drawText(startX + 20, y, 80, 20, Qt::AlignLeft | Qt::AlignVCenter, series.name);
        
        startX += 110;
    }
}

void StackedBarChart::drawLabels(QPainter &painter, const QRect &plotArea)
{
    double maxVal = m_autoScale ? m_autoMaxValue : m_maxValue;
    
    painter.setPen(QColor(200, 200, 200));
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    
    // Y axis labels
    for (int i = 0; i <= 5; ++i) {
        double value = maxVal * (5 - i) / 5;
        int y = plotArea.top() + plotArea.height() * i / 5;
        QString label = QString::number(value, 'f', 0);
        painter.drawText(5, y - 10, 70, 20, Qt::AlignRight | Qt::AlignVCenter, label);
    }
    
    // Category labels
    int numCategories = m_categories.size();
    double barWidth = plotArea.width() / (numCategories * 1.5);
    double spacing = barWidth * 0.5;
    
    for (int i = 0; i < numCategories; ++i) {
        double barX = plotArea.left() + i * (barWidth + spacing) + spacing / 2;
        painter.drawText(static_cast<int>(barX) - 30, plotArea.bottom() + 5, 
                        static_cast<int>(barWidth) + 60, 20, Qt::AlignCenter, m_categories[i]);
    }
    
    // Y axis label
    painter.save();
    painter.translate(25, plotArea.center().y());
    painter.rotate(-90);
    painter.drawText(-60, -10, 120, 20, Qt::AlignCenter, m_yLabel);
    painter.restore();
}









