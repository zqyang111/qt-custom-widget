#ifndef INDUSTRIALSLIDERWIDGET_H
#define INDUSTRIALSLIDERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

/**
 * @brief Industrial style slider widget
 * 
 * This widget displays a vertical or horizontal slider with scale marks,
 * suitable for precise value adjustment in industrial applications.
 */
class IndustrialSliderWidget : public QWidget
{
    Q_OBJECT

public:
    enum Orientation {
        Horizontal,
        Vertical
    };

    explicit IndustrialSliderWidget(QWidget *parent = nullptr);
    ~IndustrialSliderWidget() override;

    // Getters
    double value() const { return m_value; }
    double minimum() const { return m_minimum; }
    double maximum() const { return m_maximum; }
    Orientation orientation() const { return m_orientation; }

    // Setters
    void setValue(double value);
    void setRange(double minimum, double maximum);
    void setOrientation(Orientation orientation);
    void setTickInterval(double interval);

signals:
    void valueChanged(double value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override { return QSize(100, 200); }

private:
    void drawTrack(QPainter &painter);
    void drawTicks(QPainter &painter);
    void drawHandle(QPainter &painter);
    void updateValueFromMouse(const QPoint &pos);

    double m_value;
    double m_minimum;
    double m_maximum;
    double m_tickInterval;
    Orientation m_orientation;
    bool m_pressed;

    QColor m_trackColor;
    QColor m_handleColor;
    QColor m_tickColor;
};

#endif // INDUSTRIALSLIDERWIDGET_H

