#ifndef PENDULUMWIDGET_H
#define PENDULUMWIDGET_H

#include <QWidget>
#include <QTimer>

/**
 * @brief Pendulum Widget - Industrial pendulum oscillation animation
 * This widget displays a pendulum with realistic swinging motion.
 * Shows angle, period, and amplitude.
 */
class PendulumWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PendulumWidget(QWidget *parent = nullptr);
    ~PendulumWidget();

    // Setters
    void setAmplitude(double amplitude);  // degrees
    void setPeriod(double period);  // seconds
    void setSwinging(bool swinging);
    void setDamping(double damping);  // 0.0 to 1.0

    // Getters
    double amplitude() const { return m_amplitude; }
    double period() const { return m_period; }
    bool isSwinging() const { return m_swinging; }
    double damping() const { return m_damping; }

    QSize sizeHint() const override { return QSize(300, 400); }
    QSize minimumSizeHint() const override { return QSize(200, 300); }

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onAnimationTimer();

private:
    void drawBackground(QPainter &painter);
    void drawSupport(QPainter &painter, int x, int y);
    void drawPendulum(QPainter &painter, int pivotX, int pivotY, int length, double angle);
    void drawBob(QPainter &painter, int x, int y, int radius);
    void drawAngleArc(QPainter &painter, int pivotX, int pivotY, int length, double angle);
    void drawParameters(QPainter &painter);

    double m_amplitude;
    double m_period;
    bool m_swinging;
    double m_damping;

    QTimer *m_animationTimer;
    double m_currentAngle;  // Current angle in degrees
    double m_angularVelocity;  // Angular velocity
    double m_time;  // Elapsed time
};

#endif // PENDULUMWIDGET_H












