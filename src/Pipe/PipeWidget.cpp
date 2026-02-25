#include "PipeWidget.h"
#include <QtMath>

PipeWidget::PipeWidget(QWidget *parent)
    : QWidget(parent)
    , m_orientation(Horizontal)
    , m_flowDirection(Forward)
    , m_flowRate(50.0)
    , m_pipeColor(QColor(120, 120, 120))
    , m_flowColor(QColor(0, 255, 100))  // Bright green for better visibility
    , m_animationTimer(new QTimer(this))
    , m_animationOffset(0)
{
    setMinimumSize(100, 40);

    connect(m_animationTimer, &QTimer::timeout, this, &PipeWidget::onAnimationTimer);
    m_animationTimer->start(50);  // 20 FPS
}

PipeWidget::~PipeWidget()
{
}

void PipeWidget::setOrientation(Orientation orientation)
{
    if (m_orientation != orientation) {
        m_orientation = orientation;
        updateGeometry();
        update();
    }
}

void PipeWidget::setFlowDirection(FlowDirection direction)
{
    if (m_flowDirection != direction) {
        m_flowDirection = direction;
        update();
    }
}

void PipeWidget::setFlowRate(double rate)
{
    if (rate < 0.0) rate = 0.0;
    if (rate > 100.0) rate = 100.0;

    m_flowRate = rate;
    update();
}

void PipeWidget::setPipeColor(const QColor &color)
{
    m_pipeColor = color;
    update();
}

QSize PipeWidget::sizeHint() const
{
    if (m_orientation == Horizontal)
        return QSize(200, 60);
    else
        return QSize(60, 200);
}

QSize PipeWidget::minimumSizeHint() const
{
    if (m_orientation == Horizontal)
        return QSize(100, 40);
    else
        return QSize(40, 100);
}

void PipeWidget::onAnimationTimer()
{
    if (m_flowDirection != NoFlow && m_flowRate > 0.0) {
        m_animationOffset += (m_flowDirection == Forward ? 1 : -1);
        if (m_animationOffset > 30) m_animationOffset = 0;
        if (m_animationOffset < 0) m_animationOffset = 30;
        update();
    }
}

void PipeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(40, 40, 40));

    drawPipe(painter);
    drawFlowArrows(painter);
}

void PipeWidget::drawPipe(QPainter &painter)
{
    if (m_orientation == Horizontal) {
        int pipeHeight = qMin(height() - 10, 40);
        int pipeY = (height() - pipeHeight) / 2;

        // Draw pipe body
        QLinearGradient gradient(0, pipeY, 0, pipeY + pipeHeight);
        gradient.setColorAt(0.0, m_pipeColor.darker(150));
        gradient.setColorAt(0.5, m_pipeColor);
        gradient.setColorAt(1.0, m_pipeColor.darker(150));

        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(60, 60, 60), 2));
        painter.drawRect(5, pipeY, width() - 10, pipeHeight);

        // Draw pipe highlights
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 255, 255, 30));
        painter.drawRect(5, pipeY + 2, width() - 10, pipeHeight / 4);

    } else {  // Vertical
        int pipeWidth = qMin(width() - 10, 40);
        int pipeX = (width() - pipeWidth) / 2;

        // Draw pipe body
        QLinearGradient gradient(pipeX, 0, pipeX + pipeWidth, 0);
        gradient.setColorAt(0.0, m_pipeColor.darker(150));
        gradient.setColorAt(0.5, m_pipeColor);
        gradient.setColorAt(1.0, m_pipeColor.darker(150));

        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(60, 60, 60), 2));
        painter.drawRect(pipeX, 5, pipeWidth, height() - 10);

        // Draw pipe highlights
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 255, 255, 30));
        painter.drawRect(pipeX + 2, 5, pipeWidth / 4, height() - 10);
    }
}

void PipeWidget::drawFlowArrows(QPainter &painter)
{
    if (m_flowDirection == NoFlow || m_flowRate < 0.1)
        return;

    painter.setPen(Qt::NoPen);
    
    // Define two colors: green and orange
    QColor greenColor(0, 255, 100);
    QColor orangeColor(255, 165, 0);
    
    int alphaValue = static_cast<int>(150 * m_flowRate / 100.0);

    if (m_orientation == Horizontal) {
        int pipeHeight = qMin(height() - 10, 40);
        int pipeY = (height() - pipeHeight) / 2;
        int arrowY = height() / 2;
        int arrowSpacing = 30;

        int numArrows = (width() / arrowSpacing) + 2;
        
        for (int i = -1; i < numArrows; ++i) {
            int x = 10 + i * arrowSpacing + m_animationOffset;
            
            if (x < 0 || x > width()) continue;

            // Alternate colors: green for even, orange for odd
            QColor arrowColor = (i % 2 == 0) ? greenColor : orangeColor;
            arrowColor.setAlpha(alphaValue);
            painter.setBrush(arrowColor);

            // Add wave effect - arrows move up and down in a sine wave
            double waveOffset = qSin((x + m_animationOffset * 2) * 0.1) * 3;
            int waveY = arrowY + static_cast<int>(waveOffset);

            QPolygon arrow;
            if (m_flowDirection == Forward) {
                // Arrow pointing right (flatter) - tip at right
                arrow << QPoint(x + 10, waveY)
                      << QPoint(x, waveY - 4)
                      << QPoint(x, waveY + 4);
            } else {
                // Arrow pointing left (flatter) - tip at left
                arrow << QPoint(x - 10, waveY)
                      << QPoint(x, waveY - 4)
                      << QPoint(x, waveY + 4);
            }
            painter.drawPolygon(arrow);
        }

    } else {  // Vertical
        int pipeWidth = qMin(width() - 10, 40);
        int pipeX = (width() - pipeWidth) / 2;
        int arrowX = width() / 2;
        int arrowSpacing = 30;

        int numArrows = (height() / arrowSpacing) + 2;
        
        for (int i = -1; i < numArrows; ++i) {
            int y = 10 + i * arrowSpacing + m_animationOffset;
            
            if (y < 0 || y > height()) continue;

            // Alternate colors: green for even, orange for odd
            QColor arrowColor = (i % 2 == 0) ? greenColor : orangeColor;
            arrowColor.setAlpha(alphaValue);
            painter.setBrush(arrowColor);

            // Add wave effect - arrows move left and right in a sine wave
            double waveOffset = qSin((y + m_animationOffset * 2) * 0.1) * 3;
            int waveX = arrowX + static_cast<int>(waveOffset);

            QPolygon arrow;
            if (m_flowDirection == Forward) {
                // Arrow pointing down (flatter) - tip at bottom
                arrow << QPoint(waveX, y + 10)
                      << QPoint(waveX - 4, y)
                      << QPoint(waveX + 4, y);
            } else {
                // Arrow pointing up (flatter) - tip at top
                arrow << QPoint(waveX, y - 10)
                      << QPoint(waveX - 4, y)
                      << QPoint(waveX + 4, y);
            }
            painter.drawPolygon(arrow);
        }
    }
}

