#include "ThreePhaseMeterWidget.h"
#include <QtMath>

ThreePhaseMeterWidget::ThreePhaseMeterWidget(QWidget *parent)
    : QWidget(parent)
    , m_voltageA(220.0)
    , m_voltageB(220.0)
    , m_voltageC(220.0)
    , m_currentA(10.0)
    , m_currentB(10.0)
    , m_currentC(10.0)
    , m_frequency(50.0)
    , m_powerFactor(0.95)
    , m_phaseAColor(QColor(255, 0, 0))
    , m_phaseBColor(QColor(255, 255, 0))
    , m_phaseCColor(QColor(0, 150, 255))
{
    setMinimumSize(250, 180);
}

ThreePhaseMeterWidget::~ThreePhaseMeterWidget()
{
}

void ThreePhaseMeterWidget::setVoltages(double va, double vb, double vc)
{
    m_voltageA = va;
    m_voltageB = vb;
    m_voltageC = vc;
    update();
}

void ThreePhaseMeterWidget::setCurrents(double ia, double ib, double ic)
{
    m_currentA = ia;
    m_currentB = ib;
    m_currentC = ic;
    update();
}

void ThreePhaseMeterWidget::setFrequency(double freq)
{
    m_frequency = freq;
    update();
}

void ThreePhaseMeterWidget::setPowerFactor(double pf)
{
    if (pf >= 0.0 && pf <= 1.0) {
        m_powerFactor = pf;
        update();
    }
}

void ThreePhaseMeterWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawPhaseVectors(painter);
    drawParameters(painter);
    drawPowerInfo(painter);
}

void ThreePhaseMeterWidget::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(30, 30, 30));
    
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);
    
    // Draw title
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRectF(10, 5, width() - 20, 20), Qt::AlignCenter, "THREE PHASE METER");
}

void ThreePhaseMeterWidget::drawPhaseVectors(QPainter &painter)
{
    int centerX = width() / 2;
    int centerY = 80;
    double vectorLength = 50.0;

    painter.save();
    painter.translate(centerX, centerY);

    // Draw circle
    painter.setPen(QPen(QColor(80, 80, 80), 1));
    painter.setBrush(QColor(50, 50, 50));
    painter.drawEllipse(-55, -55, 110, 110);

    // Draw phase vectors (120 degrees apart)
    struct PhaseVector {
        double angle;
        QString label;
        QColor color;
        double voltage;
    };

    QVector<PhaseVector> phases = {
        {-90.0, "A", m_phaseAColor, m_voltageA},        // Phase A at top
        {30.0, "B", m_phaseBColor, m_voltageB},         // Phase B at 120 deg
        {150.0, "C", m_phaseCColor, m_voltageC}         // Phase C at 240 deg
    };

    for (const auto &phase : phases) {
        painter.save();
        painter.rotate(phase.angle);

        // Calculate vector length based on voltage
        double length = (phase.voltage / 250.0) * vectorLength;

        // Draw vector line
        painter.setPen(QPen(phase.color, 3));
        painter.drawLine(0, 0, 0, static_cast<int>(-length));

        // Draw arrow head
        painter.setBrush(phase.color);
        QPolygon arrow;
        arrow << QPoint(0, static_cast<int>(-length))
              << QPoint(-4, static_cast<int>(-length + 8))
              << QPoint(4, static_cast<int>(-length + 8));
        painter.drawPolygon(arrow);

        // Draw phase label
        painter.save();
        painter.translate(0, -length - 12);
        painter.rotate(-phase.angle);
        
        QFont font = painter.font();
        font.setPointSize(12);
        font.setBold(true);
        painter.setFont(font);
        painter.setPen(phase.color);
        painter.drawText(QRectF(-10, -10, 20, 20), Qt::AlignCenter, phase.label);
        
        painter.restore();
        painter.restore();
    }

    // Draw center dot
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(-3, -3, 6, 6);

    painter.restore();
}

void ThreePhaseMeterWidget::drawParameters(QPainter &painter)
{
    int leftX = 10;
    int rightX = width() / 2 + 10;
    int startY = 145;
    int lineHeight = 18;

    painter.setPen(Qt::white);
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);

    // Left column - Voltages
    painter.setPen(m_phaseAColor);
    painter.drawText(leftX, startY, 100, lineHeight, Qt::AlignLeft,
                    QString("VA: %1 V").arg(m_voltageA, 0, 'f', 1));
    
    painter.setPen(m_phaseBColor);
    painter.drawText(leftX, startY + lineHeight, 100, lineHeight, Qt::AlignLeft,
                    QString("VB: %1 V").arg(m_voltageB, 0, 'f', 1));
    
    painter.setPen(m_phaseCColor);
    painter.drawText(leftX, startY + lineHeight * 2, 100, lineHeight, Qt::AlignLeft,
                    QString("VC: %1 V").arg(m_voltageC, 0, 'f', 1));

    // Right column - Currents
    painter.setPen(m_phaseAColor);
    painter.drawText(rightX, startY, 100, lineHeight, Qt::AlignLeft,
                    QString("IA: %1 A").arg(m_currentA, 0, 'f', 2));
    
    painter.setPen(m_phaseBColor);
    painter.drawText(rightX, startY + lineHeight, 100, lineHeight, Qt::AlignLeft,
                    QString("IB: %1 A").arg(m_currentB, 0, 'f', 2));
    
    painter.setPen(m_phaseCColor);
    painter.drawText(rightX, startY + lineHeight * 2, 100, lineHeight, Qt::AlignLeft,
                    QString("IC: %1 A").arg(m_currentC, 0, 'f', 2));
}

void ThreePhaseMeterWidget::drawPowerInfo(QPainter &painter)
{
    int startY = 200;
    
    painter.setPen(QColor(180, 180, 180));
    QFont font = painter.font();
    font.setPointSize(9);
    font.setBold(true);
    painter.setFont(font);

    // Frequency
    QString freqText = QString("Freq: %1 Hz").arg(m_frequency, 0, 'f', 1);
    painter.drawText(10, startY, 100, 16, Qt::AlignLeft, freqText);

    // Power Factor
    QString pfText = QString("PF: %1").arg(m_powerFactor, 0, 'f', 2);
    painter.drawText(width() / 2 + 10, startY, 100, 16, Qt::AlignLeft, pfText);

    // Total Power (rough estimate)
    double totalPower = (m_voltageA * m_currentA + m_voltageB * m_currentB + 
                        m_voltageC * m_currentC) * m_powerFactor / 1000.0;
    
    painter.setPen(QColor(0, 255, 100));
    font.setPointSize(10);
    painter.setFont(font);
    QString powerText = QString("Total: %1 kW").arg(totalPower, 0, 'f', 2);
    painter.drawText(10, 25, width() - 20, 18, Qt::AlignCenter, powerText);
}












