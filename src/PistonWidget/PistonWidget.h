#ifndef PISTONWIDGET_H
#define PISTONWIDGET_H

#include <QWidget>
#include <QTimer>

/**
 * @brief Piston Widget - Industrial piston reciprocating motion animation
 * This widget displays a piston with crankshaft mechanism showing reciprocating motion.
 * Shows RPM, stroke, and pressure.
 */
class PistonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PistonWidget(QWidget *parent = nullptr);
    ~PistonWidget();

    // Setters
    void setRPM(double rpm);
    void setStroke(double stroke);  // mm
    void setPressure(double pressure);  // bar
    void setRunning(bool running);

    // Getters
    double rpm() const { return m_rpm; }
    double stroke() const { return m_stroke; }
    double pressure() const { return m_pressure; }
    bool isRunning() const { return m_running; }

    QSize sizeHint() const override { return QSize(350, 400); }
    QSize minimumSizeHint() const override { return QSize(250, 300); }

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onAnimationTimer();

private:
    void drawBackground(QPainter &painter);
    void drawCylinder(QPainter &painter, int x, int y, int width, int height);
    void drawPiston(QPainter &painter, int x, int y, int width, int height);
    void drawConnectingRod(QPainter &painter, int x1, int y1, int x2, int y2);
    void drawCrankshaft(QPainter &painter, int centerX, int centerY, int radius, double angle);
    void drawSpark(QPainter &painter, int x, int y);
    void drawIntakeFlow(QPainter &painter, int x, int y, double phase);
    void drawExhaustFlow(QPainter &painter, int x, int y, double phase);
    void drawParameters(QPainter &painter);

    double m_rpm;
    double m_stroke;
    double m_pressure;
    bool m_running;

    QTimer *m_animationTimer;
    double m_crankAngle;  // Crankshaft rotation angle
};

#endif // PISTONWIDGET_H


