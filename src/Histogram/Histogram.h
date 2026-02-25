#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QColor>

/**
 * @brief Histogram - Statistical distribution chart
 */
class Histogram : public QWidget
{
    Q_OBJECT

public:
    explicit Histogram(QWidget *parent = nullptr);
    ~Histogram();

    // Data management
    void setData(const QVector<double> &data);
    void addValue(double value);
    void clearData();
    
    // Configuration
    void setBinCount(int bins);
    void setRange(double min, double max);
    void setAutoRange(bool autoRange);
    void setBarColor(const QColor &color);
    void setTitle(const QString &title);
    void setXLabel(const QString &label);
    void setYLabel(const QString &label);
    void setShowStatistics(bool show);
    
    QSize sizeHint() const override { return QSize(600, 400); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBackground(QPainter &painter);
    void drawHistogram(QPainter &painter, const QRect &plotArea);
    void drawAxes(QPainter &painter, const QRect &plotArea);
    void drawLabels(QPainter &painter, const QRect &plotArea);
    void drawStatistics(QPainter &painter);
    void calculateHistogram();
    void calculateStatistics();

    QVector<double> m_data;
    QVector<int> m_bins;
    int m_binCount;
    double m_rangeMin;
    double m_rangeMax;
    bool m_autoRange;
    QColor m_barColor;
    QString m_title;
    QString m_xLabel;
    QString m_yLabel;
    bool m_showStatistics;
    
    double m_actualMin;
    double m_actualMax;
    double m_mean;
    double m_stdDev;
    int m_maxBinCount;
};

#endif // HISTOGRAM_H









