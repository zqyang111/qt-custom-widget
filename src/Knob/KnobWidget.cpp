#include "KnobWidget.h"
#include <QtMath>
#include <QWheelEvent>

KnobWidget::KnobWidget(QWidget *parent)
    : QWidget(parent)
    , m_value(0.0)
    , m_minimum(0.0)
    , m_maximum(100.0)
    , m_step(1.0)
    , m_pressed(false)
{
    setMinimumSize(150, 150);
    setCursor(Qt::PointingHandCursor);
}

KnobWidget::~KnobWidget()
{
}

void KnobWidget::setValue(double value)
{
    if (value < m_minimum)
        value = m_minimum;
    if (value > m_maximum)
        value = m_maximum;

    // Snap to step
    if (m_step > 0) {
        value = qRound(value / m_step) * m_step;
    }

    if (qAbs(m_value - value) > 0.001) {
        m_value = value;
        update();
        emit valueChanged(m_value);
    }
}

void KnobWidget::setRange(double minimum, double maximum)
{
    if (minimum < maximum) {
        m_minimum = minimum;
        m_maximum = maximum;
        setValue(m_value);
    }
}

void KnobWidget::setStep(double step)
{
    if (step > 0) {
        m_step = step;
        setValue(m_value);
    }
}

void KnobWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawKnob(painter);
    drawIndicator(painter);
    drawValue(painter);
}

void KnobWidget::drawBackground(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 100.0, side / 100.0);

    // Draw outer circle shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 100));
    painter.drawEllipse(-47, -45, 94, 94);

    // Draw outer circle
    QRadialGradient outerGradient(0, -10, 50);
    outerGradient.setColorAt(0.0, QColor(80, 80, 80));
    outerGradient.setColorAt(0.7, QColor(50, 50, 50));
    outerGradient.setColorAt(1.0, QColor(30, 30, 30));

    painter.setBrush(outerGradient);
    painter.setPen(QPen(QColor(20, 20, 20), 2));
    painter.drawEllipse(-45, -45, 90, 90);

    // Draw scale marks and labels
    double angleRange = MAX_ANGLE - MIN_ANGLE;
    int numTicks = 11;
    
    QFont font = painter.font();
    font.setPointSize(7);
    font.setBold(true);
    painter.setFont(font);
    
    for (int i = 0; i < numTicks; ++i) {
        double angle = MIN_ANGLE + (angleRange * i / (numTicks - 1));
        painter.save();
        painter.rotate(angle);
        
        // Draw major and minor ticks
        if (i % 2 == 0) {
            // Major tick - longer and thicker
            painter.setPen(QPen(QColor(220, 220, 220), 3));
            painter.drawLine(0, -42, 0, -33);
            
            // Draw scale numbers
            painter.save();
            painter.translate(0, -50);
            painter.rotate(-angle);  // Counter-rotate text to keep it upright
            
            double scaleValue = m_minimum + (m_maximum - m_minimum) * i / (numTicks - 1);
            QString text = QString::number(static_cast<int>(scaleValue));
            
            painter.setPen(QColor(200, 200, 200));
            QRectF textRect(-10, -6, 20, 12);
            painter.drawText(textRect, Qt::AlignCenter, text);
            painter.restore();
            
        } else {
            // Minor tick - shorter
            painter.setPen(QPen(QColor(180, 180, 180), 2));
            painter.drawLine(0, -42, 0, -37);
        }
        
        painter.restore();
    }

    painter.restore();
}

