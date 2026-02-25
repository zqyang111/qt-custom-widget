#include "MotorControllerWidget.h"
#include <QtMath>

MotorControllerWidget::MotorControllerWidget(QWidget *parent)
    : QWidget(parent)
    , m_state(Stopped)
    , m_rpm(0.0)
    , m_torque(0.0)
    , m_power(0.0)
    , m_ratedRPM(1500.0)
    , m_rotationAngle(0.0)
    , m_animationTimer(new QTimer(this))
    , m_runningColor(QColor(0, 200, 0))
    , m_stoppedColor(QColor(150, 150, 150))
    , m_faultColor(QColor(255, 0, 0))
{
    setMinimumSize(200, 150);

    connect(m_animationTimer, &QTimer::timeout, this, &MotorControllerWidget::onAnimationTimer);
    m_animationTimer->start(50);  // 20 FPS
}

MotorControllerWidget::~MotorControllerWidget()
{
}

void MotorControllerWidget::setState(MotorState state)
{
    if (m_state != state) {
        m_state = state;
        update();
    }
}

void MotorControllerWidget::setRPM(double rpm)
{
    if (rpm < 0) rpm = 0;
    
    if (qAbs(m_rpm - rpm) > 0.1) {
        m_rpm = rpm;
        
        // Auto update state based on RPM
        if (m_rpm > 10) {
            if (m_state != RunningForward && m_state != RunningReverse && m_state != Fault)
                m_state = RunningForward;
        } else {
            if (m_state == RunningForward || m_state == RunningReverse)
                m_state = Stopped;
        }
        
        update();
    }
}

void MotorControllerWidget::setTorque(double torque)
{
    m_torque = torque;
    update();
}

void MotorControllerWidget::setPower(double power)
{
    m_power = power;
    update();
}

void MotorControllerWidget::setRatedRPM(double ratedRPM)
{
    if (ratedRPM > 0) {
        m_ratedRPM = ratedRPM;
        update();
    }
}

void MotorControllerWidget::onAnimationTimer()
{
    if (m_state == RunningForward) {
        m_rotationAngle += (m_rpm / m_ratedRPM) * 10.0;
        if (m_rotationAngle >= 360.0) m_rotationAngle -= 360.0;
        update();
    } else if (m_state == RunningReverse) {
        m_rotationAngle -= (m_rpm / m_ratedRPM) * 10.0;
        if (m_rotationAngle < 0.0) m_rotationAngle += 360.0;
        update();
    }
}

void MotorControllerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawMotorSymbol(painter);
    drawParameters(painter);
    drawStateIndicator(painter);
}

void MotorControllerWidget::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(40, 40, 40));
    
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);
}

void MotorControllerWidget::drawMotorSymbol(QPainter &painter)
{
    int centerX = width() / 2;
    int centerY = 75;  // Move down to avoid overlap with state bar
    int motorSize = 45;

    // Draw motor body (circle)
    QRadialGradient motorGradient(centerX, centerY - 10, motorSize);
    motorGradient.setColorAt(0.0, QColor(180, 180, 180));
    motorGradient.setColorAt(0.7, QColor(140, 140, 140));
    motorGradient.setColorAt(1.0, QColor(100, 100, 100));

    painter.setBrush(motorGradient);
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.drawEllipse(centerX - motorSize, centerY - motorSize,
                       motorSize * 2, motorSize * 2);

    // Draw motor label "M"
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(20);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRectF(centerX - 20, centerY - 15, 40, 30),
                    Qt::AlignCenter, "M");

    // Draw rotation indicator if running
    if (m_state == RunningForward || m_state == RunningReverse) {
        painter.save();
        painter.translate(centerX, centerY);
        painter.rotate(m_rotationAngle);

        // Draw rotation arrow
        painter.setPen(QPen(m_runningColor, 3));
        painter.setBrush(Qt::NoBrush);
        
        int arcRadius = motorSize - 10;
        int startAngle = 90;
        int spanAngle = (m_state == RunningForward) ? -270 : 270;
        
        painter.drawArc(-arcRadius, -arcRadius, arcRadius * 2, arcRadius * 2,
                       startAngle * 16, spanAngle * 16);

        // Draw arrow head
        double arrowAngle = (m_state == RunningForward) ? -180 : 180;
        painter.rotate(arrowAngle);
        
        QPolygon arrowHead;
        arrowHead << QPoint(0, -arcRadius)
                  << QPoint(-6, -arcRadius + 10)
                  << QPoint(6, -arcRadius + 10);
        painter.setBrush(m_runningColor);
        painter.drawPolygon(arrowHead);

        painter.restore();
    }
}

void MotorControllerWidget::drawParameters(QPainter &painter)
{
    int startY = 135;  // Adjust to match motor position
    int lineHeight = 20;

    painter.setPen(Qt::white);
    QFont labelFont = painter.font();
    labelFont.setPointSize(9);
    painter.setFont(labelFont);

    // RPM
    QString rpmText = QString("RPM: %1").arg(m_rpm, 0, 'f', 0);
    painter.drawText(15, startY, 120, lineHeight, Qt::AlignLeft | Qt::AlignVCenter, rpmText);

    // Torque
    QString torqueText = QString("Torque: %1 Nm").arg(m_torque, 0, 'f', 1);
    painter.drawText(15, startY + lineHeight, 150, lineHeight, 
                    Qt::AlignLeft | Qt::AlignVCenter, torqueText);

    // Power
    QString powerText = QString("Power: %1 kW").arg(m_power, 0, 'f', 2);
    painter.drawText(15, startY + lineHeight * 2, 150, lineHeight,
                    Qt::AlignLeft | Qt::AlignVCenter, powerText);

    // Load percentage
    double loadPercent = (m_rpm / m_ratedRPM) * 100.0;
    if (loadPercent > 100.0) loadPercent = 100.0;
    
    QFont valueFont = painter.font();
    valueFont.setPointSize(10);
    valueFont.setBold(true);
    painter.setFont(valueFont);
    
    QString loadText = QString("%1%").arg(loadPercent, 0, 'f', 0);
    painter.drawText(width() - 80, startY, 70, lineHeight,
                    Qt::AlignRight | Qt::AlignVCenter, loadText);
    
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    painter.drawText(width() - 80, startY + lineHeight, 70, lineHeight,
                    Qt::AlignRight | Qt::AlignVCenter, "Load");
}

void MotorControllerWidget::drawStateIndicator(QPainter &painter)
{
    // Draw state at top
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);

    QString stateText;
    QColor stateColor;

    switch (m_state) {
    case Stopped:
        stateText = "STOPPED";
        stateColor = m_stoppedColor;
        break;
    case Starting:
        stateText = "STARTING";
        stateColor = QColor(255, 200, 0);
        break;
    case RunningForward:
        stateText = "RUNNING FWD";
        stateColor = m_runningColor;
        break;
    case RunningReverse:
        stateText = "RUNNING REV";
        stateColor = QColor(0, 150, 255);
        break;
    case Stopping:
        stateText = "STOPPING";
        stateColor = QColor(255, 165, 0);
        break;
    case Fault:
        stateText = "FAULT";
        stateColor = m_faultColor;
        break;
    }

    // Draw state background
    QRect stateRect(10, 5, width() - 20, 20);
    painter.setBrush(stateColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(stateRect, 3, 3);

    // Draw state text
    painter.setPen(m_state == Starting ? Qt::black : Qt::white);
    painter.drawText(stateRect, Qt::AlignCenter, stateText);
}

