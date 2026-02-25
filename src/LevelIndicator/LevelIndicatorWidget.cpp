#include "LevelIndicatorWidget.h"
#include <QtMath>

LevelIndicatorWidget::LevelIndicatorWidget(QWidget *parent)
    : QWidget(parent)
    , m_level(50.0)
    , m_minLevel(0.0)
    , m_maxLevel(100.0)
    , m_warningLevel(20.0)
    , m_criticalLevel(10.0)
    , m_unit("%")
    , m_liquidColor(QColor(0, 150, 255))
    , m_warningColor(QColor(255, 165, 0))
    , m_criticalColor(QColor(255, 0, 0))
{
    setMinimumSize(100, 150);
}

LevelIndicatorWidget::~LevelIndicatorWidget()
{
}

void LevelIndicatorWidget::setLevel(double level)
{
    if (level < m_minLevel)
        level = m_minLevel;
    if (level > m_maxLevel)
        level = m_maxLevel;

    if (qAbs(m_level - level) > 0.001) {
        m_level = level;
        update();
    }
}

void LevelIndicatorWidget::setRange(double minLevel, double maxLevel)
{
    if (minLevel < maxLevel) {
        m_minLevel = minLevel;
        m_maxLevel = maxLevel;
        setLevel(m_level);
    }
}

void LevelIndicatorWidget::setUnit(const QString &unit)
{
    m_unit = unit;
    update();
}

void LevelIndicatorWidget::setWarningRange(double warningLevel, double criticalLevel)
{
    m_warningLevel = warningLevel;
    m_criticalLevel = criticalLevel;
    update();
}

void LevelIndicatorWidget::setLiquidColor(const QColor &color)
{
    m_liquidColor = color;
    update();
}

void LevelIndicatorWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawTank(painter);
    drawLiquid(painter);
    drawScale(painter);
    drawValue(painter);
}

void LevelIndicatorWidget::drawTank(QPainter &painter)
{
    int tankWidth = qMin(static_cast<int>(width() * 0.35), 50);  // Limit tank width to leave space for labels
    int tankHeight = height() - 60;
    int tankX = 15;  // Fixed left margin
    int tankY = 30;

    // Draw outer tank
    QLinearGradient gradient(tankX, 0, tankX + tankWidth, 0);
    gradient.setColorAt(0.0, QColor(180, 180, 180));
    gradient.setColorAt(0.5, QColor(220, 220, 220));
    gradient.setColorAt(1.0, QColor(180, 180, 180));

    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(100, 100, 100), 3));
    painter.drawRoundedRect(tankX, tankY, tankWidth, tankHeight, 5, 5);

    // Draw inner tank (transparent area)
    painter.setBrush(QColor(50, 50, 50, 200));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(tankX + 8, tankY + 8, tankWidth - 16, tankHeight - 16, 3, 3);
}

void LevelIndicatorWidget::drawLiquid(QPainter &painter)
{
    int tankWidth = qMin(static_cast<int>(width() * 0.35), 50);
    int tankHeight = height() - 60;
    int tankX = 15;
    int tankY = 30;

    double percentage = (m_level - m_minLevel) / (m_maxLevel - m_minLevel);
    int liquidHeight = static_cast<int>((tankHeight - 16) * percentage);
    int liquidY = tankY + tankHeight - 8 - liquidHeight;

    if (liquidHeight > 0) {
        // Choose color based on level
        QColor currentColor;
        if (m_level <= m_criticalLevel)
            currentColor = m_criticalColor;
        else if (m_level <= m_warningLevel)
            currentColor = m_warningColor;
        else
            currentColor = m_liquidColor;

        // Create liquid gradient
        QLinearGradient liquidGradient(tankX + 8, liquidY, tankX + 8, liquidY + liquidHeight);
        liquidGradient.setColorAt(0.0, currentColor.lighter(130));
        liquidGradient.setColorAt(0.5, currentColor);
        liquidGradient.setColorAt(1.0, currentColor.darker(110));

        painter.setBrush(liquidGradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(tankX + 8, liquidY, tankWidth - 16, liquidHeight, 3, 3);

        // Add wave effect on top
        painter.setPen(QPen(currentColor.lighter(150), 2));
        int waveY = liquidY;
        painter.drawLine(tankX + 8, waveY, tankX + tankWidth - 8, waveY);
    }
}

void LevelIndicatorWidget::drawScale(QPainter &painter)
{
    int tankWidth = qMin(static_cast<int>(width() * 0.35), 50);
    int tankHeight = height() - 60;
    int tankX = 15;
    int tankY = 30;
    int tankRight = tankX + tankWidth;

    painter.setPen(QPen(Qt::white, 1));
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    int numMarks = 10;
    for (int i = 0; i <= numMarks; ++i) {
        int y = tankY + tankHeight - 8 - (i * (tankHeight - 16) / numMarks);
        double levelValue = m_minLevel + (m_maxLevel - m_minLevel) * i / numMarks;

        // Draw scale marks
        if (i % 2 == 0) {
            painter.drawLine(tankRight + 5, y, tankRight + 15, y);
            
            // Draw scale numbers with more space
            QString text = QString::number(static_cast<int>(levelValue));
            int textX = tankRight + 18;
            int textWidth = width() - textX - 5;  // Use remaining width
            painter.drawText(textX, y - 8, textWidth, 16,
                           Qt::AlignLeft | Qt::AlignVCenter, text);
        } else {
            painter.drawLine(tankRight + 5, y, tankRight + 10, y);
        }
    }
}

void LevelIndicatorWidget::drawValue(QPainter &painter)
{
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(12);
    font.setBold(true);
    painter.setFont(font);

    QString valueText = QString::number(m_level, 'f', 1);
    if (!m_unit.isEmpty())
        valueText += " " + m_unit;

    painter.drawText(QRectF(0, 5, width(), 20), Qt::AlignCenter, valueText);
}

