#ifndef PIECHART_H
#define PIECHART_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QColor>

/**
 * @brief Pie Chart - Statistical pie chart display
 */
class PieChart : public QWidget
{
    Q_OBJECT

public:
    struct PieSlice {
        QString label;
        double value;
        QColor color;
    };

    explicit PieChart(QWidget *parent = nullptr);
    ~PieChart();

    // Data management
    void addSlice(const QString &label, double value, const QColor &color);
    void setSliceValue(const QString &label, double value);
    void clearSlices();
    
    // Configuration
    void setTitle(const QString &title);
    void setShowPercentage(bool show);
    void setShowLegend(bool show);
    void set3DEffect(bool enabled);
    
    QSize sizeHint() const override { return QSize(500, 400); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBackground(QPainter &painter);
    void drawPie(QPainter &painter, const QRect &pieArea);
    void drawLegend(QPainter &painter, const QRect &legendArea);
    double getTotalValue() const;

    QVector<PieSlice> m_slices;
    QString m_title;
    bool m_showPercentage;
    bool m_showLegend;
    bool m_3dEffect;
};

#endif // PIECHART_H










