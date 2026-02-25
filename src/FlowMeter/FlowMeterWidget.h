#ifndef FLOWMETERWIDGET_H
#define FLOWMETERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

/**
 * @brief Flow meter widget
 * 
 * This widget displays instantaneous flow rate and total accumulated flow,
 * suitable for monitoring fluid flow in pipes.
 */
class FlowMeterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FlowMeterWidget(QWidget *parent = nullptr);
    ~FlowMeterWidget() override;

    // Getters
    double flowRate() const { return m_flowRate; }
    double totalFlow() const { return m_totalFlow; }
    QString unit() const { return m_unit; }

    // Setters
    void setFlowRate(double rate);  // Current flow rate
    void setTotalFlow(double total);  // Accumulated flow
    void setUnit(const QString &unit);
    void resetTotal();

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(180, 150); }
    QSize minimumSizeHint() const override { return QSize(120, 100); }

private:
    void drawBackground(QPainter &painter);
    void drawFlowRate(QPainter &painter);
    void drawTotalFlow(QPainter &painter);
    void drawFlowIndicator(QPainter &painter);

    double m_flowRate;
    double m_totalFlow;
    QString m_unit;

    QColor m_backgroundColor;
    QColor m_displayColor;
    QColor m_textColor;
};

#endif // FLOWMETERWIDGET_H












