#include "DigitalWaveform.h"
#include <QPainter>
#include <QPainterPath>

DigitalWaveform::DigitalWaveform(QWidget *parent)
    : QWidget(parent)
    , m_maxTimeSteps(100)
    , m_title("Digital Waveform")
    , m_showLabels(true)
{
}

DigitalWaveform::~DigitalWaveform()
{
}

void DigitalWaveform::addChannel(const QString &name, const QColor &color)
{
    ChannelData channel;
    channel.name = name;
    channel.color = color;
    m_channels[name] = channel;
    m_channelOrder.append(name);
    update();
}

void DigitalWaveform::setChannelState(const QString &name, bool high)
{
    if (m_channels.contains(name)) {
        if (m_channels[name].states.isEmpty() || 
            m_channels[name].states.last() != high) {
            addChannelTransition(name, high);
        }
    }
}

void DigitalWaveform::addChannelTransition(const QString &name, bool state)
{
    if (m_channels.contains(name)) {
        m_channels[name].states.append(state);
        
        if (m_channels[name].states.size() > m_maxTimeSteps) {
            m_channels[name].states.removeFirst();
        }
        
        update();
    }
}

void DigitalWaveform::clearChannel(const QString &name)
{
    if (m_channels.contains(name)) {
        m_channels[name].states.clear();
        update();
    }
}

void DigitalWaveform::clearAllChannels()
{
    for (auto &channel : m_channels) {
        channel.states.clear();
    }
    update();
}

void DigitalWaveform::setMaxTimeSteps(int steps)
{
    m_maxTimeSteps = qMax(10, steps);
    update();
}

void DigitalWaveform::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void DigitalWaveform::setShowLabels(bool show)
{
    m_showLabels = show;
    update();
}

void DigitalWaveform::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);  // Sharp edges for digital signals

    QRect plotArea(100, 40, width() - 120, height() - 60);

    drawBackground(painter);
    drawGrid(painter, plotArea);
    drawWaveforms(painter, plotArea);
    drawChannelLabels(painter, plotArea);
}

void DigitalWaveform::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor(43, 43, 43));
    
    painter.setPen(QColor(224, 224, 224));
    QFont titleFont = painter.font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(10, 10, width() - 20, 25, Qt::AlignCenter, m_title);
}

void DigitalWaveform::drawGrid(QPainter &painter, const QRect &plotArea)
{
    painter.fillRect(plotArea, QColor(0, 0, 0));
    
    painter.setPen(QPen(QColor(40, 40, 40), 1));
    
    // Vertical time grid
    for (int i = 0; i <= 10; ++i) {
        int x = plotArea.left() + plotArea.width() * i / 10;
        painter.drawLine(x, plotArea.top(), x, plotArea.bottom());
    }
    
    // Border
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawRect(plotArea);
}

void DigitalWaveform::drawWaveforms(QPainter &painter, const QRect &plotArea)
{
    if (m_channelOrder.isEmpty()) return;
    
    int numChannels = m_channelOrder.size();
    int channelHeight = plotArea.height() / numChannels;
    
    for (int ch = 0; ch < numChannels; ++ch) {
        const QString &channelName = m_channelOrder[ch];
        const ChannelData &channel = m_channels[channelName];
        
        if (channel.states.isEmpty()) continue;
        
        int channelTop = plotArea.top() + ch * channelHeight;
        int channelBottom = channelTop + channelHeight;
        int channelMid = (channelTop + channelBottom) / 2;
        int signalHeight = channelHeight * 0.7;
        
        // Draw horizontal separation line
        painter.setPen(QPen(QColor(60, 60, 60), 1));
        if (ch > 0) {
            painter.drawLine(plotArea.left(), channelTop, plotArea.right(), channelTop);
        }
        
        // Draw waveform
        painter.setPen(QPen(channel.color, 2));
        
        double stepWidth = static_cast<double>(plotArea.width()) / m_maxTimeSteps;
        
        for (int i = 0; i < channel.states.size(); ++i) {
            int x1 = plotArea.left() + static_cast<int>(i * stepWidth);
            int x2 = plotArea.left() + static_cast<int>((i + 1) * stepWidth);
            
            int y = channel.states[i] ? 
                   (channelMid - signalHeight / 2) :  // HIGH
                   (channelMid + signalHeight / 2);   // LOW
            
            // Horizontal line
            painter.drawLine(x1, y, x2, y);
            
            // Vertical transition
            if (i < channel.states.size() - 1 && channel.states[i] != channel.states[i + 1]) {
                int nextY = channel.states[i + 1] ? 
                           (channelMid - signalHeight / 2) : 
                           (channelMid + signalHeight / 2);
                painter.drawLine(x2, y, x2, nextY);
            }
        }
        
        // Draw HIGH/LOW reference lines (dashed)
        painter.setPen(QPen(QColor(80, 80, 80), 1, Qt::DashLine));
        painter.drawLine(plotArea.left(), channelMid - signalHeight / 2, 
                        plotArea.right(), channelMid - signalHeight / 2);
        painter.drawLine(plotArea.left(), channelMid + signalHeight / 2, 
                        plotArea.right(), channelMid + signalHeight / 2);
    }
}

void DigitalWaveform::drawChannelLabels(QPainter &painter, const QRect &plotArea)
{
    if (!m_showLabels || m_channelOrder.isEmpty()) return;
    
    painter.setPen(QColor(200, 200, 200));
    QFont labelFont = painter.font();
    labelFont.setPointSize(9);
    labelFont.setBold(true);
    painter.setFont(labelFont);
    
    int numChannels = m_channelOrder.size();
    int channelHeight = plotArea.height() / numChannels;
    
    for (int ch = 0; ch < numChannels; ++ch) {
        const QString &channelName = m_channelOrder[ch];
        const ChannelData &channel = m_channels[channelName];
        
        int channelTop = plotArea.top() + ch * channelHeight;
        int channelMid = channelTop + channelHeight / 2;
        
        // Draw channel name
        painter.setPen(channel.color);
        painter.drawText(5, channelMid - 10, 90, 20, Qt::AlignRight | Qt::AlignVCenter, channel.name);
    }
}









