#include "SwitchButtonWidget.h"

SwitchButtonWidget::SwitchButtonWidget(QWidget *parent)
    : QWidget(parent)
    , m_checked(false)
    , m_enabled(true)
    , m_pressed(false)
    , m_onColor(QColor(0, 200, 0))
    , m_offColor(QColor(150, 150, 150))
    , m_disabledColor(QColor(80, 80, 80))
{
    setMinimumSize(60, 30);
    setCursor(Qt::PointingHandCursor);
}

SwitchButtonWidget::~SwitchButtonWidget()
{
}

void SwitchButtonWidget::setChecked(bool checked)
{
    if (m_checked != checked) {
        m_checked = checked;
        update();
        emit toggled(m_checked);
    }
}

void SwitchButtonWidget::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        setCursor(m_enabled ? Qt::PointingHandCursor : Qt::ForbiddenCursor);
        update();
    }
}

void SwitchButtonWidget::toggle()
{
    setChecked(!m_checked);
}

void SwitchButtonWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawHandle(painter);
}

void SwitchButtonWidget::drawBackground(QPainter &painter)
{
    int h = height();
    int w = width();
    int radius = h / 2;

    // Choose color based on state
    QColor bgColor;
    if (!m_enabled)
        bgColor = m_disabledColor;
    else if (m_checked)
        bgColor = m_onColor;
    else
        bgColor = m_offColor;

    // Draw background track
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
    painter.drawRoundedRect(0, 0, w, h, radius, radius);

    // Add inner shadow
    painter.setBrush(QColor(0, 0, 0, 50));
    painter.drawRoundedRect(2, 2, w - 4, h - 4, radius - 2, radius - 2);

    // Draw ON/OFF text
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(8);
    font.setBold(true);
    painter.setFont(font);

    if (m_checked) {
        painter.drawText(QRect(5, 0, w / 2, h), Qt::AlignCenter, "ON");
    } else {
        painter.drawText(QRect(w / 2, 0, w / 2 - 5, h), Qt::AlignCenter, "OFF");
    }
}

void SwitchButtonWidget::drawHandle(QPainter &painter)
{
    int h = height();
    int w = width();
    int handleSize = h - 8;
    int handleX;

    if (m_checked)
        handleX = w - handleSize - 4;
    else
        handleX = 4;

    // Draw shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 100));
    painter.drawEllipse(handleX + 2, 6, handleSize, handleSize);

    // Draw handle
    QRadialGradient gradient(handleX + handleSize / 2, 4 + handleSize / 2, handleSize / 2);
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.7, QColor(240, 240, 240));
    gradient.setColorAt(1.0, QColor(200, 200, 200));

    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(150, 150, 150), 1));
    painter.drawEllipse(handleX, 4, handleSize, handleSize);
}

void SwitchButtonWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_enabled && event->button() == Qt::LeftButton) {
        m_pressed = true;
        update();
    }
    QWidget::mousePressEvent(event);
}

void SwitchButtonWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_enabled && m_pressed && event->button() == Qt::LeftButton) {
        m_pressed = false;
        toggle();
        emit clicked();
    }
    QWidget::mouseReleaseEvent(event);
}












