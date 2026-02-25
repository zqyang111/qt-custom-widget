#include "SpectrumAnalyzer.h"
#include <QPainter>
#include <QtMath>
#include <algorithm>

SpectrumAnalyzer::SpectrumAnalyzer(QWidget *parent)
    : QWidget(parent)
    , m_freqMin(0.0)
    , m_freqMax(1000.0)
    , m_ampMin(0.0)
    , m_ampMax(100.0)
    , m_autoScale(true)
    , m_barColor(QColor(0, 255, 100))
    , m_peakHoldEnabled(true)
    , m_title("Spectrum Analyzer")
    , m_freqUnit("Hz")
    , m_ampUnit("dB")
{
    // Initialize with default frequency bins
    for (int i = 0; i < 50; ++i) {
        m_frequencies.append(i * 20.0);
        m_amplitudes.append(0.0);
        m_peakHold.append(0.0);
    }
}

SpectrumAnalyzer::~SpectrumAnalyzer()
{
}

void SpectrumAnalyzer::setSpectrumData(const QVector<double> &frequencies, const QVector<double> &amplitudes)
{
    m_frequencies = frequencies;
    m_amplitudes = amplitudes;
    
    if (m_peakHold.size() != amplitudes.size()) {
        m_peakHold.resize(amplitudes.size());
        m_peakHold.fill(0.0);
    }
    
    updatePeakHold();
    update();
}

void SpectrumAnalyzer::setAmplitudeAt(int index, double amplitude)
{
    if (index >= 0 && index < m_amplitudes.size()) {
        m_amplitudes[index] = amplitude;
        updatePeakHold();
        update();
    }
}

void SpectrumAnalyzer::clearData()
{
    m_amplitudes.fill(0.0);
    m_peakHold.fill(0.0);
    update();
}

void SpectrumAnalyzer::setFrequencyRange(double min, double max)
{
    m_freqMin = min;
    m_freqMax = max;
    update();
}

void SpectrumAnalyzer::setAmplitudeRange(double min, double max)
{
    m_ampMin = min;
    m_ampMax = max;
    m_autoScale = false;
    update();
}

void SpectrumAnalyzer::setAutoScale(bool autoScale)
{
    m_autoScale = autoScale;
    update();
}

void SpectrumAnalyzer::setBarColor(const QColor &color)
{
    m_barColor = color;
    update();
}

void SpectrumAnalyzer::setPeakHoldEnabled(bool enabled)
{
    m_peakHoldEnabled = enabled;
    update();
}

void SpectrumAnalyzer::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void SpectrumAnalyzer::setFrequencyUnit(const QString &unit)
{
    m_freqUnit = unit;
    update();
}

void SpectrumAnalyzer::setAmplitudeUnit(const QString &unit)
{
    m_ampUnit = unit;
    update();
}

void SpectrumAnalyzer::updatePeakHold()
{
    for (int i = 0; i < qMin(m_amplitudes.size(), m_peakHold.size()); ++i) {
        m_peakHold[i] = qMax(m_peakHold[i], m_amplitudes[i]);
        // Slow decay
        m_peakHold[i] *= 0.995;
    }
}

void SpectrumAnalyzer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect plotArea(60, 40, width() - 80, height() - 80);

    drawBackground(painter);
    drawGrid(painter, plotArea);
    drawSpectrum(painter, plotArea);
    
    if (m_peakHoldEnabled) {
        drawPeakHold(painter, plotArea);
    }
    
    drawLabels(painter, plotArea);
}

void SpectrumAnalyzer::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(43, 43, 43));
    
    painter.setPen(QColor(224, 224, 224));
    QFont titleFont = painter.font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 25, Qt::AlignCenter, m_title);
}

void SpectrumAnalyzer::drawGrid(QPainter &painter, const QRect &plotArea)
{
    // Dark plot background
    painter.fillRect(plotArea, QColor(0, 0, 0));
    
    painter.setPen(QPen(QColor(40, 40, 40), 1));
    
    // Vertical grid
    for (int i = 0; i <= 10; ++i) {
        int x = plotArea.left() + plotArea.width() * i / 10;
        painter.drawLine(x, plotArea.top(), x, plotArea.bottom());
    }
    
    // Horizontal grid
    for (int i = 0; i <= 8; ++i) {
        int y = plotArea.top() + plotArea.height() * i / 8;
        painter.drawLine(plotArea.left(), y, plotArea.right(), y);
    }
    
    // Border
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawRect(plotArea);
}

