#ifndef SPRINGWIDGET_H
#define SPRINGWIDGET_H

#include <QWidget>
#include <QTimer>

/**
 * @brief Spring Oscillation Widget - Industrial spring-mass vibration system animation
 * This widget displays a spring-mass system with realistic oscillation motion.
 * Shows displacement, frequency, amplitude, and damping coefficient.
 */
class SpringWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpringWidget(QWidget *parent = nullptr);
    ~SpringWidget();

    // Setters
    void setAmplitude(double amplitude);  // mm
    void setFrequency(double frequency);  // Hz
    void setOscillating(bool oscillating);
    void setDamping(double damping);  // 0.0 to 1.0
    void setSpringConstant(double springConstant);  // N/m
    void setMass(double mass);  // kg

    // Getters
    double amplitude() const { return m_amplitude; }
    double frequency() const { return m_frequency; }
    bool isOscillating() const { return m_oscillating; }
    double damping() const { return m_damping; }
    double springConstant() const { return m_springConstant; }
    double mass() const { return m_mass; }

    QSize sizeHint() const override { return QSize(300, 450); }
    QSize minimumSizeHint() const override { return QSize(200, 350); }

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onAnimationTimer();

private:
    void drawBackground(QPainter &painter);
    void drawFixedSupport(QPainter &painter, int x, int y);
    void drawSpring(QPainter &painter, int x, int y, int naturalLength, double compression);
    void drawMass(QPainter &painter, int x, int y, int width, int height);
    void drawDisplacementIndicator(QPainter &painter, int x, int y, double displacement);
    void drawVelocityVector(QPainter &painter, int x, int y, double velocity);
    void drawParameters(QPainter &painter);

    double m_amplitude;
    double m_frequency;
    bool m_oscillating;
    double m_damping;
    double m_springConstant;
    double m_mass;

    QTimer *m_animationTimer;
    double m_currentDisplacement;  // Current displacement in mm
    double m_velocity;  // Current velocity
    double m_time;  // Elapsed time
    int m_naturalLength;  // Natural length of spring in pixels
};

#endif // SPRINGWIDGET_H



