#ifndef KNOBWIDGET_H
#define KNOBWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

/**
 * @brief Industrial style rotary knob widget
 * 
 * This widget displays a rotary knob control similar to a potentiometer,
 * suitable for precise value adjustment.
 */
class KnobWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KnobWidget(QWidget *parent = nullptr);
    ~KnobWidget() override;

    // Getters
    double value() const { return m_value; }
    double minimum() const { return m_minimum; }
    double maximum() const { return m_maximum; }

    // Setters
    void setValue(double value);
    void setRange(double minimum, double maximum);
    void setStep(double step);

signals:
    void valueChanged(double value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    QSize sizeHint() const override { return QSize(250, 250); }
    QSize minimumSizeHint() const override { return QSize(150, 150); }

private:
    void drawBackground(QPainter &painter);
    void drawKnob(QPainter &painter);
    void drawIndicator(QPainter &painter);
    void drawValue(QPainter &painter);
    double angleFromValue() const;
    void updateValueFromMouse(const QPoint &pos);

    double m_value;
    double m_minimum;
    double m_maximum;
    double m_step;

    bool m_pressed;
    QPoint m_lastPos;

    static constexpr double MIN_ANGLE = -135.0;  // degrees
    static constexpr double MAX_ANGLE = 135.0;   // degrees
};

#endif // KNOBWIDGET_H

