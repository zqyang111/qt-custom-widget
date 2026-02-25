#ifndef MOTORCONTROLLERWIDGET_H
#define MOTORCONTROLLERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

/**
 * @brief Motor controller display widget
 * 
 * This widget displays motor parameters including RPM, direction, torque, and power.
 */
class MotorControllerWidget : public QWidget
{
    Q_OBJECT

public:
    enum MotorState {
        Stopped,
        Starting,
        RunningForward,
        RunningReverse,
        Stopping,
        Fault
    };

    explicit MotorControllerWidget(QWidget *parent = nullptr);
    ~MotorControllerWidget() override;

    // Getters
    MotorState state() const { return m_state; }
    double rpm() const { return m_rpm; }
    double torque() const { return m_torque; }
    double power() const { return m_power; }

    // Setters
    void setState(MotorState state);
    void setRPM(double rpm);
    void setTorque(double torque);
    void setPower(double power);
    void setRatedRPM(double ratedRPM);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(280, 200); }
    QSize minimumSizeHint() const override { return QSize(200, 150); }

private slots:
    void onAnimationTimer();

private:
    void drawBackground(QPainter &painter);
    void drawMotorSymbol(QPainter &painter);
    void drawParameters(QPainter &painter);
    void drawStateIndicator(QPainter &painter);

    MotorState m_state;
    double m_rpm;
    double m_torque;
    double m_power;
    double m_ratedRPM;
    double m_rotationAngle;

    QTimer *m_animationTimer;

    QColor m_runningColor;
    QColor m_stoppedColor;
    QColor m_faultColor;
};

#endif // MOTORCONTROLLERWIDGET_H












