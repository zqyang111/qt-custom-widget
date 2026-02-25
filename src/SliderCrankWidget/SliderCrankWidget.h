#ifndef SLIDERCRANKWIDGET_H
#define SLIDERCRANKWIDGET_H

#include <QWidget>
#include <QTimer>

/**
 * @brief Slider-Crank Mechanism Widget - Industrial slider-crank mechanism animation
 * This widget displays a slider-crank mechanism that converts rotary motion to linear motion.
 * Shows crank angle, slider position, and mechanical advantage.
 */
class SliderCrankWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SliderCrankWidget(QWidget *parent = nullptr);
    ~SliderCrankWidget();

    // Setters
    void setRPM(double rpm);
    void setCrankLength(double length);  // mm
    void setConnectingRodLength(double length);  // mm
    void setRunning(bool running);

    // Getters
    double rpm() const { return m_rpm; }
    double crankLength() const { return m_crankLength; }
    double connectingRodLength() const { return m_connectingRodLength; }
    bool isRunning() const { return m_running; }

    QSize sizeHint() const override { return QSize(450, 350); }
    QSize minimumSizeHint() const override { return QSize(350, 250); }

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onAnimationTimer();

private:
    void drawBackground(QPainter &painter);
    void drawFrame(QPainter &painter, int x, int y, int width);
    void drawCrank(QPainter &painter, int centerX, int centerY, int length, double angle);
    void drawConnectingRod(QPainter &painter, int x1, int y1, int x2, int y2);
    void drawSlider(QPainter &painter, int x, int y, int width, int height);
    void drawParameters(QPainter &painter, double actualRodLength);

    double m_rpm;
    double m_crankLength;
    double m_connectingRodLength;
    bool m_running;

    QTimer *m_animationTimer;
    double m_crankAngle;  // Crank rotation angle in radians
};

#endif // SLIDERCRANKWIDGET_H

