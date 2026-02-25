#include "IntensityChart.h"
#include <QPainter>
#include <QtMath>
#include <algorithm>

IntensityChart::IntensityChart(QWidget *parent)
    : QWidget(parent)
    , m_rows(20)
    , m_cols(20)
    , m_valueMin(0.0)
    , m_valueMax(100.0)
    , m_autoScale(true)
    , m_colorMap(0)  // Rainbow
    , m_gridEnabled(true)
    , m_title("Intensity Chart")
    , m_autoValueMin(0.0)
    , m_autoValueMax(100.0)
{
    // Initialize with zero data
    m_data.resize(m_rows);
    for (int i = 0; i < m_rows; ++i) {
        m_data[i].resize(m_cols);
        for (int j = 0; j < m_cols; ++j) {
            m_data[i][j] = 0.0;
        }
    }
}

IntensityChart::~IntensityChart()
{
}

void IntensityChart::setData(const QVector<QVector<double>> &data)
{
    m_data = data;
    if (!data.isEmpty()) {
        m_rows = data.size();
        m_cols = data[0].size();
    }
    
    if (m_autoScale) {
        updateAutoScale();
    }
    
    update();
}

void IntensityChart::setDataAt(int row, int col, double value)
{
    if (row >= 0 && row < m_rows && col >= 0 && col < m_cols) {
        m_data[row][col] = value;
        
        if (m_autoScale) {
            updateAutoScale();
        }
        
        update();
    }
}

void IntensityChart::clearData()
{
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            m_data[i][j] = 0.0;
        }
    }
    update();
}

void IntensityChart::setSize(int rows, int cols)
{
    m_rows = qMax(1, rows);
    m_cols = qMax(1, cols);
    
    m_data.resize(m_rows);
    for (int i = 0; i < m_rows; ++i) {
        m_data[i].resize(m_cols);
        for (int j = 0; j < m_cols; ++j) {
            m_data[i][j] = 0.0;
        }
    }
    
    update();
}

void IntensityChart::setValueRange(double min, double max)
{
    m_valueMin = min;
    m_valueMax = max;
    m_autoScale = false;
    update();
}

void IntensityChart::setAutoScale(bool autoScale)
{
    m_autoScale = autoScale;
    if (autoScale) {
        updateAutoScale();
    }
    update();
}

void IntensityChart::setColorMap(int mapType)
{
    m_colorMap = qBound(0, mapType, 2);
    update();
}

void IntensityChart::setGridEnabled(bool enabled)
{
    m_gridEnabled = enabled;
    update();
}

void IntensityChart::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void IntensityChart::updateAutoScale()
{
    if (m_data.isEmpty()) return;
    
    m_autoValueMin = m_data[0][0];
    m_autoValueMax = m_data[0][0];
    
    for (const auto &row : m_data) {
        for (double value : row) {
            m_autoValueMin = qMin(m_autoValueMin, value);
            m_autoValueMax = qMax(m_autoValueMax, value);
        }
    }
}

QColor IntensityChart::valueToColor(double value, double minVal, double maxVal)
{
    double range = maxVal - minVal;
    if (range < 0.001) range = 1.0;
    
    double normalized = (value - minVal) / range;
    normalized = qBound(0.0, normalized, 1.0);
    
    if (m_colorMap == 0) {
        // Rainbow color map
        double hue = (1.0 - normalized) * 240.0;  // Blue to red
        return QColor::fromHsv(static_cast<int>(hue), 255, 255);
    } else if (m_colorMap == 1) {
        // Heat map (black -> red -> yellow -> white)
        if (normalized < 0.33) {
            double t = normalized / 0.33;
            return QColor(static_cast<int>(255 * t), 0, 0);
        } else if (normalized < 0.66) {
            double t = (normalized - 0.33) / 0.33;
            return QColor(255, static_cast<int>(255 * t), 0);
        } else {
            double t = (normalized - 0.66) / 0.34;
            return QColor(255, 255, static_cast<int>(255 * t));
        }
    } else {
        // Grayscale
        int gray = static_cast<int>(normalized * 255);
        return QColor(gray, gray, gray);
    }
}

void IntensityChart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);  // Faster for grid

    // Define areas
    QRect plotArea(60, 40, width() - 140, height() - 80);
    QRect colorBarArea(width() - 70, 40, 30, height() - 80);

    drawBackground(painter);
    drawIntensityMap(painter, plotArea);
    drawColorBar(painter, colorBarArea);
    drawLabels(painter);
}

void IntensityChart::drawBackground(QPainter &painter)
{
    // Dark theme background
    painter.fillRect(rect(), QColor(43, 43, 43));  // #2b2b2b
    
    // Title
    painter.setPen(QColor(224, 224, 224));  // Light gray text
    QFont titleFont = painter.font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 25, Qt::AlignCenter, m_title);
}

void IntensityChart::drawIntensityMap(QPainter &painter, const QRect &plotArea)
{
    if (m_data.isEmpty()) return;
    
    double minVal = m_autoScale ? m_autoValueMin : m_valueMin;
    double maxVal = m_autoScale ? m_autoValueMax : m_valueMax;
    
    double cellWidth = static_cast<double>(plotArea.width()) / m_cols;
    double cellHeight = static_cast<double>(plotArea.height()) / m_rows;
    
    // Draw cells
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            double value = m_data[i][j];
            QColor cellColor = valueToColor(value, minVal, maxVal);
            
            QRectF cellRect(
                plotArea.left() + j * cellWidth,
                plotArea.top() + i * cellHeight,
                cellWidth,
                cellHeight
            );
            
            painter.fillRect(cellRect, cellColor);
            
            // Draw grid
            if (m_gridEnabled) {
                painter.setPen(QPen(QColor(100, 100, 100, 100), 1));
                painter.drawRect(cellRect);
            }
        }
    }
    
    // Draw border
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.drawRect(plotArea);
}

void IntensityChart::drawColorBar(QPainter &painter, const QRect &barArea)
{
    double minVal = m_autoScale ? m_autoValueMin : m_valueMin;
    double maxVal = m_autoScale ? m_autoValueMax : m_valueMax;
    
    // Draw color gradient bar
    for (int i = 0; i < barArea.height(); ++i) {
        double normalized = 1.0 - static_cast<double>(i) / barArea.height();
        double value = minVal + (maxVal - minVal) * normalized;
        QColor color = valueToColor(value, minVal, maxVal);
        
        painter.fillRect(barArea.left(), barArea.top() + i, barArea.width(), 1, color);
    }
    
    // Border
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.drawRect(barArea);
    
    // Labels
    painter.setPen(QColor(200, 200, 200));  // Light gray for dark theme
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    
    painter.drawText(barArea.right() + 5, barArea.top() - 5, 40, 20, 
                     Qt::AlignLeft, QString::number(maxVal, 'f', 1));
    painter.drawText(barArea.right() + 5, barArea.bottom() - 5, 40, 20, 
                     Qt::AlignLeft, QString::number(minVal, 'f', 1));
}

void IntensityChart::drawLabels(QPainter &painter)
{
    // No additional labels needed for now
    Q_UNUSED(painter);
}

