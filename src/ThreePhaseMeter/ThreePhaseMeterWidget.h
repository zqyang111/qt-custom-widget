#ifndef THREEPHASEMETERWIDGET_H
#define THREEPHASEMETERWIDGET_H

#include <QWidget>
#include <QPainter>

/**
 * @brief Three-phase power meter widget
 * 
 * This widget displays three-phase voltage, current, power, and phase angle.
 */
class ThreePhaseMeterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ThreePhaseMeterWidget(QWidget *parent = nullptr);
    ~ThreePhaseMeterWidget() override;

    // Getters
    double voltageA() const { return m_voltageA; }
    double voltageB() const { return m_voltageB; }
    double voltageC() const { return m_voltageC; }
    double currentA() const { return m_currentA; }
    double currentB() const { return m_currentB; }
    double currentC() const { return m_currentC; }
    double frequency() const { return m_frequency; }
    double powerFactor() const { return m_powerFactor; }

    // Setters
    void setVoltages(double va, double vb, double vc);
    void setCurrents(double ia, double ib, double ic);
    void setFrequency(double freq);
    void setPowerFactor(double pf);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(300, 220); }
    QSize minimumSizeHint() const override { return QSize(250, 180); }

private:
    void drawBackground(QPainter &painter);
    void drawPhaseVectors(QPainter &painter);
    void drawParameters(QPainter &painter);
    void drawPowerInfo(QPainter &painter);

    double m_voltageA, m_voltageB, m_voltageC;
    double m_currentA, m_currentB, m_currentC;
    double m_frequency;
    double m_powerFactor;

    QColor m_phaseAColor;
    QColor m_phaseBColor;
    QColor m_phaseCColor;
};

#endif // THREEPHASEMETERWIDGET_H












