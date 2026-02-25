#ifndef COMPASSWIDGET_H
#define COMPASSWIDGET_H

#include <QWidget>
#include <QPainter>

/**
 * @brief Compass widget
 * 
 * This widget displays a compass showing direction in degrees (0-360).
 * Suitable for equipment orientation, wind direction, or angular position.
 */
class CompassWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompassWidget(QWidget *parent = nullptr);
    ~CompassWidget() override;

    // Getters
    double angle() const { return m_angle; }

    // Setters
    void setAngle(double angle);  // 0-360 degrees, 0=North

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(200, 200); }
    QSize minimumSizeHint() const override { return QSize(120, 120); }

private:
    void drawBackground(QPainter &painter);
    void drawCardinalPoints(QPainter &painter);
    void drawDegreeMarks(QPainter &painter);
    void drawNeedle(QPainter &painter);
    void drawAngleDisplay(QPainter &painter);

    double m_angle;

    QColor m_faceColor;
    QColor m_northColor;
    QColor m_needleColor;
};

#endif // COMPASSWIDGET_H












