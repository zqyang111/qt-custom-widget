#include "DigitalMeterWidget.h"
#include <QtMath>

DigitalMeterWidget::DigitalMeterWidget(QWidget *parent)
    : QWidget(parent)
    , m_value(0.0)
    , m_precision(2)
    , m_unit("")
    , m_label("Value")
    , m_backgroundColor(QColor(40, 40, 40))
    , m_displayColor(QColor(20, 20, 20))
    , m_textColor(QColor(0, 255, 100))
{
    setMinimumSize(120, 50);
}

DigitalMeterWidget::~DigitalMeterWidget()
{
}

void DigitalMeterWidget::setValue(double value)
{
    if (qAbs(m_value - value) > 0.0001) {
        m_value = value;
        update();
    }
}

void DigitalMeterWidget::setPrecision(int precision)
{
    if (precision >= 0 && m_precision != precision) {
        m_precision = precision;
        update();
    }
}

void DigitalMeterWidget::setUnit(const QString &unit)
{
    if (m_unit != unit) {
        m_unit = unit;
        update();
    }
}

void DigitalMeterWidget::setLabel(const QString &label)
{
    if (m_label != label) {
        m_label = label;
        update();
    }
}

void DigitalMeterWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawLabel(painter);
    drawValue(painter);
    drawUnit(painter);
}

void DigitalMeterWidget::drawBackground(QPainter &painter)
{
    // Draw outer frame
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.setBrush(m_backgroundColor);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);

    // Draw display area
    QRect displayRect(10, height() / 3, width() - 20, height() / 2);
    
    // Add inner shadow effect
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 150));
    painter.drawRoundedRect(displayRect.adjusted(2, 2, 0, 0), 3, 3);

    // Draw display background
    painter.setBrush(m_displayColor);
    painter.drawRoundedRect(displayRect, 3, 3);
}

void DigitalMeterWidget::drawLabel(QPainter &painter)
{
    if (m_label.isEmpty())
        return;

    painter.setPen(QColor(180, 180, 180));
    QFont font = painter.font();
    font.setPointSize(8);
    font.setBold(false);
    painter.setFont(font);

    QRect labelRect(10, 5, width() - 20, height() / 3 - 5);
    painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, m_label);
}

void DigitalMeterWidget::drawValue(QPainter &painter)
{
    QRect displayRect(10, height() / 3, width() - 20, height() / 2);

    painter.setPen(m_textColor);
    QFont font("Courier New");
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);

    QString valueText = QString::number(m_value, 'f', m_precision);

    // Calculate text width to position unit
    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(valueText);

    // Draw value
    QRect valueRect = displayRect.adjusted(5, 0, -5, 0);
    if (!m_unit.isEmpty()) {
        valueRect.setWidth(valueRect.width() - 40); // Leave space for unit
    }

    painter.drawText(valueRect, Qt::AlignRight | Qt::AlignVCenter, valueText);
}

void DigitalMeterWidget::drawUnit(QPainter &painter)
{
    if (m_unit.isEmpty())
        return;

    QRect displayRect(10, height() / 3, width() - 20, height() / 2);

    painter.setPen(m_textColor.darker(120));
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(false);
    painter.setFont(font);

    QRect unitRect(displayRect.right() - 40, displayRect.top(),
                   35, displayRect.height());
    painter.drawText(unitRect, Qt::AlignLeft | Qt::AlignVCenter, m_unit);
}


