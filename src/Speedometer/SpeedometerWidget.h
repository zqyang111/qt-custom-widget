#ifndef SPEEDOMETERWIDGET_H
#define SPEEDOMETERWIDGET_H

#include <QWidget>
#include <QPainter>

/**
 * @brief Speedometer widget (semi-circular gauge)
 * 
 * This widget displays a half-circle speedometer, suitable for displaying
 * speed, RPM, or other rate measurements.
 */
class SpeedometerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpeedometerWidget(QWidget *parent = nullptr);
    ~SpeedometerWidget() override;

    // Getters
    double value() const { return m_value; }
    double minValue() const { return m_minValue; }
    double maxValue() const { return m_maxValue; }
    QString unit() const { return m_unit; }

    // Setters
    void setValue(double value);
    void setRange(double minValue, double maxValue);
    void setUnit(const QString &unit);
    void setWarningZone(double warningStart);
    void setRedlineZone(double redlineStart);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(320, 240); }
    QSize minimumSizeHint() const override { return QSize(240, 180); }

private:
    void drawBackground(QPainter &painter);
    void drawArc(QPainter &painter);
    void drawScale(QPainter &painter);
    void drawNeedle(QPainter &painter);
    void drawCenterCap(QPainter &painter);
    void drawValue(QPainter &painter);

    double m_value;
    double m_minValue;
    double m_maxValue;
    double m_warningStart;
    double m_redlineStart;
    QString m_unit;

    QColor m_normalColor;
    QColor m_warningColor;
    QColor m_redlineColor;
};

#endif // SPEEDOMETERWIDGET_H

