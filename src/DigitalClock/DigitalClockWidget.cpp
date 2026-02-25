#include "DigitalClockWidget.h"
#include <QDateTime>

DigitalClockWidget::DigitalClockWidget(QWidget *parent)
    : QWidget(parent)
    , m_time(QTime::currentTime())
    , m_date(QDate::currentDate())
    , m_timer(new QTimer(this))
    , m_running(true)
    , m_showDate(true)
    , m_showSeconds(true)
    , m_backgroundColor(QColor(40, 40, 40))
    , m_displayColor(QColor(20, 20, 20))
    , m_textColor(QColor(0, 255, 100))
{
    setMinimumSize(180, 70);
    
    connect(m_timer, &QTimer::timeout, this, &DigitalClockWidget::updateClock);
    m_timer->start(1000);
}

DigitalClockWidget::~DigitalClockWidget()
{
}

void DigitalClockWidget::setDateTime(const QDateTime &dateTime)
{
    m_time = dateTime.time();
    m_date = dateTime.date();
    update();
}

void DigitalClockWidget::setShowDate(bool show)
{
    m_showDate = show;
    updateGeometry();
    update();
}

void DigitalClockWidget::setShowSeconds(bool show)
{
    m_showSeconds = show;
    update();
}

void DigitalClockWidget::start()
{
    if (!m_running) {
        m_running = true;
        m_timer->start(1000);
        m_time = QTime::currentTime();
        m_date = QDate::currentDate();
    }
}

void DigitalClockWidget::stop()
{
    if (m_running) {
        m_running = false;
        m_timer->stop();
    }
}

void DigitalClockWidget::updateClock()
{
    if (m_running) {
        m_time = QTime::currentTime();
        m_date = QDate::currentDate();
        update();
    }
}

void DigitalClockWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawTime(painter);
    if (m_showDate) {
        drawDate(painter);
    }
}

void DigitalClockWidget::drawBackground(QPainter &painter)
{
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.setBrush(m_backgroundColor);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);

    // Draw display area
    int timeY = m_showDate ? height() / 2 - 15 : height() / 2 - 20;
    int timeHeight = 40;
    QRect timeRect(10, timeY, width() - 20, timeHeight);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 150));
    painter.drawRoundedRect(timeRect.adjusted(2, 2, 0, 0), 3, 3);
    
    painter.setBrush(m_displayColor);
    painter.drawRoundedRect(timeRect, 3, 3);
}

void DigitalClockWidget::drawTime(QPainter &painter)
{
    int timeY = m_showDate ? height() / 2 - 15 : height() / 2 - 20;
    int timeHeight = 40;
    QRect timeRect(10, timeY, width() - 20, timeHeight);

    painter.setPen(m_textColor);
    QFont font("Courier New");
    font.setPointSize(20);
    font.setBold(true);
    painter.setFont(font);

    QString timeText;
    if (m_showSeconds) {
        timeText = m_time.toString("hh:mm:ss");
    } else {
        timeText = m_time.toString("hh:mm");
    }

    painter.drawText(timeRect, Qt::AlignCenter, timeText);
}

void DigitalClockWidget::drawDate(QPainter &painter)
{
    painter.setPen(QColor(180, 180, 180));
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(false);
    painter.setFont(font);

    QString dateText = m_date.toString("yyyy-MM-dd");
    painter.drawText(QRectF(10, height() - 25, width() - 20, 20),
                    Qt::AlignCenter, dateText);
}












