#include "SevenSegmentWidget.h"
#include <QtMath>
#include <QMap>

SevenSegmentWidget::SevenSegmentWidget(QWidget *parent)
    : QWidget(parent)
    , m_digitCount(4)
    , m_value("0")
    , m_segmentColor(QColor(255, 0, 0))
    , m_backgroundColor(QColor(20, 20, 20))
    , m_showDecimalPoint(false)
{
    setMinimumSize(40 * m_digitCount, 60);
}

SevenSegmentWidget::~SevenSegmentWidget()
{
}

void SevenSegmentWidget::setValue(int value)
{
    QString newValue = QString::number(value);
    if (m_value != newValue) {
        m_value = newValue;
        update();
    }
}

void SevenSegmentWidget::setValue(double value, int precision)
{
    QString newValue = QString::number(value, 'f', precision);
    if (m_value != newValue) {
        m_value = newValue;
        m_showDecimalPoint = true;
        update();
    }
}

void SevenSegmentWidget::setValue(const QString &value)
{
    if (m_value != value) {
        m_value = value;
        update();
    }
}

void SevenSegmentWidget::setDigitCount(int count)
{
    if (count > 0 && m_digitCount != count) {
        m_digitCount = count;
        updateGeometry();
        update();
    }
}

void SevenSegmentWidget::setSegmentColor(const QColor &color)
{
    if (m_segmentColor != color) {
        m_segmentColor = color;
        update();
    }
}

void SevenSegmentWidget::setShowDecimalPoint(bool show)
{
    if (m_showDecimalPoint != show) {
        m_showDecimalPoint = show;
        update();
    }
}

QSize SevenSegmentWidget::sizeHint() const
{
    return QSize(50 * m_digitCount, 80);
}

void SevenSegmentWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw background
    painter.fillRect(rect(), m_backgroundColor);

    // Calculate digit dimensions
    int digitWidth = width() / m_digitCount;
    int digitHeight = height() - 10;
    int startY = 5;

    // Pad value with leading spaces if needed
    QString displayValue = m_value.rightJustified(m_digitCount, ' ');
    
    // Draw each digit
    for (int i = 0; i < m_digitCount && i < displayValue.length(); ++i) {
        int x = i * digitWidth;
        drawDigit(painter, displayValue[i], x, startY, digitWidth, digitHeight);
    }
}

void SevenSegmentWidget::drawDigit(QPainter &painter, QChar digit, int x, int y, int w, int h)
{
    QVector<bool> segments = getSegmentPattern(digit);
    
    // Draw all 7 segments
    for (int i = 0; i < 7; ++i) {
        drawSegment(painter, i, x, y, w, h, segments[i]);
    }

    // Draw decimal point if this is the digit before the decimal point
    if (m_showDecimalPoint && digit == '.') {
        painter.setBrush(m_segmentColor);
        painter.setPen(Qt::NoPen);
        int dpSize = h / 12;
        painter.drawEllipse(x + w - w/4, y + h - dpSize - 5, dpSize, dpSize);
    }
}

void SevenSegmentWidget::drawSegment(QPainter &painter, int segment, int x, int y, int w, int h, bool lit)
{
    // Calculate segment dimensions
    int thickness = h / 15;
    int gap = thickness / 2;
    
    // Adjust for margins
    x += w / 8;
    w = w * 3 / 4;
    
    QPolygon polygon;
    
    switch (segment) {
    case 0: // Top
        polygon << QPoint(x + gap, y)
                << QPoint(x + w - gap, y)
                << QPoint(x + w - gap - thickness, y + thickness)
                << QPoint(x + gap + thickness, y + thickness);
        break;
    case 1: // Top right
        polygon << QPoint(x + w, y + gap)
                << QPoint(x + w, y + h/2 - gap)
                << QPoint(x + w - thickness, y + h/2 - gap - thickness)
                << QPoint(x + w - thickness, y + gap + thickness);
        break;
    case 2: // Bottom right
        polygon << QPoint(x + w, y + h/2 + gap)
                << QPoint(x + w, y + h - gap)
                << QPoint(x + w - thickness, y + h - gap - thickness)
                << QPoint(x + w - thickness, y + h/2 + gap + thickness);
        break;
    case 3: // Bottom
        polygon << QPoint(x + gap + thickness, y + h - thickness)
                << QPoint(x + w - gap - thickness, y + h - thickness)
                << QPoint(x + w - gap, y + h)
                << QPoint(x + gap, y + h);
        break;
    case 4: // Bottom left
        polygon << QPoint(x, y + h/2 + gap)
                << QPoint(x + thickness, y + h/2 + gap + thickness)
                << QPoint(x + thickness, y + h - gap - thickness)
                << QPoint(x, y + h - gap);
        break;
    case 5: // Top left
        polygon << QPoint(x, y + gap)
                << QPoint(x + thickness, y + gap + thickness)
                << QPoint(x + thickness, y + h/2 - gap - thickness)
                << QPoint(x, y + h/2 - gap);
        break;
    case 6: // Middle
        polygon << QPoint(x + gap, y + h/2)
                << QPoint(x + gap + thickness, y + h/2 - thickness/2)
                << QPoint(x + w - gap - thickness, y + h/2 - thickness/2)
                << QPoint(x + w - gap, y + h/2)
                << QPoint(x + w - gap - thickness, y + h/2 + thickness/2)
                << QPoint(x + gap + thickness, y + h/2 + thickness/2);
        break;
    }

    // Set color based on lit state
    if (lit) {
        painter.setBrush(m_segmentColor);
        painter.setPen(m_segmentColor.lighter(120));
    } else {
        QColor dimColor = m_segmentColor;
        dimColor.setAlpha(30);
        painter.setBrush(dimColor);
        painter.setPen(Qt::NoPen);
    }

    painter.drawPolygon(polygon);
}

QVector<bool> SevenSegmentWidget::getSegmentPattern(QChar digit)
{
    // Segments: 0=top, 1=top-right, 2=bottom-right, 3=bottom, 4=bottom-left, 5=top-left, 6=middle
    static const QMap<QChar, QVector<bool>> patterns = {
        {'0', {true,  true,  true,  true,  true,  true,  false}},
        {'1', {false, true,  true,  false, false, false, false}},
        {'2', {true,  true,  false, true,  true,  false, true}},
        {'3', {true,  true,  true,  true,  false, false, true}},
        {'4', {false, true,  true,  false, false, true,  true}},
        {'5', {true,  false, true,  true,  false, true,  true}},
        {'6', {true,  false, true,  true,  true,  true,  true}},
        {'7', {true,  true,  true,  false, false, false, false}},
        {'8', {true,  true,  true,  true,  true,  true,  true}},
        {'9', {true,  true,  true,  true,  false, true,  true}},
        {'-', {false, false, false, false, false, false, true}},
        {' ', {false, false, false, false, false, false, false}},
        {'A', {true,  true,  true,  false, true,  true,  true}},
        {'E', {true,  false, false, true,  true,  true,  true}},
        {'F', {true,  false, false, false, true,  true,  true}},
    };

    if (patterns.contains(digit.toUpper())) {
        return patterns.value(digit.toUpper());
    }
    
    // Return blank pattern for unknown characters
    return {false, false, false, false, false, false, false};
}

