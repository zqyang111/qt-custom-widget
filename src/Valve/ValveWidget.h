#ifndef VALVEWIDGET_H
#define VALVEWIDGET_H

#include <QWidget>
#include <QPainter>

/**
 * @brief Industrial valve widget
 * 
 * This widget displays a valve with open/closed states and opening percentage,
 * suitable for process control visualization.
 */
class ValveWidget : public QWidget
{
    Q_OBJECT

public:
    enum ValveType {
        GateValve,
        BallValve,
        ButterflyValve
    };

    enum ValveState {
        Closed,
        Opening,
        Open,
        Closing
    };

    explicit ValveWidget(QWidget *parent = nullptr);
    ~ValveWidget() override;

    // Getters
    double opening() const { return m_opening; }
    ValveType type() const { return m_type; }
    ValveState state() const { return m_state; }

    // Setters
    void setOpening(double percentage);  // 0-100%
    void setType(ValveType type);
    void setState(ValveState state);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(80, 100); }
    QSize minimumSizeHint() const override { return QSize(60, 80); }

private:
    void drawValveBody(QPainter &painter);
    void drawValveIndicator(QPainter &painter);
    void drawPipes(QPainter &painter);
    void drawStatus(QPainter &painter);

    double m_opening;
    ValveType m_type;
    ValveState m_state;

    QColor m_openColor;
    QColor m_closedColor;
    QColor m_bodyColor;
};

#endif // VALVEWIDGET_H












