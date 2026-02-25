#ifndef TANKWIDGET_H
#define TANKWIDGET_H

#include <QWidget>
#include <QPainter>

/**
 * @brief Industrial tank widget
 * 
 * This widget displays a storage tank with liquid level, temperature,
 * and pressure indicators, suitable for process monitoring.
 */
class TankWidget : public QWidget
{
    Q_OBJECT

public:
    enum TankShape {
        Cylindrical,
        Rectangular,
        Spherical
    };

    explicit TankWidget(QWidget *parent = nullptr);
    ~TankWidget() override;

    // Getters
    double level() const { return m_level; }
    double temperature() const { return m_temperature; }
    double pressure() const { return m_pressure; }
    TankShape shape() const { return m_shape; }

    // Setters
    void setLevel(double percentage);  // 0-100%
    void setTemperature(double temp);
    void setPressure(double pressure);
    void setShape(TankShape shape);
    void setCapacity(double capacity, const QString &unit);
    void setLiquidColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(140, 280); }
    QSize minimumSizeHint() const override { return QSize(100, 200); }

private:
    void drawTankBody(QPainter &painter);
    void drawLiquid(QPainter &painter);
    void drawScale(QPainter &painter);
    void drawReadings(QPainter &painter);

    double m_level;
    double m_temperature;
    double m_pressure;
    double m_capacity;
    QString m_capacityUnit;
    TankShape m_shape;

    QColor m_liquidColor;
    QColor m_tankColor;
};

#endif // TANKWIDGET_H

