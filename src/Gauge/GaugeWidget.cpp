#include "GaugeWidget.h"
#include <QPainterPath>
#include <QtMath>

GaugeWidget::GaugeWidget(QWidget *parent)
    : QWidget(parent)
    , m_value(0.0)
    , m_minValue(0.0)
    , m_maxValue(100.0)
    , m_warningStart(70.0)
    , m_criticalStart(90.0)
    , m_unit("")
    , m_normalColor(QColor(0, 200, 0))
    , m_warningColor(QColor(255, 165, 0))
    , m_criticalColor(QColor(255, 0, 0))
{
    setMinimumSize(150, 150);
}

GaugeWidget::~GaugeWidget()
{
}

void GaugeWidget::setValue(double value)
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

void GaugeWidget::setRange(double minValue, double maxValue)
{
    if (minValue < maxValue) {
        m_minValue = minValue;
        m_maxValue = maxValue;
        setValue(m_value); // Revalidate current value
    }
}

void GaugeWidget::setUnit(const QString &unit)
{
    m_unit = unit;
    update();
}

void GaugeWidget::setWarningRange(double warningStart, double criticalStart)
{
    m_warningStart = warningStart;
    m_criticalStart = criticalStart;
    update();
}

void GaugeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawScale(painter);
    drawNeedle(painter);
    drawCenterCircle(painter);
    drawValue(painter);
}

void GaugeWidget::drawBackground(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 300.0, side / 300.0);

    // Draw outer circle
    QRadialGradient gradient(0, 0, 150);
    gradient.setColorAt(0.0, QColor(60, 60, 60));
    gradient.setColorAt(0.7, QColor(40, 40, 40));
    gradient.setColorAt(1.0, QColor(20, 20, 20));
    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawEllipse(-145, -145, 290, 290);

    painter.restore();
}

void GaugeWidget::drawScale(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 300.0, side / 300.0);

    // Draw arc segments with different colors
    double startAngle = 225.0; // Bottom left
    double spanAngle = 270.0;  // 3/4 circle

    // Calculate angles for warning and critical zones
    double range = m_maxValue - m_minValue;
    double warningAngle = ((m_warningStart - m_minValue) / range) * spanAngle;
    double criticalAngle = ((m_criticalStart - m_minValue) / range) * spanAngle;

    QPen arcPen(Qt::NoPen);
    arcPen.setWidth(20);
    arcPen.setCapStyle(Qt::FlatCap);

    // Draw normal zone (green)
    arcPen.setColor(m_normalColor);
    painter.setPen(arcPen);
    painter.drawArc(-120, -120, 240, 240, static_cast<int>(startAngle * 16), 
                    static_cast<int>(warningAngle * 16));

    // Draw warning zone (orange)
    arcPen.setColor(m_warningColor);
    painter.setPen(arcPen);
    painter.drawArc(-120, -120, 240, 240, 
                    static_cast<int>((startAngle + warningAngle) * 16), 
                    static_cast<int>((criticalAngle - warningAngle) * 16));

    // Draw critical zone (red)
    arcPen.setColor(m_criticalColor);
    painter.setPen(arcPen);
    painter.drawArc(-120, -120, 240, 240, 
                    static_cast<int>((startAngle + criticalAngle) * 16), 
                    static_cast<int>((spanAngle - criticalAngle) * 16));

    // Draw scale marks
    painter.setPen(QPen(Qt::white, 2));
    for (int i = 0; i <= 10; ++i) {
        painter.save();
        double angle = startAngle + (i * spanAngle / 10.0);
        painter.rotate(angle);
        
        if (i % 2 == 0) {
            painter.drawLine(0, -110, 0, -100);
            
            // Draw scale numbers
            painter.save();
            painter.translate(0, -85);
            painter.rotate(-angle);
            double scaleValue = m_minValue + (m_maxValue - m_minValue) * i / 10.0;
            painter.drawText(QRectF(-20, -10, 40, 20), Qt::AlignCenter, 
                           QString::number(static_cast<int>(scaleValue)));
            painter.restore();
        } else {
            painter.drawLine(0, -110, 0, -105);
        }
        
        painter.restore();
    }

    painter.restore();
}

void GaugeWidget::drawNeedle(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 300.0, side / 300.0);

    // Calculate needle angle
    double percentage = (m_value - m_minValue) / (m_maxValue - m_minValue);
    double angle = 225.0 + percentage * 270.0;

    painter.rotate(angle);

    // Choose needle color based on value
    QColor needleColor;
    if (m_value >= m_criticalStart)
        needleColor = m_criticalColor;
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

void GaugeWidget::drawCenterCircle(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
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

void GaugeWidget::drawValue(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 300.0, side / 300.0);

    // Draw digital value display
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(18);
    font.setBold(true);
    painter.setFont(font);

    QString valueText = QString::number(m_value, 'f', 1);
    if (!m_unit.isEmpty())
        valueText += " " + m_unit;

    painter.drawText(QRectF(-60, 40, 120, 30), Qt::AlignCenter, valueText);

    painter.restore();
}












