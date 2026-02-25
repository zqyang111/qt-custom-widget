#ifndef DIGITALCLOCKWIDGET_H
#define DIGITALCLOCKWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QDate>

/**
 * @brief Digital clock widget
 * 
 * This widget displays a digital clock with date and time in LCD style.
 */
class DigitalClockWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DigitalClockWidget(QWidget *parent = nullptr);
    ~DigitalClockWidget() override;

    // Getters
    QTime time() const { return m_time; }
    QDate date() const { return m_date; }
    bool isRunning() const { return m_running; }

    // Setters
    void setDateTime(const QDateTime &dateTime);
    void setShowDate(bool show);
    void setShowSeconds(bool show);
    void start();
    void stop();

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(250, 100); }
    QSize minimumSizeHint() const override { return QSize(180, 70); }

private slots:
    void updateClock();

private:
    void drawBackground(QPainter &painter);
    void drawTime(QPainter &painter);
    void drawDate(QPainter &painter);

    QTime m_time;
    QDate m_date;
    QTimer *m_timer;
    bool m_running;
    bool m_showDate;
    bool m_showSeconds;

    QColor m_backgroundColor;
    QColor m_displayColor;
    QColor m_textColor;
};

#endif // DIGITALCLOCKWIDGET_H












