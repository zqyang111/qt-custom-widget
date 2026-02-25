#ifndef INDUSTRIALPROGRESSBAR_H
#define INDUSTRIALPROGRESSBAR_H

#include <QWidget>
#include <QPainter>

/**
 * @brief Industrial style progress bar widget
 * 
 * This widget displays a horizontal or vertical progress bar with
 * percentage display and customizable appearance.
 */
class IndustrialProgressBar : public QWidget
{
    Q_OBJECT

public:
    enum Orientation {
        Horizontal,
        Vertical
    };

    explicit IndustrialProgressBar(QWidget *parent = nullptr);
    ~IndustrialProgressBar() override;

    // Getters
    int value() const { return m_value; }
    int minimum() const { return m_minimum; }
    int maximum() const { return m_maximum; }
    Orientation orientation() const { return m_orientation; }

    // Setters
    void setValue(int value);
    void setRange(int minimum, int maximum);
    void setOrientation(Orientation orientation);
    void setShowPercentage(bool show);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    void drawBackground(QPainter &painter);
    void drawProgress(QPainter &painter);
    void drawText(QPainter &painter);

    int m_value;
    int m_minimum;
    int m_maximum;
    Orientation m_orientation;
    bool m_showPercentage;

    QColor m_backgroundColor;
    QColor m_progressColor;
    QColor m_borderColor;
};

#endif // INDUSTRIALPROGRESSBAR_H


