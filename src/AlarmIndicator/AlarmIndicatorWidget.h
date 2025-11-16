#ifndef ALARMINDICATORWIDGET_H
#define ALARMINDICATORWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QVector>

/**
 * @brief Alarm indicator widget
 * 
 * This widget displays multiple alarm conditions with different priorities,
 * suitable for monitoring system alarms and warnings.
 */
class AlarmIndicatorWidget : public QWidget
{
    Q_OBJECT

public:
    enum AlarmLevel {
        NoAlarm,
        Info,
        Warning,
        Critical
    };

    struct AlarmItem {
        QString message;
        AlarmLevel level;
        bool active;
        bool acknowledged;
    };

    explicit AlarmIndicatorWidget(QWidget *parent = nullptr);
    ~AlarmIndicatorWidget() override;

    // Alarm management
    void addAlarm(const QString &message, AlarmLevel level);
    void clearAlarm(const QString &message);
    void acknowledgeAlarm(int index);
    void clearAllAlarms();
    int activeAlarmCount() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QSize sizeHint() const override { return QSize(300, 200); }
    QSize minimumSizeHint() const override { return QSize(200, 100); }

private slots:
    void onFlashTimer();

private:
    void drawBackground(QPainter &painter);
    void drawAlarmList(QPainter &painter);
    void drawSummary(QPainter &painter);
    QColor getLevelColor(AlarmLevel level) const;
    QString getLevelText(AlarmLevel level) const;

    QVector<AlarmItem> m_alarms;
    QTimer *m_flashTimer;
    bool m_flashState;
    int m_scrollOffset;
};

#endif // ALARMINDICATORWIDGET_H












