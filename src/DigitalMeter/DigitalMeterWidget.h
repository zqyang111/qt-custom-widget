#ifndef DIGITALMETERWIDGET_H
#define DIGITALMETERWIDGET_H

#include <QWidget>
#include <QPainter>

/**
 * @brief Industrial style digital meter widget
 * 
 * This widget displays a digital readout with LCD-style digits,
 * suitable for displaying precise numerical values.
 */
class DigitalMeterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DigitalMeterWidget(QWidget *parent = nullptr);
    ~DigitalMeterWidget() override;

    // Getters
    double value() const { return m_value; }
    int precision() const { return m_precision; }
    QString unit() const { return m_unit; }
    QString label() const { return m_label; }

    // Setters
    void setValue(double value);
    void setPrecision(int precision);
    void setUnit(const QString &unit);
    void setLabel(const QString &label);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(200, 80); }
    QSize minimumSizeHint() const override { return QSize(120, 50); }

private:
    void drawBackground(QPainter &painter);
    void drawLabel(QPainter &painter);
    void drawValue(QPainter &painter);
    void drawUnit(QPainter &painter);

    double m_value;
    int m_precision;
    QString m_unit;
    QString m_label;

    QColor m_backgroundColor;
    QColor m_displayColor;
    QColor m_textColor;
};

#endif // DIGITALMETERWIDGET_H


