#ifndef SWITCHBUTTONWIDGET_H
#define SWITCHBUTTONWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

/**
 * @brief Industrial style switch button widget
 * 
 * This widget displays a toggle switch button with smooth animation,
 * suitable for on/off control operations.
 */
class SwitchButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchButtonWidget(QWidget *parent = nullptr);
    ~SwitchButtonWidget() override;

    // Getters
    bool isChecked() const { return m_checked; }
    bool isEnabled() const { return m_enabled; }

    // Setters
    void setChecked(bool checked);
    void setEnabled(bool enabled);
    void toggle();

signals:
    void toggled(bool checked);
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    QSize sizeHint() const override { return QSize(80, 40); }
    QSize minimumSizeHint() const override { return QSize(60, 30); }

private:
    void drawBackground(QPainter &painter);
    void drawHandle(QPainter &painter);

    bool m_checked;
    bool m_enabled;
    bool m_pressed;

    QColor m_onColor;
    QColor m_offColor;
    QColor m_disabledColor;
};

#endif // SWITCHBUTTONWIDGET_H












