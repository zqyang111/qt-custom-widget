#include "AlarmIndicatorWidget.h"
#include <QMouseEvent>

AlarmIndicatorWidget::AlarmIndicatorWidget(QWidget *parent)
    : QWidget(parent)
    , m_flashTimer(new QTimer(this))
    , m_flashState(false)
    , m_scrollOffset(0)
{
    setMinimumSize(200, 100);
    
    connect(m_flashTimer, &QTimer::timeout, this, &AlarmIndicatorWidget::onFlashTimer);
    m_flashTimer->start(500);  // Flash every 500ms
}

AlarmIndicatorWidget::~AlarmIndicatorWidget()
{
}

void AlarmIndicatorWidget::addAlarm(const QString &message, AlarmLevel level)
{
    // Check if alarm already exists
    for (auto &alarm : m_alarms) {
        if (alarm.message == message) {
            alarm.active = true;
            alarm.level = level;
            update();
            return;
        }
    }

    // Add new alarm
    AlarmItem item;
    item.message = message;
    item.level = level;
    item.active = true;
    item.acknowledged = false;
    m_alarms.prepend(item);  // Add to top
    update();
}

void AlarmIndicatorWidget::clearAlarm(const QString &message)
{
    for (int i = 0; i < m_alarms.size(); ++i) {
        if (m_alarms[i].message == message) {
            m_alarms[i].active = false;
            update();
            return;
        }
    }
}

void AlarmIndicatorWidget::acknowledgeAlarm(int index)
{
    if (index >= 0 && index < m_alarms.size()) {
        m_alarms[index].acknowledged = true;
        update();
    }
}

void AlarmIndicatorWidget::clearAllAlarms()
{
    m_alarms.clear();
    update();
}

int AlarmIndicatorWidget::activeAlarmCount() const
{
    int count = 0;
    for (const auto &alarm : m_alarms) {
        if (alarm.active && !alarm.acknowledged)
            count++;
    }
    return count;
}

void AlarmIndicatorWidget::onFlashTimer()
{
    m_flashState = !m_flashState;
    update();
}

void AlarmIndicatorWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawSummary(painter);
    drawAlarmList(painter);
}

void AlarmIndicatorWidget::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(30, 30, 30));
    
    // Draw border
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 5, 5);
}

