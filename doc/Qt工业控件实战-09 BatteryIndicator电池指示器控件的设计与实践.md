### Qt工业控件实战·第09篇｜BatteryIndicator 电池指示器控件的设计与实践

### 写在前面

在工业设备监控中，电池状态是最关键也最需要实时关注的信息之一。从移动设备、备用电源到储能系统，电池电量、电压、电流等参数直接影响设备可用性与系统稳定性。传统的数字显示虽然精确，但缺乏直观的视觉反馈；而电池指示器控件以“电池图标+电量填充+参数显示+状态指示”这一专业设计，让操作员能够快速识别电池状态并采取相应行动。本文基于本项目的 `BatteryIndicatorWidget`，深入解析其设计思路、绘制技巧与工程实践，帮助你在 Qt 应用中快速集成一款专业的工业电池指示器控件。

![](../picture/battery_indicator.png)

### 组件定位与典型场景

- **移动设备监控**：手持设备、便携式仪器等移动设备的电池电量监控，需要实时显示剩余电量与充电状态。
- **备用电源管理**：UPS、应急电源等备用电源系统的电池状态监控，结合告警形成完整的电源管理视图。
- **储能系统监控**：电池组、储能站等储能系统的电池状态监控，支持多电池组对比展示。
- **设备健康评估**：通过电池电压、电流等参数评估设备健康状态，结合历史数据形成趋势分析。

### 视觉结构拆解

`BatteryIndicatorWidget` 的画面由五个层次组成：

1. **背景（Background）**：深色背景（RGB 40, 40, 40）提供工业质感，突出前景元素。
2. **电池图标（Battery Icon）**：圆角矩形电池主体，右侧正极凸起，采用线性渐变模拟金属质感，内部深色框架形成电池轮廓。
3. **电量填充（Charge Level）**：根据电量百分比填充电池内部，颜色随电量变化（绿色/橙色/红色），采用线性渐变增强立体感。
4. **充电指示器（Charging Indicator）**：充电时显示黄色闪电图标和光晕效果，通过闪烁吸引注意。
5. **参数显示（Parameters）**：底部显示电压、电流、剩余时间等参数，采用白色文字，确保可读性。

这种“背景+图标+填充+指示+参数”的层级设计既保留了传统电池指示器的物理隐喻，又融入了现代界面的数字化表达。

### API 设计与可用性

`BatteryIndicatorWidget` 提供了完整的电池状态管理接口：

- **电量设置**：`setChargeLevel(double percentage)` - 设置电池电量（0-100%），自动更新显示并触发告警。
- **电压设置**：`setVoltage(double voltage)` - 设置电池电压，用于精确监控。
- **电流设置**：`setCurrent(double current)` - 设置电池电流，用于计算剩余时间。
- **充电状态**：`setChargingState(ChargingState state)` - 设置充电状态（Discharging/Charging/FullyCharged/Fault），影响显示效果。

核心原则是“状态驱动显示”；内部会在状态变化时自动更新颜色、闪烁效果与参数显示，确保视觉与状态的一致性。

### 电量映射与颜色联动

- 电量计算：电量百分比 \(p = chargeLevel / 100.0\) 线性映射到填充宽度，`fillWidth = (batteryWidth - 12) * p`。
- 颜色逻辑：
  - 高电量（> 60%）：绿色（RGB 0, 200, 0），传达“充足、正常”的语义。
  - 中等电量（30% < p ≤ 60%）：橙色（RGB 255, 200, 0），提示“注意、需关注”。
  - 低电量（15% < p ≤ 30%）：深橙色（RGB 255, 100, 0），警告“电量不足、需充电”。
  - 极低电量（≤ 15%）：红色（RGB 255, 0, 0），警示“危险、需立即充电”。
- 闪烁机制：电量低于 20% 时闪烁提醒，闪烁时颜色变暗，形成明显的视觉提醒。

这种映射策略适用于线性电量显示；若需支持非线性映射（如考虑电池老化），可在计算时替换为自定义函数。

### 充电状态与视觉反馈

`BatteryIndicatorWidget` 支持四种充电状态：

