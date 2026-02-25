#ifndef SPECTRUMANALYZER_H
#define SPECTRUMANALYZER_H

#include <QWidget>
#include <QVector>
#include <QColor>

/**
 * @brief Spectrum Analyzer - Frequency domain display
 * Shows signal amplitude vs frequency (like FFT display)
 */
class SpectrumAnalyzer : public QWidget
{
    Q_OBJECT

public:
    explicit SpectrumAnalyzer(QWidget *parent = nullptr);
    ~SpectrumAnalyzer();

    // Data management
    void setSpectrumData(const QVector<double> &frequencies, const QVector<double> &amplitudes);
    void setAmplitudeAt(int index, double amplitude);
    void clearData();
    
    // Configuration
    void setFrequencyRange(double min, double max);
    void setAmplitudeRange(double min, double max);
    void setAutoScale(bool autoScale);
    void setBarColor(const QColor &color);
    void setPeakHoldEnabled(bool enabled);
    void setTitle(const QString &title);
    void setFrequencyUnit(const QString &unit);
    void setAmplitudeUnit(const QString &unit);
    
    QSize sizeHint() const override { return QSize(600, 400); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawBackground(QPainter &painter);
    void drawGrid(QPainter &painter, const QRect &plotArea);
    void drawSpectrum(QPainter &painter, const QRect &plotArea);
    void drawPeakHold(QPainter &painter, const QRect &plotArea);
    void drawLabels(QPainter &painter, const QRect &plotArea);
    void updatePeakHold();

    QVector<double> m_frequencies;
    QVector<double> m_amplitudes;
    QVector<double> m_peakHold;
    
    double m_freqMin;
    double m_freqMax;
    double m_ampMin;
    double m_ampMax;
    bool m_autoScale;
    QColor m_barColor;
    bool m_peakHoldEnabled;
    QString m_title;
    QString m_freqUnit;
    QString m_ampUnit;
};

#endif // SPECTRUMANALYZER_H









