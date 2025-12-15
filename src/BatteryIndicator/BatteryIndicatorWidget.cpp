#include "BatteryIndicatorWidget.h"

BatteryIndicatorWidget::BatteryIndicatorWidget(QWidget *parent)
    : QWidget(parent)
    , m_chargeLevel(75.0)
    , m_voltage(12.6)
    , m_current(0.0)
    , m_chargingState(Discharging)
    , m_flashTimer(new QTimer(this))
    , m_flashState(false)
    , m_fullColor(QColor(0, 200, 0))
    , m_mediumColor(QColor(255, 200, 0))
    , m_lowColor(QColor(255, 100, 0))
    , m_criticalColor(QColor(255, 0, 0))
{
    setMinimumSize(150, 100);

    connect(m_flashTimer, &QTimer::timeout, this, &BatteryIndicatorWidget::onFlashTimer);
    m_flashTimer->start(500);
}

BatteryIndicatorWidget::~BatteryIndicatorWidget()
{
}

void BatteryIndicatorWidget::setChargeLevel(double percentage)
{
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;

    m_chargeLevel = percentage;
    update();
}

void BatteryIndicatorWidget::setVoltage(double voltage)
{
    m_voltage = voltage;
    update();
}

void BatteryIndicatorWidget::setCurrent(double current)
{
    m_current = current;
    update();
}

void BatteryIndicatorWidget::setChargingState(ChargingState state)
{
    if (m_chargingState != state) {
        m_chargingState = state;
        update();
    }
}

void BatteryIndicatorWidget::onFlashTimer()
{
    m_flashState = !m_flashState;
    
    // Only update if we need to flash
    if (m_chargeLevel < 20 || m_chargingState == Charging) {
        update();
    }
}

void BatteryIndicatorWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawBattery(painter);
    drawChargeLevel(painter);
    drawChargingIndicator(painter);
    drawParameters(painter);
}

void BatteryIndicatorWidget::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(40, 40, 40));
}

void BatteryIndicatorWidget::drawBattery(QPainter &painter)
{
    int batteryWidth = 120;
    int batteryHeight = 50;
    int batteryX = (width() - batteryWidth) / 2;
    int batteryY = 25;

    // Draw battery terminal (positive)
    painter.setBrush(QColor(150, 150, 150));
    painter.setPen(Qt::NoPen);
    painter.drawRect(batteryX + batteryWidth, batteryY + 15, 8, 20);

    // Draw battery body
    QLinearGradient bodyGradient(batteryX, batteryY, batteryX, batteryY + batteryHeight);
    bodyGradient.setColorAt(0.0, QColor(180, 180, 180));
    bodyGradient.setColorAt(1.0, QColor(140, 140, 140));

    painter.setBrush(bodyGradient);
    painter.setPen(QPen(QColor(100, 100, 100), 3));
    painter.drawRoundedRect(batteryX, batteryY, batteryWidth, batteryHeight, 5, 5);

    // Draw inner frame
    painter.setBrush(QColor(50, 50, 50));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(batteryX + 4, batteryY + 4,
                          batteryWidth - 8, batteryHeight - 8, 3, 3);
}

void BatteryIndicatorWidget::drawChargeLevel(QPainter &painter)
{
    int batteryWidth = 120;
    int batteryHeight = 50;
    int batteryX = (width() - batteryWidth) / 2;
    int batteryY = 25;

    double fillWidth = (batteryWidth - 12) * (m_chargeLevel / 100.0);

    // Choose color based on charge level
    QColor levelColor;
    if (m_chargeLevel > 60)
        levelColor = m_fullColor;
    else if (m_chargeLevel > 30)
        levelColor = m_mediumColor;
    else if (m_chargeLevel > 15)
        levelColor = m_lowColor;
    else
        levelColor = m_criticalColor;

    // Flash if critical level
    if (m_chargeLevel < 20 && !m_flashState && m_chargingState != Charging) {
        levelColor = levelColor.darker(300);
    }

    // Draw charge level
    QLinearGradient fillGradient(batteryX + 6, batteryY + 6,
                                 batteryX + 6, batteryY + batteryHeight - 6);
    fillGradient.setColorAt(0.0, levelColor.lighter(120));
    fillGradient.setColorAt(1.0, levelColor);

    painter.setBrush(fillGradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(batteryX + 6, batteryY + 6,
                          static_cast<int>(fillWidth), batteryHeight - 12, 2, 2);

    // Draw percentage text
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);

    QString percentText = QString::number(static_cast<int>(m_chargeLevel)) + "%";
    painter.drawText(QRectF(batteryX, batteryY, batteryWidth, batteryHeight),
                    Qt::AlignCenter, percentText);
}

