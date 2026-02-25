#ifndef PIPEWIDGET_H
#define PIPEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

/**
 * @brief Pipe widget with flow animation
 * 
 * This widget displays a pipe with animated flow direction arrows,
 * suitable for visualizing fluid flow in process diagrams.
 */
class PipeWidget : public QWidget
{
    Q_OBJECT

public:
    enum Orientation {
        Horizontal,
        Vertical
    };

    enum FlowDirection {
        NoFlow,
        Forward,   // Left to right or top to bottom
        Reverse    // Right to left or bottom to top
    };

    explicit PipeWidget(QWidget *parent = nullptr);
    ~PipeWidget() override;

    // Getters
    Orientation orientation() const { return m_orientation; }
    FlowDirection flowDirection() const { return m_flowDirection; }
    double flowRate() const { return m_flowRate; }

    // Setters
    void setOrientation(Orientation orientation);
    void setFlowDirection(FlowDirection direction);
    void setFlowRate(double rate);  // 0-100%
    void setPipeColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private slots:
    void onAnimationTimer();

private:
    void drawPipe(QPainter &painter);
    void drawFlowArrows(QPainter &painter);

    Orientation m_orientation;
    FlowDirection m_flowDirection;
    double m_flowRate;
    QColor m_pipeColor;
    QColor m_flowColor;

    QTimer *m_animationTimer;
    int m_animationOffset;
};

#endif // PIPEWIDGET_H












