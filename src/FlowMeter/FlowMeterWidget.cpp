#include "FlowMeterWidget.h"
#include <QtMath>

FlowMeterWidget::FlowMeterWidget(QWidget *parent)
    : QWidget(parent)
    , m_flowRate(0.0)
    , m_totalFlow(0.0)
    , m_unit("L/min")
    , m_backgroundColor(QColor(40, 40, 40))
    , m_displayColor(QColor(20, 20, 20))
    , m_textColor(QColor(0, 255, 100))
{
    setMinimumSize(120, 100);
}

FlowMeterWidget::~FlowMeterWidget()
{
}

void FlowMeterWidget::setFlowRate(double rate)
{
    if (rate < 0.0)
        rate = 0.0;

    if (qAbs(m_flowRate - rate) > 0.01) {
        m_flowRate = rate;
        update();
    }
}

void FlowMeterWidget::setTotalFlow(double total)
{
    if (total < 0.0)
        total = 0.0;

    m_totalFlow = total;
    update();
}

void FlowMeterWidget::setUnit(const QString &unit)
{
    m_unit = unit;
    update();
}

void FlowMeterWidget::resetTotal()
{
    m_totalFlow = 0.0;
    update();
}

void FlowMeterWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawFlowIndicator(painter);
    drawFlowRate(painter);
    drawTotalFlow(painter);
}

void FlowMeterWidget::drawBackground(QPainter &painter)
{
    // Draw outer frame
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.setBrush(m_backgroundColor);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);
}

void FlowMeterWidget::drawFlowIndicator(QPainter &painter)
{
    // Draw flow direction arrows if there's flow
    if (m_flowRate > 0.1) {
        painter.setPen(Qt::NoPen);
        
        // Animate the arrows by using time-based offset
        static int offset = 0;
        offset = (offset + 1) % 40;
        
        int arrowY = 15;
        int arrowSpacing = 20;
        QColor arrowColor = m_textColor;
        arrowColor.setAlpha(180);
        
        painter.setBrush(arrowColor);
        
        for (int i = -1; i < 3; ++i) {
            int x = 10 + i * arrowSpacing + (offset / 2);
            if (x > width() - 15) continue;
            if (x < 5) continue;
            
            QPolygon arrow;
            arrow << QPoint(x, arrowY)
                  << QPoint(x + 8, arrowY + 4)
                  << QPoint(x, arrowY + 8);
            painter.drawPolygon(arrow);
        }
    }

    // Trigger repaint for animation
    if (m_flowRate > 0.1) {
        QTimer::singleShot(50, this, QOverload<>::of(&FlowMeterWidget::update));
    }
}

void FlowMeterWidget::drawFlowRate(QPainter &painter)
{
    // Flow rate display area
    int displayY = 35;
    int displayHeight = 45;
    
    QRect displayRect(10, displayY, width() - 20, displayHeight);
    
    // Draw display background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 150));
    painter.drawRoundedRect(displayRect.adjusted(2, 2, 0, 0), 3, 3);
    
    painter.setBrush(m_displayColor);
    painter.drawRoundedRect(displayRect, 3, 3);
    
    // Draw label
    painter.setPen(QColor(180, 180, 180));
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    painter.drawText(displayRect.adjusted(5, 3, -5, -displayHeight + 15), 
                    Qt::AlignLeft | Qt::AlignTop, "Flow Rate");
    
    // Draw value
    painter.setPen(m_textColor);
    QFont valueFont("Courier New");
    valueFont.setPointSize(16);
    valueFont.setBold(true);
    painter.setFont(valueFont);
    
    QString flowText = QString::number(m_flowRate, 'f', 2);
    painter.drawText(displayRect.adjusted(5, 18, -50, -3), 
                    Qt::AlignLeft | Qt::AlignBottom, flowText);
    
    // Draw unit
    painter.setPen(m_textColor.darker(120));
    QFont unitFont = painter.font();
    unitFont.setPointSize(10);
    unitFont.setBold(false);
    painter.setFont(unitFont);
    painter.drawText(displayRect.adjusted(5, 18, -5, -3), 
                    Qt::AlignRight | Qt::AlignBottom, m_unit);
}

void FlowMeterWidget::drawTotalFlow(QPainter &painter)
{
    // Total flow display area
    int displayY = 85;
    int displayHeight = 40;
    
    QRect displayRect(10, displayY, width() - 20, displayHeight);
    
    // Draw display background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 150));
    painter.drawRoundedRect(displayRect.adjusted(2, 2, 0, 0), 3, 3);
    
    painter.setBrush(m_displayColor);
    painter.drawRoundedRect(displayRect, 3, 3);
    
    // Draw label
    painter.setPen(QColor(180, 180, 180));
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    painter.drawText(displayRect.adjusted(5, 3, -5, -displayHeight + 15), 
                    Qt::AlignLeft | Qt::AlignTop, "Total");
    
    // Draw value
    painter.setPen(QColor(100, 200, 255));
    QFont valueFont("Courier New");
    valueFont.setPointSize(14);
    valueFont.setBold(true);
    painter.setFont(valueFont);
    
    // Determine unit for total (remove /min or /h)
    QString totalUnit = m_unit;
    totalUnit.replace("/min", "");
    totalUnit.replace("/h", "");
    totalUnit.replace("/s", "");
    
    QString totalText = QString::number(m_totalFlow, 'f', 1);
    painter.drawText(displayRect.adjusted(5, 15, -40, -3), 
                    Qt::AlignLeft | Qt::AlignBottom, totalText);
    
    // Draw unit
    QFont unitFont = painter.font();
    unitFont.setPointSize(9);
    unitFont.setBold(false);
    painter.setFont(unitFont);
    painter.drawText(displayRect.adjusted(5, 15, -5, -3), 
                    Qt::AlignRight | Qt::AlignBottom, totalUnit);
}












