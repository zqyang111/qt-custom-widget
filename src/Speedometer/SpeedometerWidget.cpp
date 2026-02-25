#include "SpeedometerWidget.h"
#include <QtMath>

SpeedometerWidget::SpeedometerWidget(QWidget *parent)
    : QWidget(parent)
    , m_value(0.0)
    , m_minValue(0.0)
    , m_maxValue(200.0)
    , m_warningStart(140.0)
    , m_redlineStart(180.0)
    , m_unit("km/h")
    , m_normalColor(QColor(0, 200, 0))
    , m_warningColor(QColor(255, 165, 0))
    , m_redlineColor(QColor(255, 0, 0))
{
    setMinimumSize(240, 180);
}

SpeedometerWidget::~SpeedometerWidget()
{
}

void SpeedometerWidget::setValue(double value)
{
    if (value < m_minValue)
        value = m_minValue;
    if (value > m_maxValue)
        value = m_maxValue;

    if (qAbs(m_value - value) > 0.001) {
        m_value = value;
        update();
    }
}

void SpeedometerWidget::setRange(double minValue, double maxValue)
{
    if (minValue < maxValue) {
        m_minValue = minValue;
        m_maxValue = maxValue;
        setValue(m_value);
    }
}

void SpeedometerWidget::setUnit(const QString &unit)
{
    m_unit = unit;
    update();
}

void SpeedometerWidget::setWarningZone(double warningStart)
{
    m_warningStart = warningStart;
    update();
}

void SpeedometerWidget::setRedlineZone(double redlineStart)
{
    m_redlineStart = redlineStart;
    update();
}

void SpeedometerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawArc(painter);
    drawScale(painter);
    drawNeedle(painter);
    drawCenterCap(painter);
    drawValue(painter);
}

void SpeedometerWidget::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(30, 30, 30));

    // Calculate size based on widget dimensions
    int side = qMin(width(), static_cast<int>(height() * 1.4));
    painter.save();
    painter.translate(width() / 2, height() - 30);  // Move center higher
    painter.scale(side / 300.0, side / 300.0);

    // Draw outer circle
    QRadialGradient gradient(0, 0, 150);
    gradient.setColorAt(0.0, QColor(60, 60, 60));
    gradient.setColorAt(0.7, QColor(40, 40, 40));
    gradient.setColorAt(1.0, QColor(20, 20, 20));

    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    // Draw bottom half circle: from 180 deg (left/9 o'clock) clockwise to 0 deg (right/3 o'clock)
    // Qt: positive span = counter-clockwise, negative span = clockwise
    painter.drawPie(-145, -145, 290, 290, 180 * 16, -180 * 16);

    painter.restore();
}

void SpeedometerWidget::drawArc(QPainter &painter)
{
    int side = qMin(width(), static_cast<int>(height() * 1.4));
    painter.save();
    painter.translate(width() / 2, height() - 30);
    painter.scale(side / 300.0, side / 300.0);

    // For speedometer arc: from 180 deg (left) clockwise to 0 deg (right), via bottom (270 deg)
    double startAngle = 180.0;   // Start from left (9 o'clock)
    double spanAngle = -180.0;   // Negative = clockwise, draws bottom half
    double range = m_maxValue - m_minValue;

    // Calculate angles for zones (as positive offsets, will be made negative for drawing)
    double warningAngle = ((m_warningStart - m_minValue) / range) * 180.0;
    double redlineAngle = ((m_redlineStart - m_minValue) / range) * 180.0;

    QPen arcPen(Qt::NoPen);
    arcPen.setWidth(20);
    arcPen.setCapStyle(Qt::FlatCap);

    // Normal zone (green) - from start to warning
    arcPen.setColor(m_normalColor);
    painter.setPen(arcPen);
    painter.drawArc(-120, -120, 240, 240, 
                    static_cast<int>(startAngle * 16), 
                    static_cast<int>(-warningAngle * 16));

    // Warning zone (orange) - from warning to redline
    arcPen.setColor(m_warningColor);
    painter.setPen(arcPen);
    painter.drawArc(-120, -120, 240, 240,
                    static_cast<int>((startAngle - warningAngle) * 16),
                    static_cast<int>(-(redlineAngle - warningAngle) * 16));

    // Redline zone (red) - from redline to max
    arcPen.setColor(m_redlineColor);
    painter.setPen(arcPen);
    painter.drawArc(-120, -120, 240, 240,
                    static_cast<int>((startAngle - redlineAngle) * 16),
                    static_cast<int>(-(180.0 - redlineAngle) * 16));

    painter.restore();
}

