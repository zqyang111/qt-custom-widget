#ifndef RADARCHART_H
#define RADARCHART_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QColor>

/**
 * @brief Radar Chart - Multi-dimensional comparison chart
 * Spider/Web chart for comparing multiple metrics
 */
class RadarChart : public QWidget
{
    Q_OBJECT

public:
    explicit RadarChart(QWidget *parent = nullptr);
    ~RadarChart();

    // Data management
    void setCategories(const QVector<QString> &categories);
    void addDataSeries(const QString &name, const QVector<double> &values, const QColor &color);
    void clearAllSeries();
    
    // Configuration
    void setMaxValue(double maxValue);
    void setTitle(const QString &title);
    void setShowLegend(bool show);
    void setFillArea(bool fill);
    
    QSize sizeHint() const override { return QSize(700, 700); }
    QSize minimumSizeHint() const override { return QSize(500, 500); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    struct DataSeries {
        QString name;
        QVector<double> values;
        QColor color;
    };

    void drawBackground(QPainter &painter);
    void drawRadarGrid(QPainter &painter, const QPoint &center, int radius);
    void drawDataSeries(QPainter &painter, const QPoint &center, int radius);
    void drawLegend(QPainter &painter);
    void drawCategoryLabels(QPainter &painter, const QPoint &center, int radius);

    QVector<QString> m_categories;
    QVector<DataSeries> m_dataSeries;
    double m_maxValue;
    QString m_title;
    bool m_showLegend;
    bool m_fillArea;
};

#endif // RADARCHART_H