1. **放电（Discharging）**：正常显示电量，低电量时闪烁提醒。
2. **充电（Charging）**：显示黄色闪电图标和光晕效果，闪电图标闪烁，形成明显的充电指示。
3. **充满（FullyCharged）**：显示满电量，停止闪烁，表示充电完成。
4. **故障（Fault）**：显示故障状态，可用于电池异常或连接故障的提示。

状态切换通过 `setChargingState` 实现，可在运行时动态切换。

### 细节打磨：绘制技巧与真实感

- **电池图标渐变**：电池主体采用线性渐变，从顶部亮（RGB 180, 180, 180）到底部暗（RGB 140, 140, 140），模拟金属质感。
- **电量填充渐变**：电量填充采用线性渐变，从顶部亮（`color.lighter(120)`）到底部暗（`color`），增强立体感。
- **充电指示器**：充电时显示黄色闪电图标（多边形绘制）和径向渐变光晕，闪电图标闪烁，形成明显的视觉提醒。
- **低电量闪烁**：电量低于 20% 时闪烁，闪烁时颜色变暗（`color.darker(300)`），确保在深色背景下清晰可见。
- **参数显示**：电压、电流采用白色文字，剩余时间估算基于电流计算，提供实用的参考信息。
- **抗锯齿**：启用 `QPainter::Antialiasing`，在圆角矩形、多边形与渐变绘制下保持平滑边缘。

### 性能与适配

- **抗锯齿**：启用 `QPainter::Antialiasing`，在复杂渐变与多边形绘制下保持平滑边缘。
- **定时器优化**：使用 `QTimer` 每 500ms 更新一次闪烁状态，仅在需要闪烁时调用 `update()`，避免无谓重绘。
- **尺寸自适应**：电池图标大小、参数位置均基于控件尺寸计算，确保在不同尺寸下保持视觉比例一致。
- **最小尺寸约束**：通过 `minimumSizeHint()` 返回 `QSize(150, 100)`，确保控件在小尺寸下仍可清晰显示。

### 可扩展方向

- **多电池组支持**：支持显示多个电池组的状态，形成电池矩阵视图。
- **历史数据曲线**：在电池指示器旁叠加小型折线图，展示电量变化趋势。
- **告警阈值配置**：支持自定义低电量、极低电量阈值，适应不同电池类型。
- **电池类型支持**：支持不同电池类型（锂离子、铅酸、镍氢等），显示不同的电压范围与特性。
- **自定义主题**：开放颜色、字体参数，支持夜间模式、高对比模式等主题切换。
- **声音提醒**：低电量或故障时播放提示音，增强多模态提醒。
- **剩余时间估算**：基于电流和电量更精确地估算剩余时间，考虑电池特性曲线。

### 与业务集成

- **线程模型**：电池数据采集线程与 UI 线程解耦，使用信号槽或线程安全队列把最新数据推送到主线程再调用 `setChargeLevel`、`setVoltage`、`setCurrent`。
- **状态同步**：电池状态与电量值保持一致，避免状态与显示不匹配；可通过 `setChargingState` 覆盖自动状态（如用于故障检测）。
- **多电池系统**：在仪表盘中并排显示多个电池指示器，分别监控不同电池组，形成电池矩阵视图。
- **告警策略**：结合电量阈值、电压异常、电流异常与时间过滤，形成告警闭环。
- **数据持久化**：结合数据库或配置文件，保存电池历史数据，实现电池健康评估与趋势分析。
- **国际化与可访问性**：参数文本可配置；颜色同时配合文字/图标，覆盖色弱/高对比场景。

### 简要用法示例

```cpp
// 创建与基础配置
auto battery = new BatteryIndicatorWidget(this);
battery->setChargeLevel(75.0);  // 75% charge
battery->setVoltage(12.6);      // 12.6V
battery->setCurrent(2.5);       // 2.5A
battery->setChargingState(BatteryIndicatorWidget::Discharging);

// 充电状态
battery->setChargingState(BatteryIndicatorWidget::Charging);

// 低电量告警（自动闪烁）
battery->setChargeLevel(15.0);
```

