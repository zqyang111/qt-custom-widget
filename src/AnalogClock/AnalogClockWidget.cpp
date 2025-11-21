#include "AnalogClockWidget.h"
#include <QtMath>
#include <QDateTime>

AnalogClockWidget::AnalogClockWidget(QWidget *parent)
    : QWidget(parent)
    , m_time(QTime::currentTime())
    , m_timer(new QTimer(this))
    , m_running(true)
    , m_faceColor(QColor(240, 240, 240))
    , m_hourHandColor(QColor(40, 40, 40))
    , m_minuteHandColor(QColor(60, 60, 60))
    , m_secondHandColor(QColor(255, 0, 0))
{
    setMinimumSize(120, 120);
    
    connect(m_timer, &QTimer::timeout, this, &AnalogClockWidget::updateClock);
    m_timer->start(1000);  // Update every second
}

AnalogClockWidget::~AnalogClockWidget()
{
}

void AnalogClockWidget::setTime(const QTime &time)
{
    m_time = time;
    update();
}

void AnalogClockWidget::start()
{
    if (!m_running) {
        m_running = true;
        m_timer->start(1000);
        m_time = QTime::currentTime();
    }
}

void AnalogClockWidget::stop()
{
    if (m_running) {
        m_running = false;
        m_timer->stop();
    }
}

void AnalogClockWidget::reset()
{
    m_time = QTime(0, 0, 0);
    update();
}

void AnalogClockWidget::updateClock()
{
    if (m_running) {
        m_time = QTime::currentTime();
        update();
    }
}

void AnalogClockWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawTicks(painter);
    drawHands(painter);
    drawCenterDot(painter);
}

void AnalogClockWidget::drawBackground(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    // Draw outer ring
    painter.setPen(QPen(QColor(100, 100, 100), 3));
    painter.setBrush(m_faceColor);
    painter.drawEllipse(-95, -95, 190, 190);

    // Draw inner shadow
    QRadialGradient gradient(0, -20, 100);
    gradient.setColorAt(0.0, QColor(255, 255, 255, 0));
    gradient.setColorAt(0.8, QColor(0, 0, 0, 20));
    gradient.setColorAt(1.0, QColor(0, 0, 0, 40));
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(-90, -90, 180, 180);

    painter.restore();
}

void AnalogClockWidget::drawTicks(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    painter.setPen(QPen(QColor(80, 80, 80), 2));

    // Draw hour marks
    for (int i = 0; i < 12; ++i) {
        painter.save();
        painter.rotate(30.0 * i);  // 360 / 12 = 30 degrees
        
        if (i % 3 == 0) {
            // Major marks at 12, 3, 6, 9
            painter.setPen(QPen(QColor(60, 60, 60), 4));
            painter.drawLine(0, -85, 0, -70);
            
            // Draw numbers
            painter.save();
            painter.translate(0, -60);
            painter.rotate(-30.0 * i);
            
            QFont font = painter.font();
            font.setPointSize(14);
            font.setBold(true);
            painter.setFont(font);
            painter.setPen(QColor(40, 40, 40));
            
            int hour = (i == 0) ? 12 : i;
            painter.drawText(QRectF(-15, -10, 30, 20), Qt::AlignCenter, 
                           QString::number(hour));
            painter.restore();
        } else {
            // Regular hour marks
            painter.setPen(QPen(QColor(100, 100, 100), 2));
            painter.drawLine(0, -85, 0, -75);
        }
        
        painter.restore();
    }

    painter.restore();
}

void AnalogClockWidget::drawHands(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    int hour = m_time.hour() % 12;
    int minute = m_time.minute();
    int second = m_time.second();

    // Hour hand
    painter.save();
    painter.rotate(30.0 * hour + 0.5 * minute);  // 30 deg/hour + 0.5 deg/minute
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_hourHandColor);
    QPolygon hourHand;
    hourHand << QPoint(-4, 8) << QPoint(4, 8) << QPoint(2, -50) << QPoint(-2, -50);
    painter.drawPolygon(hourHand);
    painter.restore();

    // Minute hand
    painter.save();
    painter.rotate(6.0 * minute + 0.1 * second);  // 6 deg/minute + 0.1 deg/second
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_minuteHandColor);
    QPolygon minuteHand;
    minuteHand << QPoint(-3, 8) << QPoint(3, 8) << QPoint(2, -70) << QPoint(-2, -70);
    painter.drawPolygon(minuteHand);
    painter.restore();

    // Second hand
    painter.save();
    painter.rotate(6.0 * second);  // 6 deg/second
    painter.setPen(QPen(m_secondHandColor, 2));
    painter.drawLine(0, 10, 0, -80);
    painter.restore();

    painter.restore();
}

void AnalogClockWidget::drawCenterDot(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    // Draw center dot
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.setBrush(m_secondHandColor);
    painter.drawEllipse(-6, -6, 12, 12);

    painter.restore();
}












