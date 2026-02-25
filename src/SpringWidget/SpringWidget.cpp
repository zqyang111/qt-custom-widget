#include "SpringWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

SpringWidget::SpringWidget(QWidget *parent)
    : QWidget(parent)
    , m_amplitude(50.0)
    , m_frequency(0.5)
    , m_oscillating(false)
    , m_damping(0.0)
    , m_springConstant(100.0)
    , m_mass(1.0)
    , m_currentDisplacement(0.0)
    , m_velocity(0.0)
    , m_time(0.0)
    , m_naturalLength(120)
{
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &SpringWidget::onAnimationTimer);
    m_animationTimer->start(50);  // 20 FPS
}

SpringWidget::~SpringWidget()
{
}

void SpringWidget::setAmplitude(double amplitude)
{
    m_amplitude = qBound(10.0, amplitude, 100.0);
    if (m_oscillating) {
        m_currentDisplacement = m_amplitude;
        m_velocity = 0.0;
        m_time = 0.0;
    }
    update();
}

void SpringWidget::setFrequency(double frequency)
{
    m_frequency = qBound(0.1, frequency, 3.0);
    update();
}

void SpringWidget::setOscillating(bool oscillating)
{
    m_oscillating = oscillating;
    if (oscillating) {
        m_currentDisplacement = m_amplitude;
        m_velocity = 0.0;
        m_time = 0.0;
    } else {
        m_currentDisplacement = 0.0;
        m_velocity = 0.0;
    }
    update();
}

void SpringWidget::setDamping(double damping)
{
    m_damping = qBound(0.0, damping, 1.0);
    update();
}

void SpringWidget::setSpringConstant(double springConstant)
{
    m_springConstant = qBound(10.0, springConstant, 1000.0);
    update();
}

void SpringWidget::setMass(double mass)
{
    m_mass = qBound(0.1, mass, 10.0);
    update();
}

void SpringWidget::onAnimationTimer()
{
    if (m_oscillating) {
        double dt = 0.05;  // Time step (50ms)
        m_time += dt;
        
        // Simple harmonic motion with damping
        double omega = 2.0 * M_PI * m_frequency;  // Angular frequency
        double dampingFactor = qExp(-m_damping * m_time);
        
        // Calculate current effective amplitude (damped amplitude)
        double currentAmplitude = m_amplitude * dampingFactor;
        
        // Calculate displacement using damped harmonic motion formula
        // x(t) = A * e^(-?t) * cos(?t)
        m_currentDisplacement = currentAmplitude * qCos(omega * m_time);
        
        // Calculate velocity for visualization
        // v(t) = -A * e^(-?t) * [? * cos(?t) + ? * sin(?t)]
        m_velocity = -currentAmplitude * (m_damping * qCos(omega * m_time) + omega * qSin(omega * m_time));
        
        // Stop if effective amplitude becomes too small
        if (currentAmplitude < 1.0 && m_damping > 0.0) {
            m_oscillating = false;
            m_currentDisplacement = 0.0;
            m_velocity = 0.0;
        }
        
        update();
    }
}

void SpringWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    
    // Calculate positions
    int centerX = width() / 2;
    int supportY = 60;
    int massWidth = 80;
    int massHeight = 60;
    
    // Calculate displacement in pixels (scale factor: 1mm = 1 pixel)
    int displacementPx = static_cast<int>(m_currentDisplacement);
    
    // Draw fixed support at top
    drawFixedSupport(painter, centerX, supportY);
    
    // Draw spring
    int springTop = supportY + 30;
    int currentSpringLength = m_naturalLength + displacementPx;
    drawSpring(painter, centerX, springTop, m_naturalLength, displacementPx);
    
    // Draw mass
    int massY = springTop + currentSpringLength;
    drawMass(painter, centerX, massY, massWidth, massHeight);
    
    // Draw displacement indicator
    if (m_oscillating || m_currentDisplacement != 0) {
        drawDisplacementIndicator(painter, centerX + massWidth / 2 + 20, massY + massHeight / 2, m_currentDisplacement);
    }
    
    // Draw velocity vector
    if (m_oscillating && qAbs(m_velocity) > 0.1) {
        drawVelocityVector(painter, centerX, massY + massHeight / 2, m_velocity);
    }
    
    drawParameters(painter);
}

