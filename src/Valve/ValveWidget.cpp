#include "ValveWidget.h"
#include <QtMath>

ValveWidget::ValveWidget(QWidget *parent)
    : QWidget(parent)
    , m_opening(0.0)
    , m_type(GateValve)
    , m_state(Closed)
    , m_openColor(QColor(0, 200, 0))
    , m_closedColor(QColor(200, 0, 0))
    , m_bodyColor(QColor(150, 150, 150))
{
    setMinimumSize(60, 80);
}

ValveWidget::~ValveWidget()
{
}

void ValveWidget::setOpening(double percentage)
{
    if (percentage < 0.0)
        percentage = 0.0;
    if (percentage > 100.0)
        percentage = 100.0;

    if (qAbs(m_opening - percentage) > 0.1) {
        m_opening = percentage;
        
        // Update state based on opening
        if (m_opening <= 0.0)
            m_state = Closed;
        else if (m_opening >= 100.0)
            m_state = Open;
        else
            m_state = Opening;
        
        update();
    }
}

void ValveWidget::setType(ValveType type)
{
    if (m_type != type) {
        m_type = type;
        update();
    }
}

void ValveWidget::setState(ValveState state)
{
    if (m_state != state) {
        m_state = state;
        update();
    }
}

void ValveWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(40, 40, 40));

    drawPipes(painter);
    drawValveBody(painter);
    drawValveIndicator(painter);
    drawStatus(painter);
}

void ValveWidget::drawValveBody(QPainter &painter)
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int valveSize = qMin(width(), height()) / 2;

    // Draw valve body based on type
    switch (m_type) {
    case GateValve: {
        // Draw diamond shape for gate valve
        QPolygon valveBody;
        valveBody << QPoint(centerX, centerY - valveSize)
                  << QPoint(centerX + valveSize, centerY)
                  << QPoint(centerX, centerY + valveSize)
                  << QPoint(centerX - valveSize, centerY);

        QLinearGradient gradient(centerX - valveSize, centerY, 
                                centerX + valveSize, centerY);
        gradient.setColorAt(0.0, m_bodyColor.darker(120));
        gradient.setColorAt(0.5, m_bodyColor);
        gradient.setColorAt(1.0, m_bodyColor.darker(120));

        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(80, 80, 80), 2));
        painter.drawPolygon(valveBody);
        break;
    }
    case BallValve: {
        // Draw circular shape for ball valve
        QRadialGradient gradient(centerX, centerY - valveSize / 4, valveSize);
        gradient.setColorAt(0.0, m_bodyColor.lighter(130));
        gradient.setColorAt(0.7, m_bodyColor);
        gradient.setColorAt(1.0, m_bodyColor.darker(120));

        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(80, 80, 80), 2));
        painter.drawEllipse(centerX - valveSize, centerY - valveSize,
                          valveSize * 2, valveSize * 2);
        break;
    }
    case ButterflyValve: {
        // Draw rectangle shape for butterfly valve
        QRect valveRect(centerX - valveSize, centerY - valveSize * 0.8,
                       valveSize * 2, valveSize * 1.6);

        QLinearGradient gradient(valveRect.topLeft(), valveRect.bottomLeft());
        gradient.setColorAt(0.0, m_bodyColor.darker(120));
        gradient.setColorAt(0.5, m_bodyColor);
        gradient.setColorAt(1.0, m_bodyColor.darker(120));

        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(80, 80, 80), 2));
        painter.drawRoundedRect(valveRect, 5, 5);
        break;
    }
    }
}

void ValveWidget::drawValveIndicator(QPainter &painter)
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int valveSize = qMin(width(), height()) / 2;

    // Draw opening indicator based on valve type
    switch (m_type) {
    case GateValve: {
        // Draw vertical bar showing gate position
        int barHeight = valveSize * 1.5;
        int barY = centerY - barHeight / 2;
        double gatePos = (100.0 - m_opening) / 100.0;  // Inverted: closed=top, open=bottom
        int indicatorY = barY + static_cast<int>(barHeight * gatePos);

        painter.setPen(QPen(QColor(255, 255, 0), 2));
        painter.setBrush(QColor(255, 255, 0));
        painter.drawRect(centerX - 8, indicatorY - 3, 16, 6);
        break;
    }
    case BallValve: {
        // Draw ball rotation indicator
        painter.save();
        painter.translate(centerX, centerY);
        double angle = (m_opening / 100.0) * 90.0;  // 0-90 degrees
        painter.rotate(angle);

        painter.setPen(QPen(QColor(255, 255, 0), 3));
        painter.drawLine(-valveSize / 2, 0, valveSize / 2, 0);

        painter.restore();
        break;
    }
    case ButterflyValve: {
        // Draw butterfly disc rotation
        painter.save();
        painter.translate(centerX, centerY);
        double angle = (m_opening / 100.0) * 90.0;  // 0-90 degrees
        painter.rotate(angle);

        QLinearGradient gradient(-valveSize * 0.8, 0, valveSize * 0.8, 0);
        gradient.setColorAt(0.0, QColor(255, 255, 0));
        gradient.setColorAt(0.5, QColor(255, 255, 100));
        gradient.setColorAt(1.0, QColor(255, 255, 0));

        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(200, 200, 0), 2));
        painter.drawEllipse(static_cast<int>(-valveSize * 0.8), -5,
                          static_cast<int>(valveSize * 1.6), 10);

        painter.restore();
        break;
    }
    }
}

void ValveWidget::drawPipes(QPainter &painter)
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int pipeWidth = width() / 4;

    // Determine pipe color based on valve opening
    QColor pipeColor;
    if (m_opening > 50.0)
        pipeColor = m_openColor;
    else if (m_opening > 0.0)
        pipeColor = QColor(255, 165, 0);  // Orange for partial opening
    else
        pipeColor = m_closedColor;

    // Top pipe
    QLinearGradient topGradient(centerX - pipeWidth / 2, 0,
                                centerX + pipeWidth / 2, 0);
    topGradient.setColorAt(0.0, pipeColor.darker(150));
    topGradient.setColorAt(0.5, pipeColor);
    topGradient.setColorAt(1.0, pipeColor.darker(150));

    painter.setBrush(topGradient);
    painter.setPen(QPen(QColor(80, 80, 80), 1));
    painter.drawRect(centerX - pipeWidth / 2, 0, pipeWidth, centerY - height() / 4);

    // Bottom pipe
    QLinearGradient bottomGradient(centerX - pipeWidth / 2, 0,
                                   centerX + pipeWidth / 2, 0);
    bottomGradient.setColorAt(0.0, pipeColor.darker(150));
    bottomGradient.setColorAt(0.5, pipeColor);
    bottomGradient.setColorAt(1.0, pipeColor.darker(150));

    painter.setBrush(bottomGradient);
    painter.drawRect(centerX - pipeWidth / 2, centerY + height() / 4,
                    pipeWidth, centerY - height() / 4);
}

void ValveWidget::drawStatus(QPainter &painter)
{
    // Draw opening percentage
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);

    QString statusText = QString::number(m_opening, 'f', 0) + "%";
    painter.drawText(QRectF(0, height() - 20, width(), 18),
                    Qt::AlignCenter, statusText);

    // Draw state indicator
    QString stateText;
    switch (m_state) {
    case Closed:   stateText = "CLOSED"; break;
    case Opening:  stateText = "OPENING"; break;
    case Open:     stateText = "OPEN"; break;
    case Closing:  stateText = "CLOSING"; break;
    }

    font.setPointSize(8);
    painter.setFont(font);
    painter.drawText(QRectF(0, 2, width(), 16), Qt::AlignCenter, stateText);
}












