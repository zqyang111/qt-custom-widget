#include "TankWidget.h"
#include <QtMath>

TankWidget::TankWidget(QWidget *parent)
    : QWidget(parent)
    , m_level(50.0)
    , m_temperature(25.0)
    , m_pressure(1.0)
    , m_capacity(1000.0)
    , m_capacityUnit("L")
    , m_shape(Cylindrical)
    , m_liquidColor(QColor(100, 150, 255))
    , m_tankColor(QColor(180, 180, 180))
{
    setMinimumSize(100, 200);
}

TankWidget::~TankWidget()
{
}

void TankWidget::setLevel(double percentage)
{
    if (percentage < 0.0)
        percentage = 0.0;
    if (percentage > 100.0)
        percentage = 100.0;

    if (qAbs(m_level - percentage) > 0.1) {
        m_level = percentage;
        update();
    }
}

void TankWidget::setTemperature(double temp)
{
    if (qAbs(m_temperature - temp) > 0.1) {
        m_temperature = temp;
        update();
    }
}

void TankWidget::setPressure(double pressure)
{
    if (qAbs(m_pressure - pressure) > 0.01) {
        m_pressure = pressure;
        update();
    }
}

void TankWidget::setShape(TankShape shape)
{
    if (m_shape != shape) {
        m_shape = shape;
        update();
    }
}

void TankWidget::setCapacity(double capacity, const QString &unit)
{
    m_capacity = capacity;
    m_capacityUnit = unit;
    update();
}

void TankWidget::setLiquidColor(const QColor &color)
{
    m_liquidColor = color;
    update();
}

void TankWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(40, 40, 40));

    drawTankBody(painter);
    drawLiquid(painter);
    drawScale(painter);
    drawReadings(painter);
}

void TankWidget::drawTankBody(QPainter &painter)
{
    int tankWidth = width() * 0.5;
    int tankHeight = height() - 100;  // Reserve 100 pixels for text at bottom
    int tankX = (width() - tankWidth) / 2;
    int tankY = 25;  // Top margin

    // Draw tank based on shape
    switch (m_shape) {
    case Cylindrical: {
        // Draw top ellipse
        QLinearGradient topGradient(tankX, tankY, tankX, tankY + 10);
        topGradient.setColorAt(0.0, m_tankColor.lighter(130));
        topGradient.setColorAt(1.0, m_tankColor);

        painter.setBrush(topGradient);
        painter.setPen(QPen(QColor(100, 100, 100), 2));
        painter.drawEllipse(tankX, tankY, tankWidth, 10);

        // Draw cylinder body
        QLinearGradient bodyGradient(tankX, tankY, tankX + tankWidth, tankY);
        bodyGradient.setColorAt(0.0, m_tankColor.darker(120));
        bodyGradient.setColorAt(0.5, m_tankColor);
        bodyGradient.setColorAt(1.0, m_tankColor.darker(120));

        painter.setBrush(bodyGradient);
        painter.setPen(QPen(QColor(100, 100, 100), 2));
        painter.drawRect(tankX, tankY + 5, tankWidth, tankHeight);

        // Draw bottom ellipse
        painter.setBrush(m_tankColor.darker(110));
        painter.drawEllipse(tankX, tankY + tankHeight, tankWidth, 10);
        break;
    }
    case Rectangular: {
        QLinearGradient gradient(tankX, tankY, tankX + tankWidth, tankY);
        gradient.setColorAt(0.0, m_tankColor.darker(120));
        gradient.setColorAt(0.5, m_tankColor);
        gradient.setColorAt(1.0, m_tankColor.darker(120));

        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(100, 100, 100), 2));
        painter.drawRect(tankX, tankY, tankWidth, tankHeight);
        break;
    }
    case Spherical: {
        QRadialGradient gradient(tankX + tankWidth / 2, 
                                tankY + tankHeight / 2 - tankHeight / 6,
                                tankHeight / 2);
        gradient.setColorAt(0.0, m_tankColor.lighter(130));
        gradient.setColorAt(0.7, m_tankColor);
        gradient.setColorAt(1.0, m_tankColor.darker(130));

        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(100, 100, 100), 2));
        painter.drawEllipse(tankX, tankY, tankWidth, tankHeight);
        break;
    }
    }

    // Make tank semi-transparent to see liquid
    painter.setOpacity(0.7);
    painter.fillRect(tankX + 2, tankY + 2, tankWidth - 4, tankHeight - 4,
                     QColor(60, 60, 60, 100));
    painter.setOpacity(1.0);
}

