#include "SliderCrankWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

SliderCrankWidget::SliderCrankWidget(QWidget *parent)
    : QWidget(parent)
    , m_rpm(60.0)
    , m_crankLength(40.0)
    , m_connectingRodLength(120.0)
    , m_running(false)
    , m_crankAngle(0.0)
{
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &SliderCrankWidget::onAnimationTimer);
}

SliderCrankWidget::~SliderCrankWidget()
{
}

void SliderCrankWidget::setRPM(double rpm)
{
    m_rpm = qBound(0.0, rpm, 1000.0);
    update();
}

void SliderCrankWidget::setCrankLength(double length)
{
    m_crankLength = qBound(20.0, length, 80.0);
    update();
}

void SliderCrankWidget::setConnectingRodLength(double length)
{
    m_connectingRodLength = qBound(60.0, length, 200.0);
    update();
}

void SliderCrankWidget::setRunning(bool running)
{
    m_running = running;
    if (running) {
        m_animationTimer->start(50);  // 20 FPS
    } else {
        m_animationTimer->stop();
    }
    update();
}

void SliderCrankWidget::onAnimationTimer()
{
    if (m_running) {
        double angularVelocity = (m_rpm * 2.0 * M_PI) / 60.0;  // rad/s
        m_crankAngle += angularVelocity * 0.05;  // 50ms time step
        
        if (m_crankAngle >= 2.0 * M_PI) {
            m_crankAngle -= 2.0 * M_PI;
        }
        
        update();
    }
}

void SliderCrankWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    
    // Calculate positions (scale for display)
    int crankCenterX = 150;
    int crankCenterY = height() / 2;
    double crankLengthPx = m_crankLength * 1.5;
    double rodLengthPx = m_connectingRodLength * 1.5;
    
    // Calculate crank end position (point B)
    double crankEndX = crankCenterX + crankLengthPx * qCos(m_crankAngle);
    double crankEndY = crankCenterY + crankLengthPx * qSin(m_crankAngle);
    
    // Slider geometry
    int sliderWidth = 60;
    int sliderHeight = 80;
    // Slider pin is at fixed height (top of slider, 15 pixels from top)
    double sliderPinY = crankCenterY - sliderHeight + 15;
    
    // Calculate slider X position using fixed connecting rod length L
    // The connecting rod connects crank end (crankEndX, crankEndY) to slider pin (sliderX, sliderPinY)
    // Distance formula: sqrt((sliderX - crankEndX)^2 + (sliderPinY - crankEndY)^2) = rodLengthPx
    // Solve for sliderX:
    double dy = sliderPinY - crankEndY;
    double discriminant = rodLengthPx * rodLengthPx - dy * dy;
    
    double sliderX;
    if (discriminant >= 0) {
        // Take positive solution (slider moves to the right)
        sliderX = crankEndX + qSqrt(discriminant);
    } else {
        // If discriminant < 0, mechanism is in invalid configuration
        // This shouldn't happen with proper rod length, but handle gracefully
        sliderX = crankEndX;
    }
    
    // Draw ground
    drawFrame(painter, 0, crankCenterY, width());
    
    // Draw slider on ground (bottom at ground level)
    drawSlider(painter, static_cast<int>(sliderX), crankCenterY, sliderWidth, sliderHeight);
    
    // Draw connecting rod (connect to slider pin)
    drawConnectingRod(painter, static_cast<int>(crankEndX), static_cast<int>(crankEndY), 
                      static_cast<int>(sliderX), static_cast<int>(sliderPinY));
    
    // Draw crank
    drawCrank(painter, crankCenterX, crankCenterY, static_cast<int>(crankLengthPx), m_crankAngle);
    
    // Calculate actual connecting rod length for verification (should always equal rodLengthPx)
    double dx = sliderX - crankEndX;
    double dyRod = sliderPinY - crankEndY;
    double actualRodLength = qSqrt(dx * dx + dyRod * dyRod);
    
    drawParameters(painter, actualRodLength / 1.5);  // Convert back to mm
}

void SliderCrankWidget::drawBackground(QPainter &painter)
{
    QLinearGradient bgGradient(0, 0, 0, height());
    bgGradient.setColorAt(0.0, QColor(240, 240, 245));
    bgGradient.setColorAt(1.0, QColor(220, 220, 230));
    painter.fillRect(rect(), bgGradient);

    painter.setPen(QColor(60, 60, 60));
    QFont titleFont = painter.font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 20, tr("Slider-Crank Mechanism"));
}

