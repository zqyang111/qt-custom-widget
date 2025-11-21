#ifndef ANALOGCLOCKWIDGET_H
#define ANALOGCLOCKWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QTime>

/**
 * @brief Analog clock widget
 * 
 * This widget displays an industrial-style analog clock with hour, minute, and second hands.
 */
class AnalogClockWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnalogClockWidget(QWidget *parent = nullptr);
    ~AnalogClockWidget() override;

    // Getters
    QTime time() const { return m_time; }
    bool isRunning() const { return m_running; }

    // Setters
    void setTime(const QTime &time);
    void start();
    void stop();
    void reset();

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(200, 200); }
    QSize minimumSizeHint() const override { return QSize(120, 120); }

private slots:
    void updateClock();

private:
    void drawBackground(QPainter &painter);
    void drawTicks(QPainter &painter);
    void drawHands(QPainter &painter);
    void drawCenterDot(QPainter &painter);

    QTime m_time;
    QTimer *m_timer;
    bool m_running;

    QColor m_faceColor;
    QColor m_hourHandColor;
    QColor m_minuteHandColor;
    QColor m_secondHandColor;
};

#endif // ANALOGCLOCKWIDGET_H