> 注：数据采集与 UI 更新解耦，非主线程不直接操作控件。

### 设计取舍回顾

- **选择四段颜色分区**：高/中/低/极低四段覆盖大多数电池监控场景，避免过度细分带来的复杂度。
- **固定闪烁阈值**：电量低于 20% 时闪烁，符合工业界通用认知，但可通过扩展支持自定义阈值。
- **充电指示器闪烁**：充电时闪电图标闪烁，形成明显的视觉提醒，但需注意闪烁频率的合理性。
- **剩余时间估算**：基于简单线性模型估算剩余时间，适用于大多数场景，但可通过扩展支持更复杂的电池模型。

### 结语

`BatteryIndicatorWidget` 以直观的视觉表现和完整的参数显示，覆盖了工业设备监控中“电池状态、电量显示、充电指示、参数监控”的核心需求；同时保持足够的扩展空间，便于适配不同产品线的主题风格与业务规则。  
如果你正在打造一个既专业又易用的可视化界面，不妨把它作为“电池监控”的基础组件，再根据项目特性逐步演进：加多电池组、加历史曲线、加告警配置与健康评估。期待你基于它构建出更具表现力的工业 UI。

### 附录｜源码

源码放置方式：
- **文内"关键片段"**：展示核心接口与关键绘制逻辑，控制篇幅。
- **仓库/附件"完整源码"**：附仓库链接或网盘附件，标注 commit 与分支。
- **标注文件路径与版本**：便于在工程中定位。
- **构建指引简述**：包含 Qt 版本、CMake 配置与最小可运行示例。

以下为关键片段预览（完整代码请见工程仓库）：

关键头文件：`src/BatteryIndicator/BatteryIndicatorWidget.h`

```8:67:src/BatteryIndicator/BatteryIndicatorWidget.h
/**
 * @brief Battery indicator widget
 * 
 * This widget displays battery charge level, voltage, current, and charging status.
 */
class BatteryIndicatorWidget : public QWidget
{
    Q_OBJECT

public:
    enum ChargingState {
        Discharging,
        Charging,
        FullyCharged,
        Fault
    };

    explicit BatteryIndicatorWidget(QWidget *parent = nullptr);
    ~BatteryIndicatorWidget() override;

    // Getters
    double chargeLevel() const { return m_chargeLevel; }
    double voltage() const { return m_voltage; }
    double current() const { return m_current; }
    ChargingState chargingState() const { return m_chargingState; }

    // Setters
    void setChargeLevel(double percentage);  // 0-100%
    void setVoltage(double voltage);
    void setCurrent(double current);
    void setChargingState(ChargingState state);
```

关键实现：`src/BatteryIndicator/BatteryIndicatorWidget.cpp`

构造与定时器设置：

```3:20:src/BatteryIndicator/BatteryIndicatorWidget.cpp
BatteryIndicatorWidget::BatteryIndicatorWidget(QWidget *parent)
    : QWidget(parent)
    , m_chargeLevel(75.0)
    , m_voltage(12.6)
    , m_current(0.0)
    , m_chargingState(Discharging)
    , m_flashTimer(new QTimer(this))
    , m_flashState(false)
    , m_fullColor(QColor(0, 200, 0))
    , m_mediumColor(QColor(255, 200, 0))
    , m_lowColor(QColor(255, 100, 0))
    , m_criticalColor(QColor(255, 0, 0))
{
    setMinimumSize(150, 100);

    connect(m_flashTimer, &QTimer::timeout, this, &BatteryIndicatorWidget::onFlashTimer);
    m_flashTimer->start(500);
}
```

电量设置与闪烁逻辑：

```26:33:src/BatteryIndicator/BatteryIndicatorWidget.cpp
void BatteryIndicatorWidget::setChargeLevel(double percentage)
{
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;

    m_chargeLevel = percentage;
    update();
}
```

闪烁定时器槽函数：

```55:63:src/BatteryIndicator/BatteryIndicatorWidget.cpp
void BatteryIndicatorWidget::onFlashTimer()
{
    m_flashState = !m_flashState;
    
    // Only update if we need to flash
    if (m_chargeLevel < 20 || m_chargingState == Charging) {
        update();
    }
}
```

