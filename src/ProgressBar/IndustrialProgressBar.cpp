#include "IndustrialProgressBar.h"
#include <QtMath>

IndustrialProgressBar::IndustrialProgressBar(QWidget *parent)
    : QWidget(parent)
    , m_value(0)
    , m_minimum(0)
    , m_maximum(100)
    , m_orientation(Horizontal)
    , m_showPercentage(true)
    , m_backgroundColor(QColor(50, 50, 50))
    , m_progressColor(QColor(0, 180, 0))
    , m_borderColor(QColor(120, 120, 120))
{
    setMinimumSize(200, 30);
}

IndustrialProgressBar::~IndustrialProgressBar()
{
}

void IndustrialProgressBar::setValue(int value)
{
    if (value < m_minimum)
        value = m_minimum;
    if (value > m_maximum)
        value = m_maximum;

    if (m_value != value) {
        m_value = value;
        update();
    }
}

void IndustrialProgressBar::setRange(int minimum, int maximum)
{
    if (minimum < maximum) {
        m_minimum = minimum;
        m_maximum = maximum;
        setValue(m_value); // Revalidate current value
    }
}

void IndustrialProgressBar::setOrientation(Orientation orientation)
{
    if (m_orientation != orientation) {
        m_orientation = orientation;
        updateGeometry();
        update();
    }
}

void IndustrialProgressBar::setShowPercentage(bool show)
{
    if (m_showPercentage != show) {
        m_showPercentage = show;
        update();
    }
}

QSize IndustrialProgressBar::sizeHint() const
{
    if (m_orientation == Horizontal)
        return QSize(200, 30);
    else
        return QSize(30, 200);
}

QSize IndustrialProgressBar::minimumSizeHint() const
{
    if (m_orientation == Horizontal)
        return QSize(100, 20);
    else
        return QSize(20, 100);
}

void IndustrialProgressBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawProgress(painter);
    drawText(painter);
}

void IndustrialProgressBar::drawBackground(QPainter &painter)
{
    // Draw outer border
    painter.setPen(QPen(m_borderColor, 2));
    painter.setBrush(m_backgroundColor);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);

    // Draw inner shadow effect
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 50));
    painter.drawRoundedRect(rect().adjusted(3, 3, -3, -3), 3, 3);
}

void IndustrialProgressBar::drawProgress(QPainter &painter)
{
    if (m_maximum == m_minimum)
        return;

    double percentage = static_cast<double>(m_value - m_minimum) / 
                       (m_maximum - m_minimum);

    QRectF progressRect;
    if (m_orientation == Horizontal) {
        int progressWidth = static_cast<int>((width() - 8) * percentage);
        progressRect = QRectF(4, 4, progressWidth, height() - 8);
    } else {
        int progressHeight = static_cast<int>((height() - 8) * percentage);
        progressRect = QRectF(4, height() - 4 - progressHeight, 
                            width() - 8, progressHeight);
    }

    // Create gradient for progress bar
    QLinearGradient gradient;
    if (m_orientation == Horizontal) {
        gradient = QLinearGradient(progressRect.topLeft(), progressRect.topRight());
    } else {
        gradient = QLinearGradient(progressRect.bottomLeft(), progressRect.topLeft());
    }

    QColor lightColor = m_progressColor.lighter(120);
    QColor darkColor = m_progressColor.darker(110);

    gradient.setColorAt(0.0, lightColor);
    gradient.setColorAt(0.5, m_progressColor);
    gradient.setColorAt(1.0, darkColor);

    painter.setPen(Qt::NoPen);
    painter.setBrush(gradient);
    painter.drawRoundedRect(progressRect, 3, 3);

    // Add highlight effect
    QLinearGradient highlight;
    if (m_orientation == Horizontal) {
        highlight = QLinearGradient(progressRect.topLeft(), progressRect.bottomLeft());
    } else {
        highlight = QLinearGradient(progressRect.topLeft(), progressRect.topRight());
    }
    highlight.setColorAt(0.0, QColor(255, 255, 255, 80));
    highlight.setColorAt(0.5, QColor(255, 255, 255, 20));
    highlight.setColorAt(1.0, QColor(255, 255, 255, 0));

    painter.setBrush(highlight);
    QRectF highlightRect = progressRect;
    if (m_orientation == Horizontal)
        highlightRect.setHeight(highlightRect.height() / 2);
    else
        highlightRect.setWidth(highlightRect.width() / 2);
    
    painter.drawRoundedRect(highlightRect, 3, 3);
}

void IndustrialProgressBar::drawText(QPainter &painter)
{
    if (!m_showPercentage)
        return;

    if (m_maximum == m_minimum)
        return;

    int percentage = static_cast<int>(
        100.0 * (m_value - m_minimum) / (m_maximum - m_minimum));

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(10);
    painter.setFont(font);

    QString text = QString::number(percentage) + "%";
    painter.drawText(rect(), Qt::AlignCenter, text);
}












