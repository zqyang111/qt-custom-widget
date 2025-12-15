#ifndef BATTERYINDICATORWIDGET_H
#define BATTERYINDICATORWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

/**
 * @brief Battery indicator widget
 * 
 * This widget displays battery charge level, voltage, current, and charging status.
 */
class BatteryIndicatorWidget : public QWidget
{
    Q_OBJECT

public:
    enum ChargingState {
        Discharging,
        Charging,
        FullyCharged,
        Fault
    };

    explicit BatteryIndicatorWidget(QWidget *parent = nullptr);
    ~BatteryIndicatorWidget() override;

    // Getters
    double chargeLevel() const { return m_chargeLevel; }
    double voltage() const { return m_voltage; }
    double current() const { return m_current; }
    ChargingState chargingState() const { return m_chargingState; }

    // Setters
    void setChargeLevel(double percentage);  // 0-100%
    void setVoltage(double voltage);
    void setCurrent(double current);
    void setChargingState(ChargingState state);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(200, 150); }
    QSize minimumSizeHint() const override { return QSize(150, 100); }

private slots:
    void onFlashTimer();

private:
    void drawBackground(QPainter &painter);
    void drawBattery(QPainter &painter);
    void drawChargeLevel(QPainter &painter);
    void drawParameters(QPainter &painter);
    void drawChargingIndicator(QPainter &painter);

    double m_chargeLevel;
    double m_voltage;
    double m_current;
    ChargingState m_chargingState;

    QTimer *m_flashTimer;
    bool m_flashState;

    QColor m_fullColor;
    QColor m_mediumColor;
    QColor m_lowColor;
    QColor m_criticalColor;
};

#endif // BATTERYINDICATORWIDGET_H