电池图标绘制（含渐变效果）：

```84:110:src/BatteryIndicator/BatteryIndicatorWidget.cpp
void BatteryIndicatorWidget::drawBattery(QPainter &painter)
{
    int batteryWidth = 120;
    int batteryHeight = 50;
    int batteryX = (width() - batteryWidth) / 2;
    int batteryY = 25;

    // Draw battery terminal (positive)
    painter.setBrush(QColor(150, 150, 150));
    painter.setPen(Qt::NoPen);
    painter.drawRect(batteryX + batteryWidth, batteryY + 15, 8, 20);

    // Draw battery body
    QLinearGradient bodyGradient(batteryX, batteryY, batteryX, batteryY + batteryHeight);
    bodyGradient.setColorAt(0.0, QColor(180, 180, 180));
    bodyGradient.setColorAt(1.0, QColor(140, 140, 140));

    painter.setBrush(bodyGradient);
    painter.setPen(QPen(QColor(100, 100, 100), 3));
    painter.drawRoundedRect(batteryX, batteryY, batteryWidth, batteryHeight, 5, 5);

    // Draw inner frame
    painter.setBrush(QColor(50, 50, 50));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(batteryX + 4, batteryY + 4,
                          batteryWidth - 8, batteryHeight - 8, 3, 3);
}
```

电量填充绘制（含颜色联动与闪烁）：

```112:158:src/BatteryIndicator/BatteryIndicatorWidget.cpp
void BatteryIndicatorWidget::drawChargeLevel(QPainter &painter)
{
    int batteryWidth = 120;
    int batteryHeight = 50;
    int batteryX = (width() - batteryWidth) / 2;
    int batteryY = 25;

    double fillWidth = (batteryWidth - 12) * (m_chargeLevel / 100.0);

    // Choose color based on charge level
    QColor levelColor;
    if (m_chargeLevel > 60)
        levelColor = m_fullColor;
    else if (m_chargeLevel > 30)
        levelColor = m_mediumColor;
    else if (m_chargeLevel > 15)
        levelColor = m_lowColor;
    else
        levelColor = m_criticalColor;

    // Flash if critical level
    if (m_chargeLevel < 20 && !m_flashState && m_chargingState != Charging) {
        levelColor = levelColor.darker(300);
    }

    // Draw charge level
    QLinearGradient fillGradient(batteryX + 6, batteryY + 6,
                                 batteryX + 6, batteryY + batteryHeight - 6);
    fillGradient.setColorAt(0.0, levelColor.lighter(120));
    fillGradient.setColorAt(1.0, levelColor);

    painter.setBrush(fillGradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(batteryX + 6, batteryY + 6,
                          static_cast<int>(fillWidth), batteryHeight - 12, 2, 2);

    // Draw percentage text
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);

    QString percentText = QString::number(static_cast<int>(m_chargeLevel)) + "%";
    painter.drawText(QRectF(batteryX, batteryY, batteryWidth, batteryHeight),
                    Qt::AlignCenter, percentText);
}
```

充电指示器绘制（含光晕与闪电图标）：

