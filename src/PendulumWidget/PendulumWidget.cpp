#include "PendulumWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

PendulumWidget::PendulumWidget(QWidget *parent)
    : QWidget(parent)
    , m_amplitude(45.0)
    , m_period(2.0)
    , m_swinging(false)
    , m_damping(0.0)
    , m_currentAngle(0.0)
    , m_angularVelocity(0.0)
    , m_time(0.0)
{
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &PendulumWidget::onAnimationTimer);
    m_animationTimer->start(50);  // 20 FPS
}

PendulumWidget::~PendulumWidget()
{
}

void PendulumWidget::setAmplitude(double amplitude)
{
    m_amplitude = qBound(5.0, amplitude, 85.0);
    if (m_swinging) {
        m_currentAngle = m_amplitude;
        m_angularVelocity = 0.0;
        m_time = 0.0;
    }
    update();
}

void PendulumWidget::setPeriod(double period)
{
    m_period = qBound(0.5, period, 5.0);
    update();
}

void PendulumWidget::setSwinging(bool swinging)
{
    m_swinging = swinging;
    if (swinging) {
        m_currentAngle = m_amplitude;
        m_angularVelocity = 0.0;
        m_time = 0.0;
    } else {
        m_currentAngle = 0.0;
        m_angularVelocity = 0.0;
    }
    update();
}

void PendulumWidget::setDamping(double damping)
{
    m_damping = qBound(0.0, damping, 1.0);
    update();
}

void PendulumWidget::onAnimationTimer()
{
    if (m_swinging) {
        double dt = 0.05;  // Time step (50ms)
        m_time += dt;
        
        // Simple harmonic motion with damping
        double omega = 2.0 * M_PI / m_period;  // Angular frequency
        double dampingFactor = qExp(-m_damping * m_time);
        
        // Calculate current effective amplitude (damped amplitude)
        double currentAmplitude = m_amplitude * dampingFactor;
        
        // Calculate angle using damped harmonic motion formula
        m_currentAngle = currentAmplitude * qCos(omega * m_time);
        
        // Stop if effective amplitude becomes too small (not current angle)
        if (currentAmplitude < 0.5 && m_damping > 0.0) {
            m_swinging = false;
            m_currentAngle = 0.0;
        }
        
        update();
    }
}

void PendulumWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    
    // Calculate positions
    int pivotX = width() / 2;
    int pivotY = 60;
    int pendulumLength = height() - 180;
    
    // Draw support
    drawSupport(painter, pivotX, pivotY);
    
    // Draw angle arc
    if (m_swinging || m_currentAngle != 0) {
        drawAngleArc(painter, pivotX, pivotY, pendulumLength / 2, m_currentAngle);
    }
    
    // Draw pendulum
    drawPendulum(painter, pivotX, pivotY, pendulumLength, m_currentAngle);
    
    drawParameters(painter);
}

void PendulumWidget::drawBackground(QPainter &painter)
{
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
    painter.drawText(10, 20, tr("Pendulum"));
    
    // Draw reference vertical line
    painter.setPen(QPen(QColor(200, 200, 200), 1, Qt::DashLine));
    painter.drawLine(width() / 2, 60, width() / 2, height() - 120);
}

void PendulumWidget::drawSupport(QPainter &painter, int x, int y)
{
    // Support beam
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.setBrush(QColor(120, 120, 120));
    painter.drawRect(x - 60, y - 15, 120, 15);
    
    // Pivot point
    painter.setPen(QPen(QColor(60, 60, 60), 2));
    painter.setBrush(QColor(100, 100, 100));
    painter.drawEllipse(QPointF(x, y), 8, 8);
    
    // Support brackets
    painter.setPen(QPen(QColor(100, 100, 100), 3));
    painter.drawLine(x - 40, y - 15, x - 30, y);
    painter.drawLine(x + 40, y - 15, x + 30, y);
}

