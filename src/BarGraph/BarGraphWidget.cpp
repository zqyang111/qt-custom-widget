#include "BarGraphWidget.h"
#include <QtMath>

BarGraphWidget::BarGraphWidget(QWidget *parent)
    : QWidget(parent)
    , m_maxValue(100.0)
    , m_autoScale(true)
    , m_showLabels(true)
    , m_showValues(true)
    , m_backgroundColor(QColor(30, 30, 30))
    , m_axisColor(QColor(150, 150, 150))
{
    setMinimumSize(200, 150);
}

BarGraphWidget::~BarGraphWidget()
{
}

void BarGraphWidget::addBar(const QString &label, double value, const QColor &color)
{
    BarData bar;
    bar.label = label;
    bar.value = value;
    bar.color = color;
    m_bars.append(bar);
    update();
}

void BarGraphWidget::setBarValue(int index, double value)
{
    if (index >= 0 && index < m_bars.size()) {
        m_bars[index].value = value;
        update();
    }
}

void BarGraphWidget::setBarValue(const QString &label, double value)
{
    for (auto &bar : m_bars) {
        if (bar.label == label) {
            bar.value = value;
            update();
            return;
        }
    }
}

void BarGraphWidget::clearBars()
{
    m_bars.clear();
    update();
}

void BarGraphWidget::setMaxValue(double maxValue)
{
    if (maxValue > 0.0) {
        m_maxValue = maxValue;
        m_autoScale = false;
        update();
    }
}

void BarGraphWidget::setAutoScale(bool enabled)
{
    m_autoScale = enabled;
    update();
}

void BarGraphWidget::setShowLabels(bool show)
{
    m_showLabels = show;
    update();
}

void BarGraphWidget::setShowValues(bool show)
{
    m_showValues = show;
    update();
}

void BarGraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawAxes(painter);
    drawBars(painter);
}

void BarGraphWidget::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), m_backgroundColor);
}

void BarGraphWidget::drawAxes(QPainter &painter)
{
    int marginLeft = 50;
    int marginRight = 20;
    int marginTop = 20;
    int marginBottom = m_showLabels ? 50 : 30;

    int chartWidth = width() - marginLeft - marginRight;
    int chartHeight = height() - marginTop - marginBottom;

    // Draw axes
    painter.setPen(QPen(m_axisColor, 2));
    painter.drawLine(marginLeft, marginTop, marginLeft, marginTop + chartHeight);
    painter.drawLine(marginLeft, marginTop + chartHeight, 
                    marginLeft + chartWidth, marginTop + chartHeight);

    // Draw Y-axis labels
    double maxVal = m_maxValue;
    if (m_autoScale && !m_bars.isEmpty()) {
        maxVal = 0.0;
        for (const auto &bar : m_bars) {
            if (bar.value > maxVal)
                maxVal = bar.value;
        }
        maxVal *= 1.1;  // Add 10% margin
    }

    painter.setPen(m_axisColor);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    int numYLabels = 5;
    for (int i = 0; i <= numYLabels; ++i) {
        int y = marginTop + chartHeight - (i * chartHeight / numYLabels);
        double value = (maxVal * i) / numYLabels;

        painter.drawLine(marginLeft - 5, y, marginLeft, y);
        
        QString label = QString::number(value, 'f', 0);
        painter.drawText(5, y - 8, marginLeft - 15, 16,
                        Qt::AlignRight | Qt::AlignVCenter, label);
    }
}

void BarGraphWidget::drawBars(QPainter &painter)
{
    if (m_bars.isEmpty())
        return;

    int marginLeft = 50;
    int marginRight = 20;
    int marginTop = 20;
    int marginBottom = m_showLabels ? 50 : 30;

    int chartWidth = width() - marginLeft - marginRight;
    int chartHeight = height() - marginTop - marginBottom;

    // Calculate max value for scaling
    double maxVal = m_maxValue;
    if (m_autoScale) {
        maxVal = 0.0;
        for (const auto &bar : m_bars) {
            if (bar.value > maxVal)
                maxVal = bar.value;
        }
        maxVal *= 1.1;
    }

    if (maxVal == 0.0)
        maxVal = 1.0;

    // Calculate bar dimensions
    int numBars = m_bars.size();
    int barSpacing = 10;
    int totalSpacing = (numBars + 1) * barSpacing;
    int barWidth = (chartWidth - totalSpacing) / numBars;

    if (barWidth < 10)
        barWidth = 10;

    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    // Draw each bar
    for (int i = 0; i < numBars; ++i) {
        const BarData &bar = m_bars[i];

        int x = marginLeft + barSpacing + i * (barWidth + barSpacing);
        double percentage = bar.value / maxVal;
        int barHeight = static_cast<int>(chartHeight * percentage);
        int y = marginTop + chartHeight - barHeight;

        // Draw bar
        QLinearGradient gradient(x, y, x, y + barHeight);
        gradient.setColorAt(0.0, bar.color.lighter(120));
        gradient.setColorAt(1.0, bar.color);

        painter.setBrush(gradient);
        painter.setPen(QPen(bar.color.darker(130), 2));
        painter.drawRect(x, y, barWidth, barHeight);

        // Draw value on top of bar
        if (m_showValues) {
            painter.setPen(Qt::white);
            font.setBold(true);
            painter.setFont(font);
            
            QString valueText = QString::number(bar.value, 'f', 1);
            painter.drawText(x, y - 15, barWidth, 12,
                           Qt::AlignCenter, valueText);
        }

        // Draw label below bar
        if (m_showLabels) {
            painter.setPen(m_axisColor);
            font.setBold(false);
            painter.setFont(font);

            painter.save();
            painter.translate(x + barWidth / 2, marginTop + chartHeight + 10);
            painter.rotate(-45);
            painter.drawText(-50, 0, 100, 20, Qt::AlignLeft | Qt::AlignVCenter, bar.label);
            painter.restore();
        }
    }
}












