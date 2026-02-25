#ifndef STACKEDBARCHART_H
#define STACKEDBARCHART_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QColor>
#include <QMap>

/**
 * @brief Stacked Bar Chart - Multi-series stacked bars
 * Shows composition and comparison
 */
class StackedBarChart : public QWidget
{
    Q_OBJECT

public:
    explicit StackedBarChart(QWidget *parent = nullptr);
    ~StackedBarChart();

    // Data management
    void addSeries(const QString &name, const QColor &color);
    void setBarData(const QString &categoryName, const QMap<QString, double> &seriesValues);
    void clearAllData();
    
    // Configuration
    void setCategories(const QVector<QString> &categories);
    void setMaxValue(double maxValue);
    void setAutoScale(bool autoScale);
    void setTitle(const QString &title);
    void setYLabel(const QString &label);
    void setShowLegend(bool show);
    void setShowValues(bool show);
    
    QSize sizeHint() const override { return QSize(700, 500); }
    QSize minimumSizeHint() const override { return QSize(500, 350); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    struct SeriesInfo {
        QString name;
        QColor color;
    };

    void drawBackground(QPainter &painter);
    void drawGrid(QPainter &painter, const QRect &plotArea);
    void drawBars(QPainter &painter, const QRect &plotArea);
    void drawLegend(QPainter &painter);
    void drawLabels(QPainter &painter, const QRect &plotArea);
    void updateAutoScale();

    QVector<QString> m_categories;
    QVector<SeriesInfo> m_series;
    QMap<QString, QMap<QString, double>> m_data;  // category -> (series -> value)
    
    double m_maxValue;
    bool m_autoScale;
    QString m_title;
    QString m_yLabel;
    bool m_showLegend;
    bool m_showValues;
    double m_autoMaxValue;
};

#endif // STACKEDBARCHART_H

