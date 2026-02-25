#include "BeltDriveWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

BeltDriveWidget::BeltDriveWidget(QWidget *parent)
    : QWidget(parent)
    , m_running(false)
    , m_rpm(0.0)
    , m_beltSpeed(0.0)
    , m_angle(0.0)
    , m_beltOffset(0.0)
    , m_timer(nullptr)
{
    // Create and start timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &BeltDriveWidget::updateAnimation);
    m_timer->start(50);  // 20 FPS
}

BeltDriveWidget::~BeltDriveWidget()
{
}

void BeltDriveWidget::setRunning(bool running)
{
    m_running = running;
    update();
}

void BeltDriveWidget::setRPM(double rpm)
{
    m_rpm = rpm;
    update();
}

void BeltDriveWidget::setBeltSpeed(double speed)
{
    m_beltSpeed = speed;
    update();
}

void BeltDriveWidget::updateAnimation()
{
    if (m_running && m_rpm > 0) {
        // Calculate angle increment based on RPM
        double deltaAngle = m_rpm * 0.06;
        
        m_angle += deltaAngle;
        if (m_angle >= 360.0) {
            m_angle -= 360.0;
        }
        
        // Update belt offset for animation
        m_beltOffset += deltaAngle * 0.5;
        if (m_beltOffset >= 20.0) {
            m_beltOffset -= 20.0;
        }
        
        update();
    }
}

void BeltDriveWidget::paintEvent(QPaintEvent *event)
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
    painter.drawText(10, 20, tr("Belt Drive System"));

    // Calculate positions - one large, one small pulley (closer together)
    int pulley1X = width() / 3;
    int pulley1Y = height() / 2;
    int pulley1Radius = qMin(width(), height()) / 8;  // Large pulley

    int pulley2X = 2 * width() / 3;
    int pulley2Y = height() / 2;
    int pulley2Radius = pulley1Radius * 0.6;  // Small pulley (60% of large)

    // Draw belt first (behind pulleys)
    drawBelt(painter);

    // Draw two pulleys
    QColor color1 = m_running ? QColor(100, 150, 200) : QColor(150, 150, 150);
    QColor color2 = m_running ? QColor(200, 150, 100) : QColor(150, 150, 150);

    // Large pulley spins slower, small pulley spins faster
    drawPulley(painter, pulley1X, pulley1Y, pulley1Radius, -m_angle * 0.6, color1);  // Large spins reversed at 60% speed
    drawPulley(painter, pulley2X, pulley2Y, pulley2Radius, m_angle, color2);  // Small at normal speed
}

void BeltDriveWidget::drawPulley(QPainter &painter, int x, int y, int radius, double angle, const QColor &color)
{
    painter.save();
    painter.translate(x, y);

    // Draw pulley body
    QRadialGradient gradient(0, 0, radius);
    gradient.setColorAt(0.0, color.lighter(120));
    gradient.setColorAt(0.7, color);
    gradient.setColorAt(1.0, color.darker(120));

    painter.setPen(QPen(color.darker(150), 2));
    painter.setBrush(gradient);
    painter.drawEllipse(QPointF(0, 0), radius, radius);

    // Draw rotation indicator lines
    painter.rotate(angle);
    painter.setPen(QPen(color.darker(180), 2));
    for (int i = 0; i < 8; ++i) {
        painter.drawLine(0, 0, radius * 0.9, 0);
        painter.rotate(45);
    }

    // Draw center shaft
    painter.resetTransform();
    painter.translate(x, y);
    painter.setBrush(QColor(80, 80, 80));
    painter.drawEllipse(QPointF(0, 0), radius * 0.2, radius * 0.2);

    painter.restore();
}

void BeltDriveWidget::drawBelt(QPainter &painter)
{
    // Calculate pulley positions and sizes (closer together)
    double pulley1X = width() / 3.0;
    double pulley1Y = height() / 2.0;
    double radius1 = qMin(width(), height()) / 8.0;  // Large pulley

    double pulley2X = 2.0 * width() / 3.0;
    double pulley2Y = height() / 2.0;
    double radius2 = radius1 * 0.6;  // Small pulley

    // Calculate external tangent points for horizontal pulleys
    double distance = pulley2X - pulley1X;
    double radiusDiff = radius1 - radius2;
    double theta = qAsin(radiusDiff / distance);  // Tangent angle
    
    // Tangent points (external tangent)
    double sinTheta = qSin(theta);
    double cosTheta = qCos(theta);
    
    QPointF p1Top(pulley1X + radius1 * sinTheta, pulley1Y - radius1 * cosTheta);
    QPointF p2Top(pulley2X + radius2 * sinTheta, pulley2Y - radius2 * cosTheta);
    QPointF p1Bot(pulley1X - radius1 * sinTheta, pulley1Y + radius1 * cosTheta);
    QPointF p2Bot(pulley2X - radius2 * sinTheta, pulley2Y + radius2 * cosTheta);

    // Calculate angles for arcs (in degrees, 0 = 3 o'clock, counter-clockwise)
    double angle1Top = qRadiansToDegrees(qAtan2(-(p1Top.y() - pulley1Y), p1Top.x() - pulley1X));
    double angle2Top = qRadiansToDegrees(qAtan2(-(p2Top.y() - pulley2Y), p2Top.x() - pulley2X));
    double angle1Bot = qRadiansToDegrees(qAtan2(-(p1Bot.y() - pulley1Y), p1Bot.x() - pulley1X));
    double angle2Bot = qRadiansToDegrees(qAtan2(-(p2Bot.y() - pulley2Y), p2Bot.x() - pulley2X));

    // Draw belt path
    QPainterPath beltPath;
    beltPath.moveTo(p1Top);
    beltPath.lineTo(p2Top);
    
    // Arc on small pulley (front, clockwise from top to bottom)
    double span2 = angle2Bot - angle2Top;
    if (span2 < 0) span2 += 360;
    if (span2 > 180) span2 -= 360;  // Take shorter arc
    beltPath.arcTo(pulley2X - radius2, pulley2Y - radius2, radius2 * 2, radius2 * 2, 
                   angle2Top, span2);
    
    beltPath.lineTo(p1Bot);
    
    // Arc on large pulley (back, counter-clockwise from bottom to top - long arc)
    double span1 = angle1Top - angle1Bot;
    if (span1 > 0) span1 -= 360;  // Make negative for long arc
    beltPath.arcTo(pulley1X - radius1, pulley1Y - radius1, radius1 * 2, radius1 * 2, 
                   angle1Bot, span1);
    
    beltPath.closeSubpath();

    // Draw the belt
    QColor beltColor = m_running ? QColor(50, 50, 50) : QColor(80, 80, 80);
    painter.setPen(QPen(beltColor.darker(130), 2));
    painter.setBrush(beltColor);
    painter.drawPath(beltPath);

    // Draw animated dashes on belt if running
    if (m_running) {
        QPen dashPen(QColor(200, 200, 0), 2);
        dashPen.setDashPattern({8, 4});
        dashPen.setDashOffset(m_beltOffset);
        painter.setPen(dashPen);
        painter.setBrush(Qt::NoBrush);
        
        // Draw dashes on the visible tangent lines
        painter.drawLine(p1Top, p2Top);
        painter.drawLine(p2Bot, p1Bot);
    }
}

