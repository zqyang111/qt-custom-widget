#ifndef DIGITALWAVEFORM_H
#define DIGITALWAVEFORM_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QColor>
#include <QMap>

/**
 * @brief Digital Waveform - Logic signal timing diagram
 * Shows digital HIGH/LOW signals over time
 */
class DigitalWaveform : public QWidget
{
    Q_OBJECT

public:
    explicit DigitalWaveform(QWidget *parent = nullptr);
    ~DigitalWaveform();

    // Channel management
    void addChannel(const QString &name, const QColor &color);
    void setChannelState(const QString &name, bool high);
    void addChannelTransition(const QString &name, bool state);
    void clearChannel(const QString &name);
    void clearAllChannels();
    
    // Configuration
    void setMaxTimeSteps(int steps);
    void setTitle(const QString &title);
    void setShowLabels(bool show);
    
    QSize sizeHint() const override { return QSize(800, 450); }
    QSize minimumSizeHint() const override { return QSize(600, 300); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    struct ChannelData {
        QString name;
        QColor color;
        QVector<bool> states;  // State history
    };

    void drawBackground(QPainter &painter);
    void drawGrid(QPainter &painter, const QRect &plotArea);
    void drawWaveforms(QPainter &painter, const QRect &plotArea);
    void drawChannelLabels(QPainter &painter, const QRect &plotArea);

    QMap<QString, ChannelData> m_channels;
    QVector<QString> m_channelOrder;
    int m_maxTimeSteps;
    QString m_title;
    bool m_showLabels;
};

#endif // DIGITALWAVEFORM_H

