#include "CamWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

CamWidget::CamWidget(QWidget *parent)
    : QWidget(parent)
    , m_rpm(60.0)
    , m_camProfile(0)
    , m_running(false)
    , m_camAngle(0.0)
{
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &CamWidget::onAnimationTimer);
}

CamWidget::~CamWidget()
{
}

void CamWidget::setRPM(double rpm)
{
    m_rpm = qBound(0.0, rpm, 300.0);
    update();
}

void CamWidget::setCamProfile(int profile)
{
    m_camProfile = qBound(0, profile, 2);
    update();
}

void CamWidget::setRunning(bool running)
{
    m_running = running;
    if (running) {
        m_animationTimer->start(50);  // 20 FPS
    } else {
        m_animationTimer->stop();
    }
    update();
}

void CamWidget::onAnimationTimer()
{
    if (m_running) {
        double angularVelocity = (m_rpm * 2.0 * M_PI) / 60.0;  // rad/s
        m_camAngle += angularVelocity * 0.05;  // 50ms time step
        
        if (m_camAngle >= 2.0 * M_PI) {
            m_camAngle -= 2.0 * M_PI;
        }
        
        update();
    }
}

double CamWidget::calculateFollowerDisplacement(double angle)
{
    double displacement = 0.0;
    double baseRadius = 60.0;  // Base cam radius
    
    switch (m_camProfile) {
        case 0:  // Circular cam - constant radius
            displacement = 0.0;  // No displacement change
            break;
            
        case 1:  // Heart cam - variable radius
            // At the top (follower contact point), we need the radius at angle relative to cam rotation
            // The follower is always at top, so we calculate radius at (¦Ð/2 - angle) in cam's frame
            {
                double contactAngle = M_PI / 2.0 - angle;  // Angle where follower touches
                double r = baseRadius * (1.0 + 0.3 * qSin(contactAngle));
                displacement = r - baseRadius;  // Displacement relative to base radius
            }
            break;
            
        case 2:  // Eccentric cam - offset circle
            // Center is at (20, 0) in cam frame. After rotation by 'angle',
            // the center is at (20*cos(angle), 20*sin(angle)) in world frame
            // Top of cam = centerY + radius
            {
                double centerOffsetY = 20.0 * qSin(angle);
                displacement = centerOffsetY;  // Vertical displacement of center
            }
            break;
    }
    
    return displacement;
}

void CamWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    
    int camCenterX = width() / 2;
    int camCenterY = height() / 2 - 20;
    int camRadius = 60;
    int rollerRadius = 15;  // Follower roller radius
    
    // Calculate follower displacement (change in cam top height)
    double displacement = calculateFollowerDisplacement(m_camAngle);
    
    // Follower roller should touch the top of the cam
    // Base position: cam top at camCenterY - camRadius
    // Roller center: cam top - rollerRadius
    int followerBaseY = camCenterY - camRadius - rollerRadius;
    int followerY = followerBaseY + static_cast<int>(displacement);
    
    // Draw frame
    drawFrame(painter, camCenterX, followerY - 100);
    
    // Draw follower
    drawFollower(painter, camCenterX, followerY, 0);  // displacement already applied to followerY
    
    // Draw cam
    drawCam(painter, camCenterX, camCenterY, camRadius, m_camAngle);
    
    drawParameters(painter);
}

void CamWidget::drawBackground(QPainter &painter)
{
    QLinearGradient bgGradient(0, 0, 0, height());
    bgGradient.setColorAt(0.0, QColor(245, 240, 240));
    bgGradient.setColorAt(1.0, QColor(230, 220, 220));
    painter.fillRect(rect(), bgGradient);

    painter.setPen(QColor(60, 60, 60));
    QFont titleFont = painter.font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 20, tr("Cam Mechanism"));
}