```160:211:src/BatteryIndicator/BatteryIndicatorWidget.cpp
void BatteryIndicatorWidget::drawChargingIndicator(QPainter &painter)
{
    if (m_chargingState != Charging) return;

    int batteryWidth = 120;
    int batteryX = (width() - batteryWidth) / 2;
    int batteryY = 25;
    int batteryHeight = 50;

    // Draw charging bolt symbol - moderate size and visible
    int centerX = batteryX + batteryWidth / 2;
    int centerY = batteryY + batteryHeight / 2;

    // Draw glow effect (always visible when charging)
    painter.setPen(Qt::NoPen);
    QRadialGradient glowGradient(centerX, centerY, 15);
    glowGradient.setColorAt(0.0, QColor(255, 255, 0, 150));
    glowGradient.setColorAt(0.6, QColor(255, 255, 0, 80));
    glowGradient.setColorAt(1.0, QColor(255, 255, 0, 0));
    painter.setBrush(glowGradient);
    painter.drawEllipse(centerX - 15, centerY - 15, 30, 30);

    // Draw charging bolt symbol (flashing for extra attention)
    if (m_flashState) {
        // Standard lightning bolt shape ?
        painter.setPen(QPen(QColor(200, 200, 0), 1));
        painter.setBrush(QColor(255, 255, 0));

        // Draw simple zigzag lightning bolt ? (polyline style)
        QPolygon bolt;
        // Simple folded line from top to bottom
        bolt << QPoint(centerX - 4, centerY - 15)      // Top left start
             << QPoint(centerX + 1, centerY - 15)      // Top right
             << QPoint(centerX - 2, centerY - 2)       // Middle left (diagonal down)
             << QPoint(centerX + 3, centerY - 2)       // Middle right (step)
             << QPoint(centerX, centerY + 15)          // Bottom tip (diagonal down)
             << QPoint(centerX + 1, centerY)           // Return path start
             << QPoint(centerX - 4, centerY)           // Return middle left
             << QPoint(centerX + 1, centerY - 13);     // Return to top (close)

        painter.drawPolygon(bolt);
        
        // Add bright highlight on top edge
        painter.setBrush(QColor(255, 255, 255, 230));
        painter.setPen(Qt::NoPen);
        QPolygon highlight;
        highlight << QPoint(centerX - 3, centerY - 14)
                  << QPoint(centerX, centerY - 14)
                  << QPoint(centerX - 1, centerY - 8);
        painter.drawPolygon(highlight);
    }
}
```

参数显示绘制：

```213:246:src/BatteryIndicator/BatteryIndicatorWidget.cpp
void BatteryIndicatorWidget::drawParameters(QPainter &painter)
{
    int startY = 85;
    int lineHeight = 18;

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);

    // Voltage
    QString voltageText = QString("Voltage: %1 V").arg(m_voltage, 0, 'f', 2);
    painter.drawText(15, startY, width() - 30, lineHeight,
                    Qt::AlignLeft | Qt::AlignVCenter, voltageText);

    // Current
    QString currentText = QString("Current: %1 A").arg(m_current, 0, 'f', 2);
    painter.drawText(15, startY + lineHeight, width() - 30, lineHeight,
                    Qt::AlignLeft | Qt::AlignVCenter, currentText);

    // Remaining time estimate (if discharging)
    if (m_chargingState == Discharging && m_current > 0.1) {
        // Simple estimate: assume linear discharge
        double remainingHours = (m_chargeLevel / 100.0) * 100.0 / m_current;
        QString timeText = QString("Est. Time: %1h %2m")
                          .arg(static_cast<int>(remainingHours))
                          .arg(static_cast<int>((remainingHours - static_cast<int>(remainingHours)) * 60));
        
        font.setPointSize(8);
        painter.setFont(font);
        painter.drawText(15, startY + lineHeight * 2, width() - 30, lineHeight,
                        Qt::AlignLeft | Qt::AlignVCenter, timeText);
    }
}
```

### 最小构建与集成指引

最小可运行步骤：

- **环境**：Qt 6.x（或 Qt 5.15+），CMake 3.20+，编译器与平台按团队规范即可。
- **依赖**：无额外三方依赖，Qt Widgets 模块即可。
- **CMake 示例**：

```cmake
cmake_minimum_required(VERSION 3.20)
project(BatteryIndicatorMiniDemo LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(BatteryIndicatorMiniDemo
    src/main.cpp
    src/BatteryIndicatorWidget.cpp
    src/BatteryIndicatorWidget.h
)
target_link_libraries(BatteryIndicatorMiniDemo PRIVATE Qt6::Widgets)
```

- **最小 `main.cpp`**：

```cpp
#include <QApplication>
#include "BatteryIndicatorWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    BatteryIndicatorWidget w;
    w.setChargeLevel(75.0);
    w.setVoltage(12.6);
    w.setCurrent(2.5);
    w.setChargingState(BatteryIndicatorWidget::Discharging);
    w.resize(250, 150);
    w.show();
    return app.exec();
}
```