void TankWidget::drawLiquid(QPainter &painter)
{
    int tankWidth = width() * 0.5;
    int tankHeight = height() - 100;
    int tankX = (width() - tankWidth) / 2;
    int tankY = 25;

    double liquidHeight = (tankHeight - 4) * (m_level / 100.0);
    int liquidY = tankY + tankHeight - 2 - static_cast<int>(liquidHeight);

    if (liquidHeight > 0) {
        QLinearGradient liquidGradient(tankX, liquidY, 
                                      tankX, liquidY + liquidHeight);
        liquidGradient.setColorAt(0.0, m_liquidColor.lighter(120));
        liquidGradient.setColorAt(0.5, m_liquidColor);
        liquidGradient.setColorAt(1.0, m_liquidColor.darker(110));

        painter.setBrush(liquidGradient);
        painter.setPen(Qt::NoPen);

        switch (m_shape) {
        case Cylindrical:
        case Rectangular:
            painter.drawRect(tankX + 2, liquidY, tankWidth - 4, 
                           static_cast<int>(liquidHeight));
            break;
        case Spherical: {
            // For spherical tank, adjust width based on height
            double radius = tankHeight / 2.0;
            double centerY = tankY + tankHeight / 2.0;
            double relativeY = liquidY - centerY;
            
            painter.setClipRect(tankX + 2, liquidY, tankWidth - 4,
                              static_cast<int>(liquidHeight));
            painter.drawEllipse(tankX + 2, tankY + 2, tankWidth - 4, tankHeight - 4);
            painter.setClipping(false);
            break;
        }
        }

        // Draw liquid surface wave
        painter.setPen(QPen(m_liquidColor.lighter(140), 2));
        painter.drawLine(tankX + 4, liquidY, tankX + tankWidth - 4, liquidY);
    }
}

void TankWidget::drawScale(QPainter &painter)
{
    int tankWidth = width() * 0.5;
    int tankHeight = height() - 100;
    int tankX = (width() - tankWidth) / 2;
    int tankY = 25;
    int tankRight = tankX + tankWidth;

    painter.setPen(QPen(Qt::white, 1));
    QFont font = painter.font();
    font.setPointSize(7);
    painter.setFont(font);

    // Draw level marks
    for (int i = 0; i <= 10; ++i) {
        int y = tankY + tankHeight - (i * tankHeight / 10);
        int level = i * 10;

        if (i % 2 == 0) {
            painter.drawLine(tankRight + 2, y, tankRight + 8, y);
            painter.drawText(tankRight + 10, y - 6, 30, 12,
                           Qt::AlignLeft | Qt::AlignVCenter,
                           QString::number(level) + "%");
        } else {
            painter.drawLine(tankRight + 2, y, tankRight + 5, y);
        }
    }
}

void TankWidget::drawReadings(QPainter &painter)
{
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    // Calculate starting position for bottom text (leave 80 pixels at bottom)
    int readingY = height() - 70;
    int lineHeight = 18;

    // Level
    QString levelText = QString("Level: %1%").arg(m_level, 0, 'f', 1);
    painter.drawText(5, readingY, width() - 10, lineHeight,
                    Qt::AlignLeft | Qt::AlignVCenter, levelText);

    // Temperature
    QString tempText = QString("Temp: %1%2C").arg(m_temperature, 0, 'f', 1)
                      .arg(QString::fromUtf8("\xC2\xB0"));
    painter.drawText(5, readingY + lineHeight, width() - 10, lineHeight,
                    Qt::AlignLeft | Qt::AlignVCenter, tempText);

    // Pressure
    QString pressureText = QString("Press: %1 bar").arg(m_pressure, 0, 'f', 2);
    painter.drawText(5, readingY + lineHeight * 2, width() - 10, lineHeight,
                    Qt::AlignLeft | Qt::AlignVCenter, pressureText);

    // Volume (at top)
    double currentVolume = m_capacity * (m_level / 100.0);
    font.setBold(true);
    font.setPointSize(9);
    painter.setFont(font);
    QString volumeText = QString("%1 / %2 %3")
                        .arg(currentVolume, 0, 'f', 0)
                        .arg(m_capacity, 0, 'f', 0)
                        .arg(m_capacityUnit);
    painter.drawText(0, 2, width(), 18, Qt::AlignCenter, volumeText);
}

