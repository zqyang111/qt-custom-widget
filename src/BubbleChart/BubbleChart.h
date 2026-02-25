#ifndef BUBBLECHART_H
#define BUBBLECHART_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QColor>

/**
 * @brief Bubble Chart - Three-variable scatter plot
 * Shows X, Y position and size (third variable)
 */
class BubbleChart : public QWidget
{
    Q_OBJECT

public:
    struct Bubble {
        double x;
        double y;
        double size;
        QString label;
        QColor color;
    };

    explicit BubbleChart(QWidget *parent = nullptr);
    ~BubbleChart();

    // Data management
    void addBubble(double x, double y, double size, const QString &label, const QColor &color);
    void clearBubbles();
    
    // Configuration
    void setXRange(double min, double max);
    void setYRange(double min, double max);
    void setSizeRange(double min, double max);
    void setAutoScale(bool autoScale);
    void setTitle(const QString &title);
    void setXLabel(const QString &label);
    void setYLabel(const QString &label);
    void setShowLabels(bool show);
    
    QSize sizeHint() const override { return QSize(700, 600); }
    QSize minimumSizeHint() const override { return QSize(500, 400); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBackground(QPainter &painter);
    void drawGrid(QPainter &painter, const QRect &plotArea);
    void drawBubbles(QPainter &painter, const QRect &plotArea);
    void drawLabels(QPainter &painter, const QRect &plotArea);
    void updateAutoScale();

    QVector<Bubble> m_bubbles;
    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
    double m_sizeMin;
    double m_sizeMax;
    bool m_autoScale;
    QString m_title;
    QString m_xLabel;
    QString m_yLabel;
    bool m_showLabels;
};

#endif // BUBBLECHART_H

