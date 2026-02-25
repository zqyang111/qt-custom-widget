#include "CompassWidget.h"
#include <QtMath>

CompassWidget::CompassWidget(QWidget *parent)
    : QWidget(parent)
    , m_angle(0.0)
    , m_faceColor(QColor(240, 240, 240))
    , m_northColor(QColor(255, 0, 0))
    , m_needleColor(QColor(0, 100, 200))
{
    setMinimumSize(120, 120);
}

CompassWidget::~CompassWidget()
{
}

void CompassWidget::setAngle(double angle)
{
    // Normalize angle to 0-360
    while (angle < 0) angle += 360.0;
    while (angle >= 360.0) angle -= 360.0;

    if (qAbs(m_angle - angle) > 0.1) {
        m_angle = angle;
        update();
    }
}

void CompassWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawDegreeMarks(painter);
    drawCardinalPoints(painter);
    drawNeedle(painter);
    // drawAngleDisplay(painter);  // Removed to avoid clutter
}

void CompassWidget::drawBackground(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    // Draw outer ring
    QRadialGradient outerGradient(0, 0, 100);
    outerGradient.setColorAt(0.85, QColor(180, 180, 180));
    outerGradient.setColorAt(1.0, QColor(120, 120, 120));
    
    painter.setPen(QPen(QColor(80, 80, 80), 3));
    painter.setBrush(outerGradient);
    painter.drawEllipse(-98, -98, 196, 196);

    // Draw face
    QRadialGradient faceGradient(0, -30, 100);
    faceGradient.setColorAt(0.0, m_faceColor.lighter(110));
    faceGradient.setColorAt(1.0, m_faceColor.darker(105));
    
    painter.setBrush(faceGradient);
    painter.setPen(QPen(QColor(150, 150, 150), 2));
    painter.drawEllipse(-90, -90, 180, 180);

    painter.restore();
}

void CompassWidget::drawCardinalPoints(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);
    
    // Compass face is fixed, no rotation

    QFont font = painter.font();
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);

    // Draw cardinal directions (fixed positions)
    struct CardinalPoint {
        double angle;
        QString label;
        QColor color;
    };

    QVector<CardinalPoint> cardinals = {
        {0.0, "N", m_northColor},           // North at top
        {90.0, "E", QColor(40, 40, 40)},    // East at right
        {180.0, "S", QColor(40, 40, 40)},   // South at bottom
        {270.0, "W", QColor(40, 40, 40)}    // West at left
    };

    for (const auto &cardinal : cardinals) {
        painter.save();
        painter.rotate(cardinal.angle);
        
        // Draw letter at fixed position
        painter.translate(0, -70);
        painter.rotate(-cardinal.angle);  // Keep text upright
        
        painter.setPen(cardinal.color);
        painter.drawText(QRectF(-15, -12, 30, 24), Qt::AlignCenter, cardinal.label);
        
        painter.restore();
    }

    painter.restore();
}

void CompassWidget::drawDegreeMarks(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);
    
    // Compass face is fixed, no rotation

    // Draw degree marks (without numbers to avoid overlap with N/E/S/W)
    for (int i = 0; i < 360; i += 10) {
        painter.save();
        painter.rotate(i);

        if (i % 30 == 0) {
            // Major marks every 30 degrees
            painter.setPen(QPen(QColor(80, 80, 80), 3));
            painter.drawLine(0, -85, 0, -70);
        } else {
            // Minor marks every 10 degrees
            painter.setPen(QPen(QColor(120, 120, 120), 1));
            painter.drawLine(0, -85, 0, -78);
        }

        painter.restore();
    }

    painter.restore();
}

void CompassWidget::drawNeedle(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    // Rotate needle by the angle (0 = North, clockwise)
    painter.rotate(m_angle);

    // Draw needle shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 80));
    
    QPolygon shadowNorth;
    shadowNorth << QPoint(-8, 3) << QPoint(8, 3) << QPoint(0, -67);
    painter.drawPolygon(shadowNorth);

    // Draw north pointer (blue) - points forward
    QLinearGradient northGradient(0, -65, 0, 0);
    northGradient.setColorAt(0.0, m_needleColor.lighter(120));
    northGradient.setColorAt(1.0, m_needleColor.darker(110));
    
    painter.setBrush(northGradient);
    painter.setPen(QPen(m_needleColor.darker(130), 2));
    
    QPolygon northPointer;
    northPointer << QPoint(-7, 0) << QPoint(7, 0) << QPoint(0, -65);
    painter.drawPolygon(northPointer);

    // Draw south pointer (gray) - points backward
    painter.setBrush(QColor(180, 180, 180));
    painter.setPen(QPen(QColor(120, 120, 120), 2));
    
    QPolygon southPointer;
    southPointer << QPoint(-7, 0) << QPoint(7, 0) << QPoint(0, 40);
    painter.drawPolygon(southPointer);

    painter.restore();
}

void CompassWidget::drawAngleDisplay(QPainter &painter)
{
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(14);
    font.setBold(true);
    painter.setFont(font);

    QString angleText = QString::number(m_angle, 'f', 1) + QString::fromUtf8("\xC2\xB0");
    
    // Draw on black background
    int displayWidth = 70;
    int displayHeight = 25;
    QRect displayRect((width() - displayWidth) / 2, height() - 35, 
                     displayWidth, displayHeight);
    
    painter.setBrush(QColor(20, 20, 20, 200));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(displayRect, 3, 3);
    
    painter.setPen(QColor(0, 255, 100));
    painter.drawText(displayRect, Qt::AlignCenter, angleText);
}