void PendulumWidget::drawPendulum(QPainter &painter, int pivotX, int pivotY, int length, double angle)
{
    // Calculate bob position
    double angleRad = qDegreesToRadians(angle);
    int bobX = pivotX + static_cast<int>(length * qSin(angleRad));
    int bobY = pivotY + static_cast<int>(length * qCos(angleRad));
    
    // Draw rod
    painter.setPen(QPen(QColor(100, 100, 120), 4, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(pivotX, pivotY, bobX, bobY);
    
    // Draw bob (weight)
    int bobRadius = 20;
    drawBob(painter, bobX, bobY, bobRadius);
    
    // Draw motion trail if swinging
    if (m_swinging && qAbs(m_angularVelocity) > 0.1) {
        painter.setPen(QPen(QColor(100, 150, 200, 100), 2, Qt::DotLine));
        // Draw a few trail positions
        for (int i = 1; i <= 3; ++i) {
            double trailAngle = angle - (i * 5);
            double trailAngleRad = qDegreesToRadians(trailAngle);
            int trailX = pivotX + static_cast<int>(length * qSin(trailAngleRad));
            int trailY = pivotY + static_cast<int>(length * qCos(trailAngleRad));
            painter.drawEllipse(QPointF(trailX, trailY), bobRadius - i * 3, bobRadius - i * 3);
        }
    }
}

void PendulumWidget::drawBob(QPainter &painter, int x, int y, int radius)
{
    // Bob shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 50));
    painter.drawEllipse(QPointF(x + 3, y + 3), radius, radius);
    
    // Bob body with metallic gradient
    QRadialGradient bobGradient(x - radius / 3, y - radius / 3, radius * 1.5);
    bobGradient.setColorAt(0.0, QColor(200, 180, 100));
    bobGradient.setColorAt(0.6, QColor(160, 140, 60));
    bobGradient.setColorAt(1.0, QColor(120, 100, 40));
    
    painter.setPen(QPen(QColor(100, 80, 20), 2));
    painter.setBrush(bobGradient);
    painter.drawEllipse(QPointF(x, y), radius, radius);
    
    // Highlight
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 255, 255, 150));
    painter.drawEllipse(QPointF(x - radius / 3, y - radius / 3), radius / 4, radius / 4);
}

void PendulumWidget::drawAngleArc(QPainter &painter, int pivotX, int pivotY, int length, double angle)
{
    // Draw arc showing current angle
    painter.setPen(QPen(QColor(100, 150, 200), 2));
    painter.setBrush(Qt::NoBrush);
    
    int startAngle = -90 * 16;  // Start from vertical (down)
    int spanAngle = static_cast<int>(angle * 16);
    
    QRect arcRect(pivotX - length, pivotY - length, length * 2, length * 2);
    painter.drawArc(arcRect, startAngle, spanAngle);
    
    // Draw angle text
    if (qAbs(angle) > 1.0) {
        painter.setPen(QColor(60, 60, 60));
        QFont angleFont = painter.font();
        angleFont.setPointSize(9);
        painter.setFont(angleFont);
        
        QString angleText = QString::number(qAbs(angle), 'f', 1) + QString::fromUtf8("\xC2\xB0");
        int textX = pivotX + (angle > 0 ? 1 : -1) * (length / 2 + 20);
        int textY = pivotY + length / 3;
        painter.drawText(textX - 20, textY, 40, 20, Qt::AlignCenter, angleText);
    }
}

void PendulumWidget::drawParameters(QPainter &painter)
{
    int startY = height() - 90;
    int lineHeight = 18;
    
    painter.setPen(QColor(60, 60, 60));
    QFont paramFont = painter.font();
    paramFont.setPointSize(9);
    painter.setFont(paramFont);
    
    // Status
    QString status = m_swinging ? tr("Swinging") : tr("Stopped");
    QColor statusColor = m_swinging ? QColor(0, 180, 0) : QColor(180, 0, 0);
    painter.setPen(statusColor);
    QFont statusFont = paramFont;
    statusFont.setBold(true);
    painter.setFont(statusFont);
    painter.drawText(QRect(10, startY, width() - 20, lineHeight), 
                     Qt::AlignLeft, tr("Status: ") + status);
    
    painter.setFont(paramFont);
    painter.setPen(QColor(60, 60, 60));
    
    // Current angle
    painter.drawText(QRect(10, startY + lineHeight, width() - 20, lineHeight), 
                     Qt::AlignLeft, tr("Current Angle: %1 deg").arg(m_currentAngle, 0, 'f', 1));
    
    // Amplitude
    painter.drawText(QRect(10, startY + lineHeight * 2, width() - 20, lineHeight), 
                     Qt::AlignLeft, tr("Max Amplitude: %1 deg").arg(m_amplitude, 0, 'f', 1));
    
    // Period
    painter.drawText(QRect(10, startY + lineHeight * 3, width() - 20, lineHeight), 
                     Qt::AlignLeft, tr("Period: %1 s").arg(m_period, 0, 'f', 2));
    
    // Damping
    if (m_damping > 0) {
        painter.drawText(QRect(10, startY + lineHeight * 4, width() - 20, lineHeight), 
                         Qt::AlignLeft, tr("Damping: %1").arg(m_damping, 0, 'f', 2));
    }
}