void SliderCrankWidget::drawFrame(QPainter &painter, int x, int y, int width)
{
    // Draw ground line
    painter.setPen(QPen(QColor(80, 80, 80), 4));
    painter.drawLine(x, y, x + width, y);
    
    // Ground fill below the line
    QLinearGradient groundGradient(0, y, 0, y + 50);
    groundGradient.setColorAt(0.0, QColor(140, 120, 100));
    groundGradient.setColorAt(1.0, QColor(100, 85, 70));
    painter.setPen(Qt::NoPen);
    painter.setBrush(groundGradient);
    painter.drawRect(x, y, width, 50);
    
    // Ground hatch pattern
    painter.setPen(QPen(QColor(80, 70, 60), 2));
    for (int i = x; i < x + width; i += 15) {
        painter.drawLine(i, y, i - 10, y + 15);
    }
}

void SliderCrankWidget::drawCrank(QPainter &painter, int centerX, int centerY, int length, double angle)
{
    int endX = centerX + static_cast<int>(length * qCos(angle));
    int endY = centerY + static_cast<int>(length * qSin(angle));
    
    // Crank arm
    painter.setPen(QPen(QColor(200, 50, 50), 6, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(centerX, centerY, endX, endY);
    
    // Center bearing
    QRadialGradient centerGradient(centerX, centerY, 15);
    centerGradient.setColorAt(0.0, QColor(180, 180, 180));
    centerGradient.setColorAt(1.0, QColor(100, 100, 100));
    painter.setPen(QPen(QColor(60, 60, 60), 2));
    painter.setBrush(centerGradient);
    painter.drawEllipse(QPointF(centerX, centerY), 15, 15);
    
    // Crank pin
    QRadialGradient pinGradient(endX, endY, 10);
    pinGradient.setColorAt(0.0, QColor(220, 200, 100));
    pinGradient.setColorAt(1.0, QColor(150, 130, 50));
    painter.setPen(QPen(QColor(100, 80, 20), 2));
    painter.setBrush(pinGradient);
    painter.drawEllipse(QPointF(endX, endY), 10, 10);
}

void SliderCrankWidget::drawConnectingRod(QPainter &painter, int x1, int y1, int x2, int y2)
{
    // Rod body
    painter.setPen(QPen(QColor(70, 130, 180), 8, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(x1, y1, x2, y2);
    
    // Bearings at ends
    painter.setPen(QPen(QColor(60, 60, 60), 2));
    painter.setBrush(QColor(200, 200, 150));
    painter.drawEllipse(QPointF(x1, y1), 8, 8);
    painter.drawEllipse(QPointF(x2, y2), 8, 8);
}

void SliderCrankWidget::drawSlider(QPainter &painter, int x, int y, int width, int height)
{
    // Slider body (bottom aligned with ground)
    int sliderTop = y - height;
    int sliderBottom = y;
    
    QLinearGradient sliderGradient(x - width/2, 0, x + width/2, 0);
    sliderGradient.setColorAt(0.0, QColor(180, 180, 200));
    sliderGradient.setColorAt(0.5, QColor(150, 150, 170));
    sliderGradient.setColorAt(1.0, QColor(180, 180, 200));
    
    painter.setPen(QPen(QColor(80, 80, 100), 2));
    painter.setBrush(sliderGradient);
    painter.drawRoundedRect(x - width/2, sliderTop, width, height, 5, 5);
    
    // Connection point (at the top of slider)
    painter.setPen(QPen(QColor(60, 60, 60), 2));
    painter.setBrush(QColor(200, 200, 150));
    painter.drawEllipse(QPointF(x, sliderTop + 15), 10, 10);
    
    // Wheels/rollers at the bottom for ground contact
    painter.setPen(QPen(QColor(60, 60, 60), 2));
    painter.setBrush(QColor(100, 100, 100));
    painter.drawEllipse(QPointF(x - width/3, sliderBottom - 8), 8, 8);
    painter.drawEllipse(QPointF(x + width/3, sliderBottom - 8), 8, 8);
}

void SliderCrankWidget::drawParameters(QPainter &painter, double actualRodLength)
{
    int startY = height() - 100;
    int lineHeight = 16;
    
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
    painter.drawText(10, startY + lineHeight * 2, tr("Crank Length: %1 mm").arg(m_crankLength, 0, 'f', 0));
    painter.drawText(10, startY + lineHeight * 3, tr("Rod Length: %1 mm").arg(m_connectingRodLength, 0, 'f', 0));
    
    // Show actual rod length to verify it's constant
    painter.setPen(QColor(0, 150, 0));
    painter.drawText(10, startY + lineHeight * 4, tr("Actual Rod: %1 mm").arg(actualRodLength, 0, 'f', 1));
    
    painter.setPen(QColor(60, 60, 60));
    double angleInDegrees = m_crankAngle * 180.0 / M_PI;
    painter.drawText(width() - 200, startY + lineHeight * 2, tr("Crank Angle: %1 deg").arg(angleInDegrees, 0, 'f', 1));
}

