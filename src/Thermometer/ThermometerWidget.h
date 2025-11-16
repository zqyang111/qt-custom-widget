#ifndef THERMOMETERWIDGET_H
#define THERMOMETERWIDGET_H

#include <QWidget>
#include <QPainter>

/**
 * @brief Industrial style thermometer widget
 * 
 * This widget displays a vertical thermometer with customizable temperature
 * range and visual appearance suitable for industrial monitoring.
 */
class ThermometerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ThermometerWidget(QWidget *parent = nullptr);
    ~ThermometerWidget() override;

    // Getters
    double temperature() const { return m_temperature; }
    double minTemperature() const { return m_minTemp; }
    double maxTemperature() const { return m_maxTemp; }

    // Setters
    void setTemperature(double temp);
    void setRange(double minTemp, double maxTemp);
    void setWarningRange(double warningTemp, double criticalTemp);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(100, 300); }
    QSize minimumSizeHint() const override { return QSize(60, 150); }

private:
    void drawBackground(QPainter &painter);
    void drawTube(QPainter &painter);
    void drawMercury(QPainter &painter);
    void drawBulb(QPainter &painter);
    void drawScale(QPainter &painter);
    void drawValue(QPainter &painter);

    double m_temperature;
    double m_minTemp;
    double m_maxTemp;
    double m_warningTemp;
    double m_criticalTemp;

    QColor m_normalColor;
    QColor m_warningColor;
    QColor m_criticalColor;
};

#endif // THERMOMETERWIDGET_H