void KnobWidget::drawKnob(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 100.0, side / 100.0);

    double angle = angleFromValue();
    painter.rotate(angle);

    // Draw knob shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 150));
    painter.drawEllipse(-28, -26, 56, 56);

    // Draw knob body
    QRadialGradient knobGradient(0, -8, 30);
    knobGradient.setColorAt(0.0, QColor(160, 160, 160));
    knobGradient.setColorAt(0.5, QColor(120, 120, 120));
    knobGradient.setColorAt(1.0, QColor(80, 80, 80));

    painter.setBrush(knobGradient);
    painter.setPen(QPen(QColor(60, 60, 60), 2));
    painter.drawEllipse(-28, -28, 56, 56);

    // Draw center depression
    QRadialGradient centerGradient(0, 0, 8);
    centerGradient.setColorAt(0.0, QColor(40, 40, 40));
    centerGradient.setColorAt(1.0, QColor(80, 80, 80));

    painter.setBrush(centerGradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(-8, -8, 16, 16);

    painter.restore();
}

void KnobWidget::drawIndicator(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 100.0, side / 100.0);

    double angle = angleFromValue();
    painter.rotate(angle);

    // Draw pointer
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 0, 0));
    
    QPolygon pointer;
    pointer << QPoint(-3, -10) << QPoint(3, -10) << QPoint(0, -25);
    painter.drawPolygon(pointer);

    // Draw pointer highlight
    painter.setBrush(QColor(255, 100, 100));
    QPolygon highlight;
    highlight << QPoint(-1, -10) << QPoint(1, -10) << QPoint(0, -25);
    painter.drawPolygon(highlight);

    painter.restore();
}

void KnobWidget::drawValue(QPainter &painter)
{
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);

    QString valueText = QString::number(m_value, 'f', 1);
    painter.drawText(QRectF(0, height() - 25, width(), 20), 
                    Qt::AlignCenter, valueText);
}

double KnobWidget::angleFromValue() const
{
    double range = m_maximum - m_minimum;
    if (range == 0)
        return MIN_ANGLE;
    
    double percentage = (m_value - m_minimum) / range;
    return MIN_ANGLE + percentage * (MAX_ANGLE - MIN_ANGLE);
}

void KnobWidget::updateValueFromMouse(const QPoint &pos)
{
    QPoint center(width() / 2, height() / 2);
    QPoint delta = pos - center;
    
    // Calculate angle from mouse position
    double angle = qAtan2(delta.x(), -delta.y()) * 180.0 / M_PI;
    
    // Clamp angle to valid range
    if (angle < MIN_ANGLE)
        angle = MIN_ANGLE;
    if (angle > MAX_ANGLE)
        angle = MAX_ANGLE;
    
    // Convert angle to value
    double angleRange = MAX_ANGLE - MIN_ANGLE;
    double percentage = (angle - MIN_ANGLE) / angleRange;
    double newValue = m_minimum + percentage * (m_maximum - m_minimum);
    
    setValue(newValue);
}

void KnobWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
        m_lastPos = event->pos();
        // Don't jump to mouse position, just start dragging from current value
    }
    QWidget::mousePressEvent(event);
}

void KnobWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pressed) {
        QPoint center(width() / 2, height() / 2);
        QPoint delta = event->pos() - center;
        
        // Calculate angle from mouse position relative to knob center
        // atan2 returns angle in radians, convert to degrees
        // Adjust for Qt coordinate system (Y-axis points down)
        double mouseAngle = qAtan2(delta.x(), -delta.y()) * 180.0 / M_PI;
        
        // Clamp to knob's valid range (-135 to +135 degrees)
        if (mouseAngle < MIN_ANGLE)
            mouseAngle = MIN_ANGLE;
        if (mouseAngle > MAX_ANGLE)
            mouseAngle = MAX_ANGLE;
        
        // Convert angle to value
        double angleRange = MAX_ANGLE - MIN_ANGLE;
        double percentage = (mouseAngle - MIN_ANGLE) / angleRange;
        double newValue = m_minimum + percentage * (m_maximum - m_minimum);
        
        setValue(newValue);
    }
    QWidget::mouseMoveEvent(event);
}

void KnobWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = false;
    }
    QWidget::mouseReleaseEvent(event);
}

void KnobWidget::wheelEvent(QWheelEvent *event)
{
    double delta = event->angleDelta().y() / 120.0;
    setValue(m_value + delta * m_step);
    event->accept();
}

