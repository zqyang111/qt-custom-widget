#ifndef LEDWIDGET_H
#define LEDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

/**
 * @brief Industrial style LED indicator widget
 * 
 * This widget displays an LED indicator with various colors and states,
 * including steady, flashing, and off states.
 */
class LEDWidget : public QWidget
{
    Q_OBJECT

public:
    enum LEDColor {
        Red,
        Green,
        Blue,
        Yellow,
        Orange,
        White
    };

    enum LEDState {
        Off,
        On,
        Flashing
    };

    explicit LEDWidget(QWidget *parent = nullptr);
    ~LEDWidget() override;

    // Getters
    LEDColor color() const { return m_color; }
    LEDState state() const { return m_state; }
    bool isOn() const { return m_isOn; }

    // Setters
    void setColor(LEDColor color);
    void setState(LEDState state);
    void setFlashInterval(int milliseconds);
    void turnOn();
    void turnOff();
    void toggle();

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(40, 40); }
    QSize minimumSizeHint() const override { return QSize(20, 20); }

private slots:
    void onFlashTimer();

private:
    void drawLED(QPainter &painter);
    QColor getLEDColor() const;

    LEDColor m_color;
    LEDState m_state;
    bool m_isOn;
    QTimer *m_flashTimer;
    int m_flashInterval;
};

#endif // LEDWIDGET_H


