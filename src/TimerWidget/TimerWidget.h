#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

/**
 * @brief Timer/Stopwatch widget
 * 
 * This widget displays a countdown timer or stopwatch for process timing.
 */
class TimerWidget : public QWidget
{
    Q_OBJECT

public:
    enum Mode {
        Stopwatch,   // Count up from 0
        Countdown    // Count down to 0
    };

    explicit TimerWidget(QWidget *parent = nullptr);
    ~TimerWidget() override;

    // Getters
    int elapsedSeconds() const { return m_elapsedSeconds; }
    int targetSeconds() const { return m_targetSeconds; }
    Mode mode() const { return m_mode; }
    bool isRunning() const { return m_running; }

    // Setters
    void setMode(Mode mode);
    void setTargetSeconds(int seconds);  // For countdown mode
    void start();
    void stop();
    void reset();

signals:
    void timeout();  // Emitted when countdown reaches 0
    void tick(int seconds);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(220, 120); }
    QSize minimumSizeHint() const override { return QSize(150, 80); }

private slots:
    void onTick();

private:
    void drawBackground(QPainter &painter);
    void drawTime(QPainter &painter);
    void drawProgress(QPainter &painter);
    void drawControls(QPainter &painter);
    QString formatTime(int totalSeconds) const;

    Mode m_mode;
    int m_elapsedSeconds;
    int m_targetSeconds;
    bool m_running;

    QTimer *m_timer;

    QColor m_backgroundColor;
    QColor m_displayColor;
    QColor m_textColor;
    QColor m_progressColor;
};

#endif // TIMERWIDGET_H