void SpringWidget::drawBackground(QPainter &painter)
{
    // Background
    QLinearGradient bgGradient(0, 0, 0, height());
    bgGradient.setColorAt(0.0, QColor(240, 245, 250));
    bgGradient.setColorAt(1.0, QColor(220, 230, 240));
    painter.fillRect(rect(), bgGradient);

    // Title
    painter.setPen(QColor(60, 60, 60));
    QFont titleFont = painter.font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 20, tr("Spring Oscillation"));
    
    // Draw equilibrium reference line
    painter.setPen(QPen(QColor(200, 200, 200), 1, Qt::DashLine));
    int equilibriumY = 60 + 30 + m_naturalLength + 30;  // Support + offset + natural length + half mass
    painter.drawLine(20, equilibriumY, width() - 20, equilibriumY);
    
    // Equilibrium label
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    painter.setPen(QColor(150, 150, 150));
    painter.drawText(width() - 100, equilibriumY - 5, tr("Equilibrium"));
}

void SpringWidget::drawFixedSupport(QPainter &painter, int x, int y)
{
    // Fixed support (ceiling)
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(100, 100, 100));
    painter.drawRect(x - 80, y, 160, 20);
    
    // Hatch pattern to indicate fixed support
    painter.setPen(QPen(QColor(70, 70, 70), 2));
    for (int i = -70; i < 80; i += 15) {
        painter.drawLine(x + i, y + 20, x + i - 10, y + 30);
    }
    
    // Draw bolt heads
    painter.setPen(QPen(QColor(60, 60, 60), 1));
    painter.setBrush(QColor(130, 130, 130));
    for (int i = -60; i <= 60; i += 40) {
        painter.drawEllipse(QPointF(x + i, y + 10), 5, 5);
    }
}

