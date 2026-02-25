#ifndef PUMPWIDGET_H
#define PUMPWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

/**
 * @brief Industrial pump widget
 * 
 * This widget displays a pump with running/stopped states and rotation animation,
 * suitable for process control visualization.
 */
class PumpWidget : public QWidget
{
    Q_OBJECT

public:
    enum PumpState {
        Stopped,
        Starting,
        Running,
        Stopping,
        Fault
    };

    explicit PumpWidget(QWidget *parent = nullptr);
    ~PumpWidget() override;

    // Getters
    PumpState state() const { return m_state; }
    double speed() const { return m_speed; }  // RPM or percentage
    bool isRunning() const { return m_state == Running; }

    // Setters
    void setState(PumpState state);
    void setSpeed(double speed);
    void start();
    void stop();

signals:
    void stateChanged(PumpState state);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(100, 100); }
    QSize minimumSizeHint() const override { return QSize(60, 60); }

private slots:
    void onAnimationTimer();

private:
    void drawPumpBody(QPainter &painter);
    void drawImpeller(QPainter &painter);
    void drawPipes(QPainter &painter);
    void drawStatus(QPainter &painter);

    PumpState m_state;
    double m_speed;
    double m_rotationAngle;

    QTimer *m_animationTimer;

    QColor m_runningColor;
    QColor m_stoppedColor;
    QColor m_faultColor;
};

#endif // PUMPWIDGET_H












