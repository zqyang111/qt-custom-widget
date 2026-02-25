#include "LEDWidget.h"

LEDWidget::LEDWidget(QWidget *parent)
    : QWidget(parent)
    , m_color(Green)
    , m_state(Off)
    , m_isOn(false)
    , m_flashTimer(new QTimer(this))
    , m_flashInterval(500)
{
    setMinimumSize(20, 20);
    connect(m_flashTimer, &QTimer::timeout, this, &LEDWidget::onFlashTimer);
}

LEDWidget::~LEDWidget()
{
}

void LEDWidget::setColor(LEDColor color)
{
    if (m_color != color) {
        m_color = color;
        update();
    }
}

void LEDWidget::setState(LEDState state)
{
    if (m_state != state) {
        m_state = state;

        m_flashTimer->stop();

        switch (m_state) {
        case Off:
            m_isOn = false;
            break;
        case On:
            m_isOn = true;
            break;
        case Flashing:
            m_isOn = true;
            m_flashTimer->start(m_flashInterval);
            break;
        }

        update();
    }
}

void LEDWidget::setFlashInterval(int milliseconds)
{
    if (milliseconds > 0) {
        m_flashInterval = milliseconds;
        if (m_state == Flashing) {
            m_flashTimer->stop();
            m_flashTimer->start(m_flashInterval);
        }
    }
}

void LEDWidget::turnOn()
{
    setState(On);
}

void LEDWidget::turnOff()
{
    setState(Off);
}

void LEDWidget::toggle()
{
    if (m_state == On)
        setState(Off);
    else
        setState(On);
}

void LEDWidget::onFlashTimer()
{
    m_isOn = !m_isOn;
    update();
}

void LEDWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawLED(painter);
}

void LEDWidget::drawLED(QPainter &painter)
{
    int side = qMin(width(), height());
    int margin = side / 10;
    int diameter = side - 2 * margin;

    painter.translate(width() / 2, height() / 2);

    // Draw LED shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 100));
    painter.drawEllipse(-diameter / 2 + 2, -diameter / 2 + 2, diameter, diameter);

    // Draw LED base (off state)
    QColor baseColor = getLEDColor().darker(300);
    QRadialGradient baseGradient(0, -diameter / 4, diameter);
    baseGradient.setColorAt(0.0, baseColor.lighter(150));
    baseGradient.setColorAt(0.5, baseColor);
    baseGradient.setColorAt(1.0, baseColor.darker(150));

    painter.setBrush(baseGradient);
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.drawEllipse(-diameter / 2, -diameter / 2, diameter, diameter);

    // Draw LED glow when on
    if (m_isOn) {
        QColor ledColor = getLEDColor();

        // Outer glow
        QRadialGradient glowGradient(0, 0, diameter * 0.8);
        glowGradient.setColorAt(0.0, ledColor);
        glowGradient.setColorAt(0.5, ledColor);
        glowGradient.setColorAt(1.0, QColor(ledColor.red(), ledColor.green(), 
                                            ledColor.blue(), 0));
        painter.setBrush(glowGradient);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(-diameter * 0.8 / 2, -diameter * 0.8 / 2, 
                          diameter * 0.8, diameter * 0.8);

        // LED body
        QRadialGradient ledGradient(0, -diameter / 4, diameter / 2);
        ledGradient.setColorAt(0.0, ledColor.lighter(180));
        ledGradient.setColorAt(0.4, ledColor);
        ledGradient.setColorAt(1.0, ledColor.darker(120));

        painter.setBrush(ledGradient);
        painter.setPen(QPen(ledColor.lighter(120), 1));
        painter.drawEllipse(-diameter / 2, -diameter / 2, diameter, diameter);

        // Highlight
        QRadialGradient highlightGradient(-diameter / 6, -diameter / 6, diameter / 3);
        highlightGradient.setColorAt(0.0, QColor(255, 255, 255, 200));
        highlightGradient.setColorAt(0.5, QColor(255, 255, 255, 80));
        highlightGradient.setColorAt(1.0, QColor(255, 255, 255, 0));

        painter.setBrush(highlightGradient);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(-diameter / 3, -diameter / 3, diameter / 2, diameter / 2);
    }
}

QColor LEDWidget::getLEDColor() const
{
    switch (m_color) {
    case Red:
        return QColor(255, 0, 0);
    case Green:
        return QColor(0, 255, 0);
    case Blue:
        return QColor(0, 120, 255);
    case Yellow:
        return QColor(255, 255, 0);
    case Orange:
        return QColor(255, 165, 0);
    case White:
        return QColor(255, 255, 255);
    default:
        return QColor(0, 255, 0);
    }
}












