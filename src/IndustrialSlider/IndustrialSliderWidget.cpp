#include "IndustrialSliderWidget.h"
#include <QtMath>

IndustrialSliderWidget::IndustrialSliderWidget(QWidget *parent)
    : QWidget(parent)
    , m_value(0.0)
    , m_minimum(0.0)
    , m_maximum(100.0)
    , m_tickInterval(10.0)
    , m_orientation(Vertical)
    , m_pressed(false)
    , m_trackColor(QColor(60, 60, 60))
    , m_handleColor(QColor(0, 180, 0))
    , m_tickColor(QColor(180, 180, 180))
{
    setMinimumSize(100, 200);
    setCursor(Qt::PointingHandCursor);
}

IndustrialSliderWidget::~IndustrialSliderWidget()
{
}

void IndustrialSliderWidget::setValue(double value)
{
    if (value < m_minimum)
        value = m_minimum;
    if (value > m_maximum)
        value = m_maximum;

    if (qAbs(m_value - value) > 0.001) {
        m_value = value;
        update();
        emit valueChanged(m_value);
    }
}

void IndustrialSliderWidget::setRange(double minimum, double maximum)
{
    if (minimum < maximum) {
        m_minimum = minimum;
        m_maximum = maximum;
        setValue(m_value);
    }
}

void IndustrialSliderWidget::setOrientation(Orientation orientation)
{
    if (m_orientation != orientation) {
        m_orientation = orientation;
        updateGeometry();
        update();
    }
}

void IndustrialSliderWidget::setTickInterval(double interval)
{
    if (interval > 0.0) {
        m_tickInterval = interval;
        update();
    }
}

QSize IndustrialSliderWidget::sizeHint() const
{
    if (m_orientation == Vertical)
        return QSize(120, 300);
    else
        return QSize(300, 100);
}

void IndustrialSliderWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(40, 40, 40));

    drawTrack(painter);
    drawTicks(painter);
    drawHandle(painter);
}

void IndustrialSliderWidget::drawTrack(QPainter &painter)
{
    if (m_orientation == Vertical) {
        int trackWidth = 20;
        int trackHeight = height() - 40;
        int trackX = 55;  // Fixed position to leave space for labels on left
        int trackY = 20;

        // Draw track groove
        painter.setBrush(m_trackColor);
        painter.setPen(QPen(QColor(40, 40, 40), 2));
        painter.drawRoundedRect(trackX, trackY, trackWidth, trackHeight, 10, 10);

        // Draw filled portion
        double percentage = (m_value - m_minimum) / (m_maximum - m_minimum);
        int filledHeight = static_cast<int>(trackHeight * percentage);
        int filledY = trackY + trackHeight - filledHeight;

        QLinearGradient gradient(trackX, filledY, trackX, trackY + trackHeight);
        gradient.setColorAt(0.0, m_handleColor.lighter(120));
        gradient.setColorAt(1.0, m_handleColor);

        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(trackX + 2, filledY, trackWidth - 4, filledHeight - 2, 8, 8);

    } else {  // Horizontal
        int trackWidth = width() - 40;
        int trackHeight = 20;
        int trackX = 20;
        int trackY = (height() - trackHeight) / 2;

        // Draw track groove
        painter.setBrush(m_trackColor);
        painter.setPen(QPen(QColor(40, 40, 40), 2));
        painter.drawRoundedRect(trackX, trackY, trackWidth, trackHeight, 10, 10);

        // Draw filled portion
        double percentage = (m_value - m_minimum) / (m_maximum - m_minimum);
        int filledWidth = static_cast<int>(trackWidth * percentage);

        QLinearGradient gradient(trackX, trackY, trackX + filledWidth, trackY);
        gradient.setColorAt(0.0, m_handleColor);
        gradient.setColorAt(1.0, m_handleColor.lighter(120));

        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(trackX + 2, trackY + 2, filledWidth - 2, trackHeight - 4, 8, 8);
    }
}