void CamWidget::drawCam(QPainter &painter, int centerX, int centerY, int radius, double angle)
{
    painter.save();
    painter.translate(centerX, centerY);
    painter.rotate(qRadiansToDegrees(angle));
    
    QPainterPath camPath;
    
    switch (m_camProfile) {
        case 0:  // Circular cam
            camPath.addEllipse(QPointF(0, 0), radius, radius);
            break;
            
        case 1:  // Heart cam
            for (int i = 0; i <= 360; i += 5) {
                double a = qDegreesToRadians(static_cast<double>(i));
                double r = radius * (1.0 + 0.3 * qSin(a));
                double x = r * qCos(a);
                double y = r * qSin(a);
                if (i == 0) {
                    camPath.moveTo(x, y);
                } else {
                    camPath.lineTo(x, y);
                }
            }
            camPath.closeSubpath();
            break;
            
        case 2:  // Eccentric cam
            camPath.addEllipse(QPointF(20, 0), radius, radius);
            break;
    }
    
    // Draw cam body
    QRadialGradient camGradient(0, 0, radius);
    camGradient.setColorAt(0.0, QColor(220, 180, 100));
    camGradient.setColorAt(1.0, QColor(160, 130, 60));
    painter.setPen(QPen(QColor(100, 80, 40), 2));
    painter.setBrush(camGradient);
    painter.drawPath(camPath);
    
    // Draw center shaft
    painter.setPen(QPen(QColor(60, 60, 60), 2));
    painter.setBrush(QColor(120, 120, 120));
    painter.drawEllipse(QPointF(0, 0), 12, 12);
    
    painter.restore();
}

void CamWidget::drawFollower(QPainter &painter, int x, int y, double displacement)
{
    int followerY = y + static_cast<int>(displacement);
    
    // Follower rod
    painter.setPen(QPen(QColor(100, 100, 120), 8, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(x, followerY, x, followerY + 80);
    
    // Follower roller
    QRadialGradient rollerGradient(x, followerY, 15);
    rollerGradient.setColorAt(0.0, QColor(200, 200, 200));
    rollerGradient.setColorAt(1.0, QColor(120, 120, 120));
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.setBrush(rollerGradient);
    painter.drawEllipse(QPointF(x, followerY), 15, 15);
}

void CamWidget::drawFrame(QPainter &painter, int x, int y)
{
    // Frame structure
    painter.setPen(QPen(QColor(80, 80, 80), 3));
    painter.setBrush(Qt::NoBrush);
    
    // Guide frame
    painter.drawRect(x - 25, y, 50, 100);
    
    // Support brackets
    painter.drawLine(x - 40, y, x - 25, y + 20);
    painter.drawLine(x + 40, y, x + 25, y + 20);
}

void CamWidget::drawParameters(QPainter &painter)
{
    int startY = height() - 90;
    int lineHeight = 18;
    
    painter.setPen(QColor(60, 60, 60));
    QFont paramFont = painter.font();
    paramFont.setPointSize(9);
    painter.setFont(paramFont);
    
    QString status = m_running ? tr("Running") : tr("Stopped");
    QColor statusColor = m_running ? QColor(0, 180, 0) : QColor(180, 0, 0);
    painter.setPen(statusColor);
    QFont statusFont = paramFont;
    statusFont.setBold(true);
    painter.setFont(statusFont);
    painter.drawText(10, startY, tr("Status: ") + status);
    
    painter.setFont(paramFont);
    painter.setPen(QColor(60, 60, 60));
    
    painter.drawText(10, startY + lineHeight, tr("RPM: %1").arg(m_rpm, 0, 'f', 0));
    
    QString profileName;
    switch (m_camProfile) {
        case 0: profileName = tr("Circular"); break;
        case 1: profileName = tr("Heart"); break;
        case 2: profileName = tr("Eccentric"); break;
    }
    painter.drawText(10, startY + lineHeight * 2, tr("Profile: ") + profileName);
    
    double displacement = calculateFollowerDisplacement(m_camAngle);
    painter.drawText(10, startY + lineHeight * 3, tr("Displacement: %1 mm").arg(displacement, 0, 'f', 1));
}