void SpringWidget::drawSpring(QPainter &painter, int x, int y, int naturalLength, double compression)
{
    // Spring parameters
    int currentLength = naturalLength + static_cast<int>(compression);
    int coilWidth = 25;
    int numCoils = 12;
    
    // Calculate color based on compression/extension
    QColor springColor;
    if (compression > 0) {
        // Extension - more red
        int red = qMin(255, 50 + static_cast<int>(compression * 2));
        springColor = QColor(red, 100, 150);
    } else {
        // Compression - more blue
        int blue = qMin(255, 150 + static_cast<int>(-compression * 2));
        springColor = QColor(100, 100, blue);
    }
    
    // Draw spring using individual coil segments to avoid overlap
    painter.setPen(QPen(springColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);
    
    // Draw each coil as a separate segment
    double yStep = currentLength / static_cast<double>(numCoils);
    int pointsPerCoil = 20;  // Number of points per coil for smooth curve
    
    for (int coil = 0; coil < numCoils; ++coil) {
        QPainterPath coilPath;
        
        for (int i = 0; i <= pointsPerCoil; ++i) {
            double t = i / static_cast<double>(pointsPerCoil);
            double angle = t * 2 * M_PI;
            double yPos = y + coil * yStep + t * yStep;
            double xOffset = coilWidth * qSin(angle);
            
            if (i == 0) {
                coilPath.moveTo(x + xOffset, yPos);
            } else {
                coilPath.lineTo(x + xOffset, yPos);
            }
        }
        
        painter.drawPath(coilPath);
    }
    
    // Draw connection lines at top and bottom with straight segments
    painter.setPen(QPen(springColor, 3, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(x, y, x, y + yStep * 0.05);
    painter.drawLine(x, y + currentLength - yStep * 0.05, x, y + currentLength);
    
    // Draw attachment point at top
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.setBrush(QColor(120, 120, 120));
    painter.drawEllipse(QPointF(x, y), 6, 6);
}

void SpringWidget::drawMass(QPainter &painter, int x, int y, int width, int height)
{
    // Mass shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 50));
    painter.drawRect(x - width / 2 + 4, y + 4, width, height);
    
    // Mass body with metallic gradient
    QLinearGradient massGradient(x - width / 2, y, x + width / 2, y);
    massGradient.setColorAt(0.0, QColor(180, 180, 200));
    massGradient.setColorAt(0.5, QColor(150, 150, 170));
    massGradient.setColorAt(1.0, QColor(120, 120, 140));
    
    painter.setPen(QPen(QColor(80, 80, 100), 2));
    painter.setBrush(massGradient);
    painter.drawRoundedRect(x - width / 2, y, width, height, 5, 5);
    
    // Mass label
    painter.setPen(QColor(80, 80, 80));
    QFont massFont = painter.font();
    massFont.setPointSize(9);
    massFont.setBold(true);
    painter.setFont(massFont);
    painter.drawText(QRect(x - width / 2, y, width, height), 
                     Qt::AlignCenter, QString("m = %1 kg").arg(m_mass, 0, 'f', 1));
    
    // Draw attachment hook at top
    painter.setPen(QPen(QColor(80, 80, 80), 3));
    painter.setBrush(QColor(120, 120, 120));
    int hookY = y - 5;
    painter.drawEllipse(QPointF(x, hookY), 6, 6);
}

void SpringWidget::drawDisplacementIndicator(QPainter &painter, int x, int y, double displacement)
{
    // Draw displacement arrow and value
    painter.setPen(QPen(QColor(255, 100, 0), 2));
    painter.setBrush(QColor(255, 100, 0));
    
    int equilibriumY = 60 + 30 + m_naturalLength + 30;
    int arrowLength = static_cast<int>(displacement);
    
    // Draw vertical line
    painter.drawLine(x, equilibriumY, x, y);
    
    // Draw arrow head
    if (displacement > 5) {
        QPolygon arrow;
        arrow << QPoint(x, y) << QPoint(x - 5, y - 8) << QPoint(x + 5, y - 8);
        painter.drawPolygon(arrow);
    } else if (displacement < -5) {
        QPolygon arrow;
        arrow << QPoint(x, y) << QPoint(x - 5, y + 8) << QPoint(x + 5, y + 8);
        painter.drawPolygon(arrow);
    }
    
    // Draw displacement value
    QFont dispFont = painter.font();
    dispFont.setPointSize(8);
    painter.setFont(dispFont);
    QString dispText = QString("%1 mm").arg(displacement, 0, 'f', 1);
    painter.drawText(x + 10, (equilibriumY + y) / 2, dispText);
}

void SpringWidget::drawVelocityVector(QPainter &painter, int x, int y, double velocity)
{
    // Draw velocity vector
    painter.setPen(QPen(QColor(0, 180, 0), 2, Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(QColor(0, 180, 0));
    
    // Scale velocity for visualization
    int vectorLength = static_cast<int>(velocity * 5);
    vectorLength = qBound(-60, vectorLength, 60);
    
    if (qAbs(vectorLength) > 5) {
        // Draw arrow
        painter.drawLine(x - 50, y, x - 50, y + vectorLength);
        
        // Draw arrow head
        if (vectorLength > 0) {
            QPolygon arrow;
            arrow << QPoint(x - 50, y + vectorLength) 
                  << QPoint(x - 55, y + vectorLength - 8) 
                  << QPoint(x - 45, y + vectorLength - 8);
            painter.drawPolygon(arrow);
        } else {
            QPolygon arrow;
            arrow << QPoint(x - 50, y + vectorLength) 
                  << QPoint(x - 55, y + vectorLength + 8) 
                  << QPoint(x - 45, y + vectorLength + 8);
            painter.drawPolygon(arrow);
        }
        
        // Draw label
        QFont velFont = painter.font();
        velFont.setPointSize(7);
        painter.setFont(velFont);
        painter.drawText(x - 90, y + vectorLength / 2, "v");
    }
}

void SpringWidget::drawParameters(QPainter &painter)
{
    int startY = height() - 110;
    int lineHeight = 18;
    
    painter.setPen(QColor(60, 60, 60));
    QFont paramFont = painter.font();
    paramFont.setPointSize(9);
    painter.setFont(paramFont);
    
    // Status
    QString status = m_oscillating ? tr("Oscillating") : tr("Stopped");
    QColor statusColor = m_oscillating ? QColor(0, 180, 0) : QColor(180, 0, 0);
    painter.setPen(statusColor);
    QFont statusFont = paramFont;
    statusFont.setBold(true);
    painter.setFont(statusFont);
    painter.drawText(QRect(10, startY, width() - 20, lineHeight), 
                     Qt::AlignLeft, tr("Status: ") + status);
    
    painter.setFont(paramFont);
    painter.setPen(QColor(60, 60, 60));
    
    // Current displacement
    painter.drawText(QRect(10, startY + lineHeight, width() - 20, lineHeight), 
                     Qt::AlignLeft, tr("Displacement: %1 mm").arg(m_currentDisplacement, 0, 'f', 1));
    
    // Amplitude
    painter.drawText(QRect(10, startY + lineHeight * 2, width() - 20, lineHeight), 
                     Qt::AlignLeft, tr("Amplitude: %1 mm").arg(m_amplitude, 0, 'f', 1));
    
    // Frequency
    painter.drawText(QRect(10, startY + lineHeight * 3, width() - 20, lineHeight), 
                     Qt::AlignLeft, tr("Frequency: %1 Hz").arg(m_frequency, 0, 'f', 2));
    
    // Spring constant
    painter.drawText(QRect(10, startY + lineHeight * 4, width() - 20, lineHeight), 
                     Qt::AlignLeft, tr("Spring k: %1 N/m").arg(m_springConstant, 0, 'f', 1));
    
    // Damping
    if (m_damping > 0) {
        painter.drawText(QRect(10, startY + lineHeight * 5, width() - 20, lineHeight), 
                         Qt::AlignLeft, tr("Damping: %1").arg(m_damping, 0, 'f', 2));
    }
}

