#include "PistonWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

PistonWidget::PistonWidget(QWidget *parent)
    : QWidget(parent)
    , m_rpm(0.0)
    , m_stroke(100.0)
    , m_pressure(0.0)
    , m_running(false)
    , m_crankAngle(0.0)
{
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &PistonWidget::onAnimationTimer);
    m_animationTimer->start(50);  // 20 FPS
}

PistonWidget::~PistonWidget()
{
}

void PistonWidget::setRPM(double rpm)
{
    m_rpm = qBound(0.0, rpm, 3000.0);
    update();
}

void PistonWidget::setStroke(double stroke)
{
    m_stroke = qBound(50.0, stroke, 200.0);
    update();
}

void PistonWidget::setPressure(double pressure)
{
    m_pressure = qMax(0.0, pressure);
    update();
}

void PistonWidget::setRunning(bool running)
{
    m_running = running;
    update();
}

void PistonWidget::onAnimationTimer()
{
    if (m_running && m_rpm > 0) {
        // Angular velocity proportional to RPM
        double deltaAngle = m_rpm * 0.06;
        
        m_crankAngle += deltaAngle;
        if (m_crankAngle >= 360.0) m_crankAngle -= 360.0;
        
        update();
    }
}

void PistonWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    
    // Calculate positions
    int centerX = width() / 2;
    int crankCenterY = height() * 2 / 3;
    int cylinderTop = 40;
    int cylinderWidth = 80;
    int cylinderHeight = height() / 2;
    
    // Crank radius (half of stroke)
    int crankRadius = m_stroke * 0.4;  // Scale to fit widget
    
    // Connecting rod length (fixed)
    int rodLength = cylinderHeight - 60;
    
    // Calculate crank pin position based on crank angle
    double angleRad = qDegreesToRadians(m_crankAngle);
    int crankX = centerX + crankRadius * qCos(angleRad);
    int crankY = crankCenterY + crankRadius * qSin(angleRad);
    
    // Calculate piston position using correct geometry
    // Piston is on vertical centerline, so horizontal offset from crank pin is (crankX - centerX)
    double horizontalOffset = crankX - centerX;
    
    // Using Pythagorean theorem: rodLength^2 = horizontalOffset^2 + verticalDistance^2
    double verticalDistance = qSqrt(rodLength * rodLength - horizontalOffset * horizontalOffset);
    
    // Piston Y position (crank pin Y - vertical distance to piston)
    int pistonY = crankY - static_cast<int>(verticalDistance);
    
    // Draw cylinder
    drawCylinder(painter, centerX - cylinderWidth / 2, cylinderTop, cylinderWidth, cylinderHeight);
    
    // Draw crankshaft first (so connecting rod appears on top)
    drawCrankshaft(painter, centerX, crankCenterY, crankRadius, m_crankAngle);
    
    // Draw connecting rod
    drawConnectingRod(painter, centerX, pistonY + 12, crankX, crankY);
    
    // Draw piston
    drawPiston(painter, centerX - cylinderWidth / 2 + 5, pistonY, cylinderWidth - 10, 25);
    
    drawParameters(painter);
    
    // Calculate normalized angle for stroke detection
    double normalizedAngle = fmod(m_crankAngle, 360.0);
    if (normalizedAngle < 0) normalizedAngle += 360.0;
    
    // Draw intake flow during intake stroke (piston moving down, 0-180 degrees)
    if (m_running && normalizedAngle >= 0 && normalizedAngle <= 180) {
        int intakeX = centerX - cylinderWidth / 2 - 10;
        int intakeY = cylinderTop - 15;
        drawIntakeFlow(painter, intakeX, intakeY, normalizedAngle);
    }
    
    // Draw exhaust flow during exhaust stroke (piston moving up, 180-360 degrees)
    if (m_running && normalizedAngle >= 180 && normalizedAngle <= 360) {
        int exhaustX = centerX + cylinderWidth / 2 + 10;
        int exhaustY = cylinderTop - 15;
        drawExhaustFlow(painter, exhaustX, exhaustY, normalizedAngle - 180);
    }
    
    // Draw spark effect when piston is near top (compression/ignition)
    // Check if near top dead center (240-300 degrees range)
    if (m_running && normalizedAngle >= 240 && normalizedAngle <= 300) {
        // Draw spark/ignition effect inside cylinder head
        drawSpark(painter, centerX, cylinderTop + 10);
    }
}

