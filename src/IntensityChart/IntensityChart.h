#ifndef INTENSITYCHART_H
#define INTENSITYCHART_H

#include <QWidget>
#include <QVector>
#include <QColor>

/**
 * @brief LabVIEW-style Intensity Chart / Heat Map
 * Displays 2D data as color-coded intensity map
 */
class IntensityChart : public QWidget
{
    Q_OBJECT

public:
    explicit IntensityChart(QWidget *parent = nullptr);
    ~IntensityChart();

    // Data management
    void setData(const QVector<QVector<double>> &data);
    void setDataAt(int row, int col, double value);
    void clearData();
    void setSize(int rows, int cols);
    
    // Configuration
    void setValueRange(double min, double max);
    void setAutoScale(bool autoScale);
    void setColorMap(int mapType);  // 0=Rainbow, 1=Heat, 2=GrayScale
    void setGridEnabled(bool enabled);
    void setTitle(const QString &title);
    
    QSize sizeHint() const override { return QSize(500, 400); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBackground(QPainter &painter);
    void drawIntensityMap(QPainter &painter, const QRect &plotArea);
    void drawColorBar(QPainter &painter, const QRect &barArea);
    void drawLabels(QPainter &painter);
    QColor valueToColor(double value, double minVal, double maxVal);
    void updateAutoScale();

    QVector<QVector<double>> m_data;
    int m_rows;
    int m_cols;
    double m_valueMin;
    double m_valueMax;
    bool m_autoScale;
    int m_colorMap;
    bool m_gridEnabled;
    QString m_title;
    
    double m_autoValueMin;
    double m_autoValueMax;
};

#endif // INTENSITYCHART_H