void AlarmIndicatorWidget::drawSummary(QPainter &painter)
{
    QRect summaryRect(5, 5, width() - 10, 30);
    
    // Draw summary background
    painter.setBrush(QColor(50, 50, 50));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(summaryRect, 3, 3);
    
    // Count alarms by level
    int criticalCount = 0, warningCount = 0, infoCount = 0;
    for (const auto &alarm : m_alarms) {
        if (alarm.active && !alarm.acknowledged) {
            switch (alarm.level) {
            case Critical: criticalCount++; break;
            case Warning: warningCount++; break;
            case Info: infoCount++; break;
            default: break;
            }
        }
    }
    
    // Draw summary text
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(10);
    painter.setFont(font);
    
    QString summaryText = QString("Alarms: ");
    painter.drawText(summaryRect.adjusted(5, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, summaryText);
    
    int xOffset = 70;
    
    // Critical
    if (criticalCount > 0) {
        QRect critRect(xOffset, summaryRect.y() + 7, 50, 16);
        painter.setBrush(getLevelColor(Critical));
        painter.drawRoundedRect(critRect, 2, 2);
        painter.setPen(Qt::white);
        painter.drawText(critRect, Qt::AlignCenter, QString::number(criticalCount));
        xOffset += 55;
    }
    
    // Warning
    if (warningCount > 0) {
        QRect warnRect(xOffset, summaryRect.y() + 7, 50, 16);
        painter.setBrush(getLevelColor(Warning));
        painter.drawRoundedRect(warnRect, 2, 2);
        painter.setPen(Qt::black);
        painter.drawText(warnRect, Qt::AlignCenter, QString::number(warningCount));
        xOffset += 55;
    }
    
    // Info
    if (infoCount > 0) {
        QRect infoRect(xOffset, summaryRect.y() + 7, 50, 16);
        painter.setBrush(getLevelColor(Info));
        painter.drawRoundedRect(infoRect, 2, 2);
        painter.setPen(Qt::white);
        painter.drawText(infoRect, Qt::AlignCenter, QString::number(infoCount));
    }
}

void AlarmIndicatorWidget::drawAlarmList(QPainter &painter)
{
    QRect listRect(5, 40, width() - 10, height() - 45);
    int itemHeight = 30;
    int y = listRect.y();
    
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);
    
    int visibleItems = qMin(m_alarms.size(), (listRect.height() / itemHeight));
    
    for (int i = 0; i < visibleItems; ++i) {
        const AlarmItem &alarm = m_alarms[i];
        
        QRect itemRect(listRect.x(), y, listRect.width(), itemHeight - 2);
        
        // Determine background color
        QColor bgColor;
        if (alarm.acknowledged) {
            bgColor = QColor(60, 60, 60);
        } else if (alarm.active) {
            // Flash for unacknowledged alarms
            if (m_flashState && alarm.level >= Warning) {
                bgColor = getLevelColor(alarm.level);
            } else {
                bgColor = getLevelColor(alarm.level).darker(200);
            }
        } else {
            bgColor = QColor(40, 40, 40);
        }
        
        painter.setBrush(bgColor);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(itemRect, 3, 3);
        
        // Draw level indicator
        QRect levelRect(itemRect.x() + 3, itemRect.y() + 3, 60, itemRect.height() - 6);
        painter.setBrush(getLevelColor(alarm.level));
        painter.drawRoundedRect(levelRect, 2, 2);
        
        painter.setPen(alarm.level == Warning ? Qt::black : Qt::white);
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(levelRect, Qt::AlignCenter, getLevelText(alarm.level));
        
        // Draw message
        painter.setPen(Qt::white);
        font.setBold(false);
        painter.setFont(font);
        
        QString displayText = alarm.message;
        QFontMetrics fm(font);
        int maxWidth = itemRect.width() - 75;
        displayText = fm.elidedText(displayText, Qt::ElideRight, maxWidth);
        
        painter.drawText(itemRect.adjusted(70, 0, -5, 0), 
                        Qt::AlignLeft | Qt::AlignVCenter, displayText);
        
        y += itemHeight;
    }
    
    // Draw "No Alarms" if empty
    if (m_alarms.isEmpty()) {
        painter.setPen(QColor(120, 120, 120));
        font.setPointSize(12);
        font.setBold(false);
        painter.setFont(font);
        painter.drawText(listRect, Qt::AlignCenter, "No Active Alarms");
    }
}

void AlarmIndicatorWidget::mousePressEvent(QMouseEvent *event)
{
    // Handle alarm acknowledgement on click
    if (event->button() == Qt::LeftButton) {
        int itemHeight = 30;
        int listY = 40;
        int clickY = event->pos().y() - listY;
        
        if (clickY >= 0) {
            int index = clickY / itemHeight;
            if (index >= 0 && index < m_alarms.size()) {
                acknowledgeAlarm(index);
            }
        }
    }
    
    QWidget::mousePressEvent(event);
}

QColor AlarmIndicatorWidget::getLevelColor(AlarmLevel level) const
{
    switch (level) {
    case Critical: return QColor(255, 0, 0);
    case Warning: return QColor(255, 200, 0);
    case Info: return QColor(0, 150, 255);
    default: return QColor(100, 100, 100);
    }
}

QString AlarmIndicatorWidget::getLevelText(AlarmLevel level) const
{
    switch (level) {
    case Critical: return "CRITICAL";
    case Warning: return "WARNING";
    case Info: return "INFO";
    default: return "NONE";
    }
}