void PistonWidget::drawBackground(QPainter &painter)
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
    painter.drawText(10, 20, tr("Piston Engine"));
}

void PistonWidget::drawCylinder(QPainter &painter, int x, int y, int width, int height)
{
    // Cylinder body
    QLinearGradient cylinderGradient(x, 0, x + width, 0);
    cylinderGradient.setColorAt(0.0, QColor(180, 180, 180));
    cylinderGradient.setColorAt(0.5, QColor(200, 200, 200));
    cylinderGradient.setColorAt(1.0, QColor(180, 180, 180));
    
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.setBrush(cylinderGradient);
    painter.drawRect(x, y, width, height);
    
    // Cylinder head
    painter.setBrush(QColor(150, 150, 150));
    painter.drawRect(x - 10, y - 20, width + 20, 20);
    
    // Intake port (left side)
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.setBrush(QColor(100, 120, 140));  // Blue-ish for intake (air)
    int intakeX = x - 10;
    int intakeY = y - 15;
    painter.drawEllipse(intakeX - 8, intakeY - 4, 16, 8);
    painter.drawRect(intakeX - 8, intakeY, 16, 8);  // Port opening
    
    // Exhaust port (right side)
    painter.setBrush(QColor(140, 100, 80));  // Reddish for exhaust (hot gas)
    int exhaustX = x + width + 10;
    int exhaustY = y - 15;
    painter.drawEllipse(exhaustX - 8, exhaustY - 4, 16, 8);
    painter.drawRect(exhaustX - 8, exhaustY, 16, 8);  // Port opening
    
    // Cooling fins
    painter.setPen(QPen(QColor(120, 120, 120), 1));
    for (int i = 0; i < 5; ++i) {
        int finY = y + i * 15;
        painter.drawLine(x - 15, finY, x, finY);
        painter.drawLine(x + width, finY, x + width + 15, finY);
    }
}

void PistonWidget::drawPiston(QPainter &painter, int x, int y, int width, int height)
{
    // Piston body
    QLinearGradient pistonGradient(x, 0, x + width, 0);
    pistonGradient.setColorAt(0.0, QColor(120, 140, 160));
    pistonGradient.setColorAt(0.5, QColor(150, 170, 190));
    pistonGradient.setColorAt(1.0, QColor(120, 140, 160));
    
    painter.setPen(QPen(QColor(80, 100, 120), 2));
    painter.setBrush(pistonGradient);
    painter.drawRect(x, y, width, height);
    
    // Piston rings
    painter.setPen(QPen(QColor(60, 60, 60), 1));
    for (int i = 1; i <= 3; ++i) {
        int ringY = y + (height * i / 4);
        painter.drawLine(x + 5, ringY, x + width - 5, ringY);
    }
    
    // Piston pin hole
    int pinX = x + width / 2;
    int pinY = y + height / 2;
    painter.setBrush(QColor(80, 80, 80));
    painter.drawEllipse(QPointF(pinX, pinY), 4, 4);
}

