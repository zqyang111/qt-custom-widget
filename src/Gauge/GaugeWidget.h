#ifndef GAUGEWIDGET_H
#define GAUGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

/**
 * @brief Industrial style gauge widget
 * 
 * This widget displays a circular gauge with customizable range,
 * value, and visual appearance suitable for industrial applications.
 */
class GaugeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GaugeWidget(QWidget *parent = nullptr);
    ~GaugeWidget() override;

    // Getters
    double value() const { return m_value; }
    double minValue() const { return m_minValue; }
    double maxValue() const { return m_maxValue; }
    QString unit() const { return m_unit; }

    // Setters
    void setValue(double value);
    void setRange(double minValue, double maxValue);
    void setUnit(const QString &unit);
    void setWarningRange(double warningStart, double criticalStart);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(300, 300); }
    QSize minimumSizeHint() const override { return QSize(150, 150); }

private:
    void drawBackground(QPainter &painter);
    void drawScale(QPainter &painter);
    void drawNeedle(QPainter &painter);
    void drawCenterCircle(QPainter &painter);
    void drawValue(QPainter &painter);

    double m_value;
    double m_minValue;
    double m_maxValue;
    double m_warningStart;
    double m_criticalStart;
    QString m_unit;

    QColor m_normalColor;
    QColor m_warningColor;
    QColor m_criticalColor;
};

#endif // GAUGEWIDGET_H