void IndustrialSliderWidget::drawTicks(QPainter &painter)
{
    painter.setPen(QPen(m_tickColor, 2));
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    double range = m_maximum - m_minimum;
    int numTicks = static_cast<int>(range / m_tickInterval) + 1;

    if (m_orientation == Vertical) {
        int trackHeight = height() - 40;
        int trackY = 20;
        int trackX = 55;  // Match track position

        for (int i = 0; i < numTicks; ++i) {
            double tickValue = m_minimum + i * m_tickInterval;
            if (tickValue > m_maximum) break;

            double percentage = (tickValue - m_minimum) / range;
            int y = trackY + trackHeight - static_cast<int>(trackHeight * percentage);

            // Draw tick mark
            painter.drawLine(trackX - 10, y, trackX - 2, y);

            // Draw tick label with sufficient space
            QString label = QString::number(static_cast<int>(tickValue));
            painter.drawText(5, y - 8, trackX - 15, 16, 
                           Qt::AlignRight | Qt::AlignVCenter, label);
        }

    } else {  // Horizontal
        int trackWidth = width() - 40;
        int trackX = 20;
        int trackY = height() / 2;

        for (int i = 0; i < numTicks; ++i) {
            double tickValue = m_minimum + i * m_tickInterval;
            if (tickValue > m_maximum) break;

            double percentage = (tickValue - m_minimum) / range;
            int x = trackX + static_cast<int>(trackWidth * percentage);

            // Draw tick mark
            painter.drawLine(x, trackY + 15, x, trackY + 25);

            // Draw tick label
            QString label = QString::number(static_cast<int>(tickValue));
            painter.drawText(x - 15, trackY + 28, 30, 16, Qt::AlignCenter, label);
        }
    }
}

void IndustrialSliderWidget::drawHandle(QPainter &painter)
{
    double percentage = (m_value - m_minimum) / (m_maximum - m_minimum);

    if (m_orientation == Vertical) {
        int trackHeight = height() - 40;
        int trackY = 20;
        int handleY = trackY + trackHeight - static_cast<int>(trackHeight * percentage);
        int handleX = 55 + 10;  // Match track position + half track width

        // Draw handle shadow
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 100));
        painter.drawEllipse(handleX - 13, handleY - 13, 26, 26);

        // Draw handle
        QRadialGradient gradient(handleX, handleY - 5, 15);
        gradient.setColorAt(0.0, m_handleColor.lighter(150));
        gradient.setColorAt(0.7, m_handleColor);
        gradient.setColorAt(1.0, m_handleColor.darker(120));

        painter.setBrush(gradient);
        painter.setPen(QPen(m_handleColor.darker(140), 2));
        painter.drawEllipse(handleX - 15, handleY - 15, 30, 30);

        // Draw value text on the right side
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setPointSize(10);
        font.setBold(true);
        painter.setFont(font);
        QString valueText = QString::number(m_value, 'f', 1);
        int textWidth = 60;
        int trackX = 55;  // Match the track position
        painter.drawText(trackX + 20, handleY - 8, textWidth, 16, 
                        Qt::AlignLeft | Qt::AlignVCenter, valueText);

    } else {  // Horizontal
        int trackWidth = width() - 40;
        int trackX = 20;
        int handleX = trackX + static_cast<int>(trackWidth * percentage);
        int handleY = height() / 2;

        // Draw handle shadow
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 100));
        painter.drawEllipse(handleX - 13, handleY - 13, 26, 26);

        // Draw handle
        QRadialGradient gradient(handleX, handleY - 5, 15);
        gradient.setColorAt(0.0, m_handleColor.lighter(150));
        gradient.setColorAt(0.7, m_handleColor);
        gradient.setColorAt(1.0, m_handleColor.darker(120));

        painter.setBrush(gradient);
        painter.setPen(QPen(m_handleColor.darker(140), 2));
        painter.drawEllipse(handleX - 15, handleY - 15, 30, 30);

        // Draw value text
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setPointSize(10);
        font.setBold(true);
        painter.setFont(font);
        QString valueText = QString::number(m_value, 'f', 1);
        painter.drawText(handleX - 25, handleY - 30, 50, 16, Qt::AlignCenter, valueText);
    }
}

void IndustrialSliderWidget::updateValueFromMouse(const QPoint &pos)
{
    double percentage;

    if (m_orientation == Vertical) {
        int trackHeight = height() - 40;
        int trackY = 20;
        int y = pos.y() - trackY;
        percentage = 1.0 - static_cast<double>(y) / trackHeight;
    } else {
        int trackWidth = width() - 40;
        int trackX = 20;
        int x = pos.x() - trackX;
        percentage = static_cast<double>(x) / trackWidth;
    }

    // Clamp percentage to valid range
    if (percentage < 0.0) percentage = 0.0;
    if (percentage > 1.0) percentage = 1.0;
    
    double newValue = m_minimum + percentage * (m_maximum - m_minimum);
    setValue(newValue);
}

void IndustrialSliderWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
        updateValueFromMouse(event->pos());
    }
    QWidget::mousePressEvent(event);
}

void IndustrialSliderWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pressed) {
        updateValueFromMouse(event->pos());
    }
    QWidget::mouseMoveEvent(event);
}

void IndustrialSliderWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = false;
    }
    QWidget::mouseReleaseEvent(event);
}