void SpeedometerWidget::drawScale(QPainter &painter)
{
    int side = qMin(width(), static_cast<int>(height() * 1.4));
    int centerX = width() / 2;
    int centerY = height() - 30;
    
    painter.save();
    painter.translate(centerX, centerY);
    painter.scale(side / 300.0, side / 300.0);

    painter.setPen(QPen(Qt::white, 2));
    QFont font = painter.font();
    font.setPointSize(12);
    font.setBold(true);
    painter.setFont(font);

    // Draw scale: 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200
    int numMajorTicks = 11;
    
    for (int i = 0; i < numMajorTicks; ++i) {
        double scaleValue = m_minValue + (m_maxValue - m_minValue) * i / (numMajorTicks - 1);
        double percentage = (scaleValue - m_minValue) / (m_maxValue - m_minValue);
        double angle = -90.0 + percentage * 180.0;
        
        painter.save();
        painter.rotate(angle);
        
        // Draw major tick - closer to the arc (from -130 to -110)
        painter.setPen(QPen(Qt::white, 3));
        painter.drawLine(0, -130, 0, -110);
        
        // Draw number (rotate back to keep upright)
        painter.save();
        painter.translate(0, -95);  // Position closer to arc
        painter.rotate(-angle);  // Counter-rotate to keep text upright
        
        painter.setPen(Qt::white);
        QString text = QString::number(static_cast<int>(scaleValue));
        painter.drawText(QRectF(-20, -10, 40, 20), Qt::AlignCenter, text);
        
        painter.restore();
        painter.restore();
        
        // Draw minor ticks between major ticks
        if (i < numMajorTicks - 1) {
            for (int j = 1; j < 5; ++j) {
                double minorPercentage = percentage + (1.0 / (numMajorTicks - 1)) * j / 5.0;
                double minorAngle = -90.0 + minorPercentage * 180.0;
                
                painter.save();
                painter.rotate(minorAngle);
                painter.setPen(QPen(QColor(200, 200, 200), 2));
                painter.drawLine(0, -130, 0, -118);  // Shorter, closer to arc
                painter.restore();
            }
        }
    }
    
    painter.restore();
}

void SpeedometerWidget::drawNeedle(QPainter &painter)
{
    int side = qMin(width(), static_cast<int>(height() * 1.4));
    painter.save();
    painter.translate(width() / 2, height() - 30);
    painter.scale(side / 300.0, side / 300.0);

    // Calculate needle angle
    // Needle default points up (270 deg in Qt), we want it to point left (180 deg) at value=0
    // So we need to offset by -90 degrees
    double percentage = (m_value - m_minValue) / (m_maxValue - m_minValue);
    double angle = -90.0 + percentage * 180.0;  // -90 to 90 degrees

    painter.rotate(angle);

    // Choose needle color
    QColor needleColor;
    if (m_value >= m_redlineStart)
        needleColor = m_redlineColor;
    else if (m_value >= m_warningStart)
        needleColor = m_warningColor;
    else
        needleColor = m_normalColor;

    // Draw needle shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 100));
    QPolygon shadowPolygon;
    shadowPolygon << QPoint(-5, 3) << QPoint(5, 3) << QPoint(2, -97) << QPoint(-2, -97);
    painter.drawPolygon(shadowPolygon);

    // Draw needle
    painter.setBrush(needleColor);
    QPolygon needlePolygon;
    needlePolygon << QPoint(-4, 0) << QPoint(4, 0) << QPoint(1, -95) << QPoint(-1, -95);
    painter.drawPolygon(needlePolygon);

    painter.restore();
}

void SpeedometerWidget::drawCenterCap(QPainter &painter)
{
    int side = qMin(width(), static_cast<int>(height() * 1.4));
    painter.save();
    painter.translate(width() / 2, height() - 30);
    painter.scale(side / 300.0, side / 300.0);

    QRadialGradient gradient(0, 0, 15);
    gradient.setColorAt(0.0, QColor(180, 180, 180));
    gradient.setColorAt(0.5, QColor(120, 120, 120));
    gradient.setColorAt(1.0, QColor(60, 60, 60));

    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.drawEllipse(-15, -15, 30, 30);

    painter.restore();
}

void SpeedometerWidget::drawValue(QPainter &painter)
{
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(20);
    font.setBold(true);
    painter.setFont(font);

    QString valueText = QString::number(m_value, 'f', 0);
    painter.drawText(QRectF(0, height() - 50, width(), 25), 
                    Qt::AlignCenter, valueText);

    // Draw unit
    font.setPointSize(10);
    font.setBold(false);
    painter.setFont(font);
    painter.drawText(QRectF(0, height() - 25, width(), 20),
                    Qt::AlignCenter, m_unit);
}

