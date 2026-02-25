#ifndef BELTDRIVEWIDGET_H
#define BELTDRIVEWIDGET_H

#include <QWidget>
#include <QTimer>

/**
 * @brief Simple Belt Drive Widget with rotation animation
 */
class BeltDriveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BeltDriveWidget(QWidget *parent = nullptr);
    ~BeltDriveWidget();

    void setRunning(bool running);
    void setRPM(double rpm);
    void setBeltSpeed(double speed);
    
    bool isRunning() const { return m_running; }
    double rpm() const { return m_rpm; }

    QSize sizeHint() const override { return QSize(500, 300); }

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateAnimation();

private:
    void drawPulley(QPainter &painter, int x, int y, int radius, double angle, const QColor &color);
    void drawBelt(QPainter &painter);

    bool m_running;
    double m_rpm;
    double m_beltSpeed;
    double m_angle;
    double m_beltOffset;
    QTimer *m_timer;
};

#endif // BELTDRIVEWIDGET_H











