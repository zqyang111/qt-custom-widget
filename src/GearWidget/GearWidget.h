#ifndef GEARWIDGET_H
#define GEARWIDGET_H

#include <QWidget>
#include <QTimer>

/**
 * @brief Simple Gear Widget with rotation animation
 */
class GearWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GearWidget(QWidget *parent = nullptr);
    ~GearWidget();

    void setRunning(bool running);
    void setRPM(double rpm);
    void setTorque(double torque);
    
    bool isRunning() const { return m_running; }
    double rpm() const { return m_rpm; }

    QSize sizeHint() const override { return QSize(400, 300); }

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateAnimation();

private:
    void drawGear(QPainter &painter, int x, int y, int radius, int teeth, double angle, const QColor &color);

    bool m_running;
    double m_rpm;
    double m_torque;
    double m_angle;
    QTimer *m_timer;
};

#endif // GEARWIDGET_H