void SpectrumAnalyzer::drawSpectrum(QPainter &painter, const QRect &plotArea)
{
    if (m_amplitudes.isEmpty()) return;
    
    double ampMax = m_autoScale ? 
        *std::max_element(m_amplitudes.begin(), m_amplitudes.end()) : m_ampMax;
    if (ampMax < 0.001) ampMax = 1.0;
    
    double barWidth = static_cast<double>(plotArea.width()) / m_amplitudes.size();
    
    // Draw bars
    for (int i = 0; i < m_amplitudes.size(); ++i) {
        double normalizedAmp = m_amplitudes[i] / ampMax;
        double barHeight = plotArea.height() * normalizedAmp;
        
        QRectF barRect(
            plotArea.left() + i * barWidth,
            plotArea.bottom() - barHeight,
            barWidth - 1,
            barHeight
        );
        
        // Gradient fill - green to yellow to red based on amplitude
        QLinearGradient gradient(barRect.topLeft(), barRect.bottomLeft());
        if (normalizedAmp > 0.8) {
            gradient.setColorAt(0.0, QColor(255, 0, 0));
            gradient.setColorAt(1.0, QColor(255, 100, 0));
        } else if (normalizedAmp > 0.5) {
            gradient.setColorAt(0.0, QColor(255, 255, 0));
            gradient.setColorAt(1.0, QColor(200, 200, 0));
        } else {
            gradient.setColorAt(0.0, m_barColor.lighter(120));
            gradient.setColorAt(1.0, m_barColor);
        }
        
        painter.setPen(Qt::NoPen);
        painter.setBrush(gradient);
        painter.drawRect(barRect);
    }
}

void SpectrumAnalyzer::drawPeakHold(QPainter &painter, const QRect &plotArea)
{
    if (m_peakHold.isEmpty()) return;
    
    double ampMax = m_autoScale ? 
        *std::max_element(m_amplitudes.begin(), m_amplitudes.end()) : m_ampMax;
    if (ampMax < 0.001) ampMax = 1.0;
    
    double barWidth = static_cast<double>(plotArea.width()) / m_peakHold.size();
    
    painter.setPen(QPen(QColor(255, 200, 0), 2));
    
    for (int i = 0; i < m_peakHold.size(); ++i) {
        double normalizedPeak = m_peakHold[i] / ampMax;
        int peakY = plotArea.bottom() - static_cast<int>(plotArea.height() * normalizedPeak);
        
        int x = plotArea.left() + static_cast<int>(i * barWidth);
        painter.drawLine(x, peakY, x + static_cast<int>(barWidth) - 1, peakY);
    }
}

void SpectrumAnalyzer::drawLabels(QPainter &painter, const QRect &plotArea)
{
    painter.setPen(QColor(200, 200, 200));
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);
    
    // Frequency labels
    for (int i = 0; i <= 5; ++i) {
        double freq = m_freqMin + (m_freqMax - m_freqMin) * i / 5;
        int x = plotArea.left() + plotArea.width() * i / 5;
        QString label = QString::number(freq, 'f', 0);
        painter.drawText(x - 30, plotArea.bottom() + 5, 60, 20, Qt::AlignCenter, label);
    }
    
    // Amplitude labels
    double ampMax = m_autoScale ? 
        (m_amplitudes.isEmpty() ? 100.0 : *std::max_element(m_amplitudes.begin(), m_amplitudes.end())) 
        : m_ampMax;
    
    for (int i = 0; i <= 5; ++i) {
        double amp = ampMax * (5 - i) / 5;
        int y = plotArea.top() + plotArea.height() * i / 5;
        QString label = QString::number(amp, 'f', 0);
        painter.drawText(5, y - 10, 50, 20, Qt::AlignRight | Qt::AlignVCenter, label);
    }
    
    // Axis labels
    painter.drawText(plotArea.left(), plotArea.bottom() + 25, 
                     plotArea.width(), 20, Qt::AlignCenter, 
                     QString("Frequency (%1)").arg(m_freqUnit));
    
    painter.save();
    painter.translate(20, plotArea.center().y());
    painter.rotate(-90);
    painter.drawText(-80, -10, 160, 20, Qt::AlignCenter, 
                     QString("Amplitude (%1)").arg(m_ampUnit));
    painter.restore();
}









