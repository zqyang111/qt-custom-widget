#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QWidget>
#include <QTimer>

/**
 * @brief Cam Mechanism Widget - Industrial cam and follower mechanism animation
 * This widget displays a cam mechanism that converts rotary motion to oscillating motion.
 * Shows cam rotation, follower displacement, and contact point.
 */
class CamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CamWidget(QWidget *parent = nullptr);
    ~CamWidget();

    // Setters
    void setRPM(double rpm);
    void setCamProfile(int profile);  // 0=circular, 1=heart, 2=eccentric
    void setRunning(bool running);

    // Getters
    double rpm() const { return m_rpm; }
    int camProfile() const { return m_camProfile; }
    bool isRunning() const { return m_running; }

    QSize sizeHint() const override { return QSize(400, 450); }
    QSize minimumSizeHint() const override { return QSize(300, 350); }

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onAnimationTimer();

private:
    void drawBackground(QPainter &painter);
    void drawCam(QPainter &painter, int centerX, int centerY, int radius, double angle);
    void drawFollower(QPainter &painter, int x, int y, double displacement);
    void drawFrame(QPainter &painter, int x, int y);
    void drawParameters(QPainter &painter);
    double calculateFollowerDisplacement(double angle);

    double m_rpm;
    int m_camProfile;
    bool m_running;

    QTimer *m_animationTimer;
    double m_camAngle;  // Cam rotation angle in radians
};

#endif // CAMWIDGET_H



