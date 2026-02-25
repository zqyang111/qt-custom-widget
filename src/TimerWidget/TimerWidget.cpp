#include "TimerWidget.h"

TimerWidget::TimerWidget(QWidget *parent)
    : QWidget(parent)
    , m_mode(Stopwatch)
    , m_elapsedSeconds(0)
    , m_targetSeconds(60)
    , m_running(false)
    , m_timer(new QTimer(this))
    , m_backgroundColor(QColor(40, 40, 40))
    , m_displayColor(QColor(20, 20, 20))
    , m_textColor(QColor(0, 255, 100))
    , m_progressColor(QColor(0, 180, 255))
{
    setMinimumSize(150, 80);
    
    connect(m_timer, &QTimer::timeout, this, &TimerWidget::onTick);
}

TimerWidget::~TimerWidget()
{
}

void TimerWidget::setMode(Mode mode)
{
    if (m_mode != mode) {
        m_mode = mode;
        reset();
    }
}

void TimerWidget::setTargetSeconds(int seconds)
{
    if (seconds > 0) {
        m_targetSeconds = seconds;
        update();
    }
}

void TimerWidget::start()
{
    if (!m_running) {
        m_running = true;
        m_timer->start(1000);  // 1 second interval
    }
}

void TimerWidget::stop()
{
    if (m_running) {
        m_running = false;
        m_timer->stop();
    }
}

void TimerWidget::reset()
{
    m_running = false;
    m_timer->stop();
    m_elapsedSeconds = 0;
    update();
}

void TimerWidget::onTick()
{
    if (m_mode == Stopwatch) {
        m_elapsedSeconds++;
    } else {  // Countdown
        if (m_elapsedSeconds < m_targetSeconds) {
            m_elapsedSeconds++;
        } else {
            m_running = false;
            m_timer->stop();
            emit timeout();
        }
    }
    
    emit tick(m_elapsedSeconds);
    update();
}

void TimerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawTime(painter);
    if (m_mode == Countdown) {
        drawProgress(painter);
    }
    drawControls(painter);
    // Note: drawDate is not used in TimerWidget
}

void TimerWidget::drawBackground(QPainter &painter)
{
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.setBrush(m_backgroundColor);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);
}

void TimerWidget::drawTime(QPainter &painter)
{
    int timeY = 25;
    int timeHeight = 45;
    QRect timeRect(10, timeY, width() - 20, timeHeight);

    // Draw display background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 150));
    painter.drawRoundedRect(timeRect.adjusted(2, 2, 0, 0), 3, 3);
    
    painter.setBrush(m_displayColor);
    painter.drawRoundedRect(timeRect, 3, 3);

    // Draw time text
    painter.setPen(m_textColor);
    QFont font("Courier New");
    font.setPointSize(22);
    font.setBold(true);
    painter.setFont(font);

    QString timeText;
    if (m_mode == Stopwatch) {
        timeText = formatTime(m_elapsedSeconds);
    } else {
        int remaining = m_targetSeconds - m_elapsedSeconds;
        if (remaining < 0) remaining = 0;
        timeText = formatTime(remaining);
    }

    painter.drawText(timeRect, Qt::AlignCenter, timeText);
}

void TimerWidget::drawProgress(QPainter &painter)
{
    if (m_targetSeconds == 0) return;

    int progressY = 75;
    int progressHeight = 12;
    QRect progressRect(15, progressY, width() - 30, progressHeight);

    // Draw progress background
    painter.setBrush(QColor(60, 60, 60));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(progressRect, 6, 6);

    // Draw progress bar
    double percentage = static_cast<double>(m_elapsedSeconds) / m_targetSeconds;
    if (percentage > 1.0) percentage = 1.0;
    
    int filledWidth = static_cast<int>(progressRect.width() * percentage);

    QLinearGradient gradient(progressRect.topLeft(), progressRect.topRight());
    gradient.setColorAt(0.0, m_progressColor);
    gradient.setColorAt(1.0, m_progressColor.lighter(120));

    painter.setBrush(gradient);
    painter.drawRoundedRect(progressRect.x(), progressRect.y(),
                          filledWidth, progressRect.height(), 6, 6);
}

void TimerWidget::drawControls(QPainter &painter)
{
    // Draw mode indicator
    painter.setPen(QColor(180, 180, 180));
    QFont font = painter.font();
    font.setPointSize(8);
    font.setBold(true);
    painter.setFont(font);

    QString modeText = (m_mode == Stopwatch) ? "STOPWATCH" : "COUNTDOWN";
    painter.drawText(QRectF(10, 5, width() - 20, 18), Qt::AlignCenter, modeText);

    // Draw running indicator
    if (m_running) {
        painter.setBrush(QColor(0, 255, 0));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(width() - 20, 8, 10, 10);
    }
}

QString TimerWidget::formatTime(int totalSeconds) const
{
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    if (hours > 0) {
        return QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    } else {
        return QString("%1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    }
}

