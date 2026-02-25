#ifndef LEVELINDICATORWIDGET_H
#define LEVELINDICATORWIDGET_H

#include <QWidget>
#include <QPainter>

/**
 * @brief Industrial style level indicator widget
 * 
 * This widget displays a vertical tank with liquid level indicator,
 * suitable for showing tank levels, water levels, or any volumetric measurements.
 */
class LevelIndicatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelIndicatorWidget(QWidget *parent = nullptr);
    ~LevelIndicatorWidget() override;

    // Getters
    double level() const { return m_level; }
    double minLevel() const { return m_minLevel; }
    double maxLevel() const { return m_maxLevel; }
    QString unit() const { return m_unit; }

    // Setters
    void setLevel(double level);
    void setRange(double minLevel, double maxLevel);
    void setUnit(const QString &unit);
    void setWarningRange(double warningLevel, double criticalLevel);
    void setLiquidColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(150, 300); }
    QSize minimumSizeHint() const override { return QSize(100, 150); }

private:
    void drawTank(QPainter &painter);
    void drawLiquid(QPainter &painter);
    void drawScale(QPainter &painter);
    void drawValue(QPainter &painter);

    double m_level;
    double m_minLevel;
    double m_maxLevel;
    double m_warningLevel;
    double m_criticalLevel;
    QString m_unit;

    QColor m_liquidColor;
    QColor m_warningColor;
    QColor m_criticalColor;
};

#endif // LEVELINDICATORWIDGET_H