void BatteryIndicatorWidget::drawChargingIndicator(QPainter &painter)
{
    if (m_chargingState != Charging) return;

    int batteryWidth = 120;
    int batteryX = (width() - batteryWidth) / 2;
    int batteryY = 25;
    int batteryHeight = 50;

    // Draw charging bolt symbol - moderate size and visible
    int centerX = batteryX + batteryWidth / 2;
    int centerY = batteryY + batteryHeight / 2;

    // Draw glow effect (always visible when charging)
    painter.setPen(Qt::NoPen);
    QRadialGradient glowGradient(centerX, centerY, 15);
    glowGradient.setColorAt(0.0, QColor(255, 255, 0, 150));
    glowGradient.setColorAt(0.6, QColor(255, 255, 0, 80));
    glowGradient.setColorAt(1.0, QColor(255, 255, 0, 0));
    painter.setBrush(glowGradient);
    painter.drawEllipse(centerX - 15, centerY - 15, 30, 30);

    // Draw charging bolt symbol (flashing for extra attention)
    if (m_flashState) {
        // Standard lightning bolt shape ?
        painter.setPen(QPen(QColor(200, 200, 0), 1));
        painter.setBrush(QColor(255, 255, 0));

        // Draw simple zigzag lightning bolt ? (polyline style)
        QPolygon bolt;
        // Simple folded line from top to bottom
        bolt << QPoint(centerX - 4, centerY - 15)      // Top left start
             << QPoint(centerX + 1, centerY - 15)      // Top right
             << QPoint(centerX - 2, centerY - 2)       // Middle left (diagonal down)
             << QPoint(centerX + 3, centerY - 2)       // Middle right (step)
             << QPoint(centerX, centerY + 15)          // Bottom tip (diagonal down)
             << QPoint(centerX + 1, centerY)           // Return path start
             << QPoint(centerX - 4, centerY)           // Return middle left
             << QPoint(centerX + 1, centerY - 13);     // Return to top (close)

        painter.drawPolygon(bolt);
        
        // Add bright highlight on top edge
        painter.setBrush(QColor(255, 255, 255, 230));
        painter.setPen(Qt::NoPen);
        QPolygon highlight;
        highlight << QPoint(centerX - 3, centerY - 14)
                  << QPoint(centerX, centerY - 14)
                  << QPoint(centerX - 1, centerY - 8);
        painter.drawPolygon(highlight);
    }
}

void BatteryIndicatorWidget::drawParameters(QPainter &painter)
{
    int startY = 85;
    int lineHeight = 18;

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);

    // Voltage
    QString voltageText = QString("Voltage: %1 V").arg(m_voltage, 0, 'f', 2);
    painter.drawText(15, startY, width() - 30, lineHeight,
                    Qt::AlignLeft | Qt::AlignVCenter, voltageText);

    // Current
    QString currentText = QString("Current: %1 A").arg(m_current, 0, 'f', 2);
    painter.drawText(15, startY + lineHeight, width() - 30, lineHeight,
                    Qt::AlignLeft | Qt::AlignVCenter, currentText);

    // Remaining time estimate (if discharging)
    if (m_chargingState == Discharging && m_current > 0.1) {
        // Simple estimate: assume linear discharge
        double remainingHours = (m_chargeLevel / 100.0) * 100.0 / m_current;
        QString timeText = QString("Est. Time: %1h %2m")
                          .arg(static_cast<int>(remainingHours))
                          .arg(static_cast<int>((remainingHours - static_cast<int>(remainingHours)) * 60));
        
        font.setPointSize(8);
        painter.setFont(font);
        painter.drawText(15, startY + lineHeight * 2, width() - 30, lineHeight,
                        Qt::AlignLeft | Qt::AlignVCenter, timeText);
    }
}

