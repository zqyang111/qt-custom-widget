#ifndef BARGRAPHWIDGET_H
#define BARGRAPHWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QString>
#include <QColor>

/**
 * @brief Bar graph widget
 * 
 * This widget displays multiple data values as vertical bars,
 * suitable for comparing multiple parameters or values.
 */
class BarGraphWidget : public QWidget
{
    Q_OBJECT

public:
    struct BarData {
        QString label;
        double value;
        QColor color;
    };

    explicit BarGraphWidget(QWidget *parent = nullptr);
    ~BarGraphWidget() override;

    // Data management
    void addBar(const QString &label, double value, const QColor &color);
    void setBarValue(int index, double value);
    void setBarValue(const QString &label, double value);
    void clearBars();

    // Setters
    void setMaxValue(double maxValue);
    void setAutoScale(bool enabled);
    void setShowLabels(bool show);
    void setShowValues(bool show);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(400, 250); }
    QSize minimumSizeHint() const override { return QSize(200, 150); }

private:
    void drawBackground(QPainter &painter);
    void drawAxes(QPainter &painter);
    void drawBars(QPainter &painter);

    QVector<BarData> m_bars;
    double m_maxValue;
    bool m_autoScale;
    bool m_showLabels;
    bool m_showValues;

    QColor m_backgroundColor;
    QColor m_axisColor;
};

#endif // BARGRAPHWIDGET_H












