#include "GearWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

GearWidget::GearWidget(QWidget *parent)
    : QWidget(parent)
    , m_running(false)
    , m_rpm(0.0)
    , m_torque(0.0)
    , m_angle(0.0)
    , m_timer(nullptr)
{
    // Create and start timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &GearWidget::updateAnimation);
    m_timer->start(50);  // 20 FPS
}

GearWidget::~GearWidget()
{
}

void GearWidget::setRunning(bool running)
{
    m_running = running;
    update();
}

void GearWidget::setRPM(double rpm)
{
    m_rpm = rpm;
    update();
}

void GearWidget::setTorque(double torque)
{
    m_torque = torque;
    update();
}

void GearWidget::updateAnimation()
{
    if (m_running && m_rpm > 0) {
        // Calculate angle increment based on RPM
        m_angle += m_rpm * 0.06;
        if (m_angle >= 360.0) {
            m_angle -= 360.0;
        }
        update();
    }
}

void GearWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background
    QLinearGradient bgGradient(0, 0, 0, height());
    bgGradient.setColorAt(0.0, QColor(240, 240, 240));
    bgGradient.setColorAt(1.0, QColor(220, 220, 220));
    painter.fillRect(rect(), bgGradient);

    // Title
    painter.setPen(QColor(60, 60, 60));
    QFont titleFont = painter.font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 20, tr("Gear Train"));

    // Calculate positions for meshing gears
    int teeth1 = 12;
    int teeth2 = 6;
    
    int centerX1 = width() / 3;
    int centerY1 = height() / 2;
    int radius1 = qMin(width(), height()) / 8;

    // Second gear radius based on teeth ratio for proper meshing
    int radius2 = radius1 * teeth2 / teeth1;
    
    // Position second gear so gears mesh (center distance = sum of radii)
    int centerX2 = centerX1 + radius1 + radius2;
    int centerY2 = centerY1;

    // Calculate rotation angles for proper meshing
    double angle1 = m_angle;
    // Second gear rotates in opposite direction at speed proportional to teeth ratio
    double angle2 = -m_angle * (double)teeth1 / teeth2;
    // Add phase offset for tooth meshing (half a tooth pitch)
    angle2 += 180.0 / teeth2;

    // Draw two gears
    QColor color1 = m_running ? QColor(100, 150, 200) : QColor(150, 150, 150);
    QColor color2 = m_running ? QColor(200, 150, 100) : QColor(150, 150, 150);

    drawGear(painter, centerX1, centerY1, radius1, teeth1, angle1, color1);
    drawGear(painter, centerX2, centerY2, radius2, teeth2, angle2, color2);
}

void GearWidget::drawGear(QPainter &painter, int x, int y, int radius, int teeth, double angle, const QColor &color)
{
    painter.save();
    painter.translate(x, y);
    painter.rotate(angle);

    // Draw gear body
    QRadialGradient gradient(0, 0, radius);
    gradient.setColorAt(0.0, color.lighter(120));
    gradient.setColorAt(0.7, color);
    gradient.setColorAt(1.0, color.darker(120));

    painter.setPen(QPen(color.darker(150), 2));
    painter.setBrush(gradient);
    painter.drawEllipse(QPointF(0, 0), radius * 0.7, radius * 0.7);

    // Draw teeth with proper width for meshing
    double toothPitch = 360.0 / teeth;  // Degrees per tooth
    double toothWidth = toothPitch * 0.45;  // Tooth takes 45%, gap takes 55%
    int innerRadius = radius * 0.7;
    int outerRadius = radius;

    for (int i = 0; i < teeth; ++i) {
        double centerAngle = toothPitch * i;
        double startAngle = centerAngle - toothWidth / 2;
        double endAngle = centerAngle + toothWidth / 2;

        double startRad = qDegreesToRadians(startAngle);
        double endRad = qDegreesToRadians(endAngle);

        QPointF p1(innerRadius * qCos(startRad), innerRadius * qSin(startRad));
        QPointF p2(outerRadius * qCos(startRad), outerRadius * qSin(startRad));
        QPointF p3(outerRadius * qCos(endRad), outerRadius * qSin(endRad));
        QPointF p4(innerRadius * qCos(endRad), innerRadius * qSin(endRad));

        QPainterPath toothPath;
        toothPath.moveTo(p1);
        toothPath.lineTo(p2);
        toothPath.lineTo(p3);
        toothPath.lineTo(p4);
        toothPath.closeSubpath();

        painter.fillPath(toothPath, gradient);
        painter.drawPath(toothPath);
    }

    // Draw center shaft
    painter.setBrush(QColor(80, 80, 80));
    painter.drawEllipse(QPointF(0, 0), radius * 0.15, radius * 0.15);

    painter.restore();
}