void PistonWidget::drawConnectingRod(QPainter &painter, int x1, int y1, int x2, int y2)
{
    // Rod body
    painter.setPen(QPen(QColor(100, 100, 120), 8, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(x1, y1, x2, y2);
    
    // Small end (piston pin)
    painter.setPen(QPen(QColor(80, 80, 100), 2));
    painter.setBrush(QColor(120, 120, 140));
    painter.drawEllipse(QPointF(x1, y1), 8, 8);
    
    // Big end (crank pin)
    painter.drawEllipse(QPointF(x2, y2), 12, 12);
}

void PistonWidget::drawCrankshaft(QPainter &painter, int centerX, int centerY, int radius, double angle)
{
    painter.save();
    painter.translate(centerX, centerY);
    painter.rotate(angle);

    // Crank web - extend all the way to crank pin
    painter.setPen(QPen(QColor(60, 60, 80), 2));
    painter.setBrush(QColor(100, 100, 120));
    painter.drawRect(-10, -15, radius + 10, 30);  // From center to crank pin
    
    // Crank pin (where connecting rod attaches)
    painter.setBrush(QColor(120, 120, 140));
    painter.drawEllipse(QPointF(radius, 0), 12, 12);
    
    // Main journal (center bearing)
    painter.setBrush(QColor(80, 80, 100));
    painter.drawEllipse(QPointF(0, 0), 20, 20);

    painter.restore();
    
    // Crankcase - transparent to see the crank mechanism
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.setBrush(Qt::NoBrush);  // No fill - transparent
    painter.drawRect(centerX - 40, centerY - 30, 80, 60);
}

void PistonWidget::drawSpark(QPainter &painter, int x, int y)
{
    painter.save();
    
    // Draw spark as radiating lines with bright center - BIGGER AND BRIGHTER
    painter.setPen(QPen(QColor(255, 255, 0), 4));  // Bright yellow, thicker
    
    // Draw 12 radiating spark lines (more lines for more visible effect)
    for (int i = 0; i < 12; ++i) {
        double angle = i * 30.0;
        double angleRad = qDegreesToRadians(angle);
        int x2 = x + 25 * qCos(angleRad);  // Longer lines
        int y2 = y + 25 * qSin(angleRad);
        painter.drawLine(x, y, x2, y2);
    }
    
    // Draw bright center - larger
    QRadialGradient sparkGradient(x, y, 20);
    sparkGradient.setColorAt(0.0, QColor(255, 255, 255, 255));  // Pure white center
    sparkGradient.setColorAt(0.3, QColor(255, 255, 0, 255));    // Bright yellow
    sparkGradient.setColorAt(0.6, QColor(255, 150, 0, 200));    // Orange
    sparkGradient.setColorAt(1.0, QColor(255, 100, 0, 0));      // Fade to transparent
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(sparkGradient);
    painter.drawEllipse(QPointF(x, y), 20, 20);  // Larger circle
    
    painter.restore();
}

void PistonWidget::drawIntakeFlow(QPainter &painter, int x, int y, double phase)
{
    painter.save();
    
    // Draw air particles flowing into intake port (blue particles)
    painter.setPen(Qt::NoPen);
    
    // Draw 3 particles at different positions based on phase
    for (int i = 0; i < 3; ++i) {
        double offset = fmod(phase * 2 + i * 60, 180.0);  // Animate position
        int particleX = x - static_cast<int>(offset / 3);
        int particleY = y + (i - 1) * 4;
        
        // Blue particle with fade
        int alpha = 255 - static_cast<int>(offset * 1.4);
        if (alpha < 0) alpha = 0;
        painter.setBrush(QColor(100, 150, 255, alpha));
        painter.drawEllipse(QPointF(particleX, particleY), 3, 3);
    }
    
    painter.restore();
}

void PistonWidget::drawExhaustFlow(QPainter &painter, int x, int y, double phase)
{
    painter.save();
    
    // Draw exhaust gas flowing out of exhaust port (red-orange particles)
    painter.setPen(Qt::NoPen);
    
    // Draw 3 particles at different positions based on phase
    for (int i = 0; i < 3; ++i) {
        double offset = fmod(phase * 2 + i * 60, 180.0);  // Animate position
        int particleX = x + static_cast<int>(offset / 3);
        int particleY = y + (i - 1) * 4;
        
        // Red-orange particle with fade
        int alpha = 255 - static_cast<int>(offset * 1.4);
        if (alpha < 0) alpha = 0;
        painter.setBrush(QColor(255, 100, 50, alpha));
        painter.drawEllipse(QPointF(particleX, particleY), 3, 3);
    }
    
    painter.restore();
}

void PistonWidget::drawParameters(QPainter &painter)
{
    Q_UNUSED(painter);
    // Parameters display removed - simple animation only
}

