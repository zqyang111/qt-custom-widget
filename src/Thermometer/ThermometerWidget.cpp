#include "ThermometerWidget.h"
#include <QtMath>
#include <QPainterPath>

ThermometerWidget::ThermometerWidget(QWidget *parent)
    : QWidget(parent)
    , m_temperature(20.0)
    , m_minTemp(-20.0)
    , m_maxTemp(120.0)
    , m_warningTemp(80.0)
    , m_criticalTemp(100.0)
    , m_normalColor(QColor(0, 180, 255))
    , m_warningColor(QColor(255, 165, 0))
    , m_criticalColor(QColor(255, 0, 0))
{
    setMinimumSize(60, 150);
}

ThermometerWidget::~ThermometerWidget()
{
}

void ThermometerWidget::setTemperature(double temp)
{
    if (temp < m_minTemp)
        temp = m_minTemp;
    if (temp > m_maxTemp)
        temp = m_maxTemp;

    if (qAbs(m_temperature - temp) > 0.001) {
        m_temperature = temp;
        update();
    }
}

void ThermometerWidget::setRange(double minTemp, double maxTemp)
{
    if (minTemp < maxTemp) {
        m_minTemp = minTemp;
        m_maxTemp = maxTemp;
        setTemperature(m_temperature); // Revalidate current temperature
    }
}

void ThermometerWidget::setWarningRange(double warningTemp, double criticalTemp)
{
    m_warningTemp = warningTemp;
    m_criticalTemp = criticalTemp;
    update();
}

void ThermometerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawTube(painter);
    drawScale(painter);
    drawMercury(painter);
    drawBulb(painter);
    drawValue(painter);
}

void ThermometerWidget::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(40, 40, 40));
}

void ThermometerWidget::drawTube(QPainter &painter)
{
    int tubeWidth = width() / 4;
    int tubeHeight = height() - height() / 5;
    int tubeX = width() / 2 - tubeWidth / 2;
    int tubeY = height() / 10;

    // Draw tube background
    QLinearGradient gradient(tubeX, 0, tubeX + tubeWidth, 0);
    gradient.setColorAt(0.0, QColor(200, 200, 200));
    gradient.setColorAt(0.5, QColor(240, 240, 240));
    gradient.setColorAt(1.0, QColor(200, 200, 200));

    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(120, 120, 120), 2));
    painter.drawRoundedRect(tubeX, tubeY, tubeWidth, tubeHeight, 
                           tubeWidth / 2, tubeWidth / 2);
}

void ThermometerWidget::drawMercury(QPainter &painter)
{
    int tubeWidth = width() / 4;
    int tubeHeight = height() - height() / 5;
    int tubeX = width() / 2 - tubeWidth / 2;
    int tubeY = height() / 10;
    int bulbRadius = width() / 3;

    // Calculate mercury height based on temperature
    double percentage = (m_temperature - m_minTemp) / (m_maxTemp - m_minTemp);
    int mercuryHeight = static_cast<int>(percentage * tubeHeight);
    int mercuryY = tubeY + tubeHeight - mercuryHeight;

    // Choose color based on temperature
    QColor mercuryColor;
    if (m_temperature >= m_criticalTemp)
        mercuryColor = m_criticalColor;
    else if (m_temperature >= m_warningTemp)
        mercuryColor = m_warningColor;
    else
        mercuryColor = m_normalColor;

    // Draw mercury in tube
    painter.setBrush(mercuryColor);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    int mercuryWidth = tubeWidth - 8;
    path.addRoundedRect(tubeX + 4, mercuryY, mercuryWidth, mercuryHeight + 4,
                       mercuryWidth / 2, mercuryWidth / 2);
    
    // Connect to bulb
    path.addEllipse(width() / 2 - bulbRadius / 2, 
                   height() - bulbRadius - 5,
                   bulbRadius, bulbRadius);

    painter.drawPath(path);
}

void ThermometerWidget::drawBulb(QPainter &painter)
{
    int bulbRadius = width() / 3;
    int bulbX = width() / 2 - bulbRadius / 2;
    int bulbY = height() - bulbRadius - 5;

    // Draw bulb outline
    QRadialGradient gradient(width() / 2, bulbY + bulbRadius / 2, bulbRadius / 2);
    gradient.setColorAt(0.0, QColor(220, 220, 220));
    gradient.setColorAt(0.7, QColor(180, 180, 180));
    gradient.setColorAt(1.0, QColor(140, 140, 140));

    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(120, 120, 120), 2));
    painter.drawEllipse(bulbX, bulbY, bulbRadius, bulbRadius);
}

void ThermometerWidget::drawScale(QPainter &painter)
{
    int tubeHeight = height() - height() / 5;
    int tubeY = height() / 10;
    int tubeWidth = width() / 4;
    int tubeRight = width() / 2 + tubeWidth / 2;

    painter.setPen(QPen(Qt::white, 1));
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    int numMarks = 10;
    for (int i = 0; i <= numMarks; ++i) {
        int y = tubeY + tubeHeight - (i * tubeHeight / numMarks);
        double temp = m_minTemp + (m_maxTemp - m_minTemp) * i / numMarks;

        // Draw major marks
        if (i % 2 == 0) {
            painter.drawLine(tubeRight, y, tubeRight + 10, y);
            painter.drawText(tubeRight + 12, y - 8, 40, 16, 
                           Qt::AlignLeft | Qt::AlignVCenter,
                           QString::number(static_cast<int>(temp)));
        } else {
            painter.drawLine(tubeRight, y, tubeRight + 5, y);
        }
    }
}

void ThermometerWidget::drawValue(QPainter &painter)
{
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);

    QString valueText = QString::number(m_temperature, 'f', 1) + QString::fromUtf8("\xC2\xB0") + "C";
    painter.drawText(QRectF(0, 5, width(), 20), Qt::AlignCenter, valueText);
}

