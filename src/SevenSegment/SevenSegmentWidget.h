#ifndef SEVENSEGMENTWIDGET_H
#define SEVENSEGMENTWIDGET_H

#include <QWidget>
#include <QPainter>

/**
 * @brief Seven segment display widget
 * 
 * This widget displays a classic seven-segment LED display,
 * suitable for showing digits 0-9 and some letters.
 */
class SevenSegmentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SevenSegmentWidget(QWidget *parent = nullptr);
    ~SevenSegmentWidget() override;

    // Getters
    int digitCount() const { return m_digitCount; }
    QString value() const { return m_value; }
    QColor segmentColor() const { return m_segmentColor; }

    // Setters
    void setValue(int value);
    void setValue(double value, int precision = 2);
    void setValue(const QString &value);
    void setDigitCount(int count);
    void setSegmentColor(const QColor &color);
    void setShowDecimalPoint(bool show);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override { return QSize(40 * m_digitCount, 60); }

private:
    void drawDigit(QPainter &painter, QChar digit, int x, int y, int w, int h);
    void drawSegment(QPainter &painter, int segment, int x, int y, int w, int h, bool lit);
    QVector<bool> getSegmentPattern(QChar digit);

    int m_digitCount;
    QString m_value;
    QColor m_segmentColor;
    QColor m_backgroundColor;
    bool m_showDecimalPoint;
};

#endif // SEVENSEGMENTWIDGET_H












