### Qt工业控件实战·第02篇｜Thermometer 温度计控件的设计与实践

### 写在前面

温度监控是工业场景中最基础也最关键的指标之一。从设备运行状态到环境监测，温度数据直接影响生产安全与效率。传统的数字显示虽然精确，但缺乏直观的视觉反馈；而温度计控件以“液柱高度”这一经典隐喻，让操作员能够“一眼看懂”当前温度状态。本文基于本项目的 `ThermometerWidget`，深入解析其设计思路、绘制技巧与工程实践，帮助你在 Qt 应用中快速集成一款专业的工业温度计控件。

<img width="1202" height="832" alt="thermometer" src="https://github.com/user-attachments/assets/ef3253b2-1ec8-483b-81a6-f1a442cf2636" />

### 组件定位与典型场景

- **设备温度监控**：电机、泵、压缩机等旋转设备的实时温度监测，需要快速识别过热风险。
- **环境温度监测**：车间、仓库、实验室等场所的环境温度可视化，支持多区域对比展示。
- **工艺过程控制**：反应釜、烘箱、冷却系统等工艺设备的温度跟踪，结合阈值告警形成闭环。
- **多级告警可视化**：通过颜色分区（正常/警告/危险）直观传达温度安全边界，降低误判风险。

### 视觉结构拆解

`ThermometerWidget` 的画面由六个层次组成：

1. **背景（Background）**：深色背景（RGB 40, 40, 40）提供工业质感，突出前景元素。
2. **温度计管（Tube）**：垂直圆角矩形，采用线性渐变模拟玻璃管的光泽与立体感。
3. **刻度与标签（Scale）**：右侧刻度线，主刻度标注数值，次刻度提供精细读数参考。
4. **液柱（Mercury）**：根据温度值动态填充的液柱，颜色随温度区间变化（蓝/橙/红）。
5. **底部球泡（Bulb）**：圆形球泡采用径向渐变，模拟真实温度计的储液球，增强真实感。
6. **数字读数（Value）**：顶部居中显示当前温度值，格式为 "XX.X°C"，便于精确读取。

这种“由外到内、由静到动”的层级设计既保留了传统温度计的物理隐喻，又融入了现代工业界面的数字化表达。

### API 设计与可用性

`ThermometerWidget` 提供了简洁直观的接口：

- **温度设置**：`setTemperature(double temp)` - 设置当前温度值，自动钳制到合法范围。
- **量程配置**：`setRange(double minTemp, double maxTemp)` - 定义温度显示范围。
- **告警分界**：`setWarningRange(double warningTemp, double criticalTemp)` - 设置警告与危险温度阈值。

核心原则是“先定义范围，再设置数值”；内部会在 `setTemperature` 时自动验证并触发重绘，确保显示始终在有效区间内。

### 液柱高度映射与颜色联动

- 液柱高度计算：温度比例 \(p = (temp - minTemp) / (maxTemp - minTemp)\) 线性映射到管高 \(h = p \times tubeHeight\)。
- 液柱起始位置：从底部球泡顶部开始向上填充，确保液柱与球泡无缝连接。
- 颜色分区逻辑：
  - 正常区间（< warningTemp）：蓝色（RGB 0, 180, 255），传达“安全、正常”的语义。
  - 警告区间（warningTemp ≤ temp < criticalTemp）：橙色（RGB 255, 165, 0），提示“注意、需关注”。
  - 危险区间（≥ criticalTemp）：红色（RGB 255, 0, 0），警示“危险、需立即处理”。

这种映射策略适用于线性温度量程；若需支持非线性映射（如对数刻度），可在计算比例时替换为自定义函数。

### 细节打磨：绘制技巧与真实感

- **圆角矩形管**：使用 `drawRoundedRect` 绘制温度计管，圆角半径设为管宽的一半，形成流畅的视觉边界。
- **线性渐变管身**：从左侧（RGB 200, 200, 200）到中间（RGB 240, 240, 240）再到右侧（RGB 200, 200, 200）的线性渐变，模拟玻璃管的反光效果。
- **径向渐变球泡**：球泡采用径向渐变，中心亮（RGB 220, 220, 220）到边缘暗（RGB 140, 140, 140），增强立体感。
- **液柱与球泡连接**：使用 `QPainterPath` 将液柱矩形与球泡椭圆合并为单一路径，确保无缝连接，避免视觉断层。
- **刻度线层次**：主刻度（偶数段）长线 + 数值标签，次刻度（奇数段）短线，形成清晰的视觉节奏。

### 性能与适配

- **抗锯齿**：启用 `QPainter::Antialiasing`，在圆角、椭圆与渐变绘制下保持平滑边缘。
- **尺寸自适应**：管宽、球泡半径、刻度位置均基于控件宽高比例计算，确保在不同尺寸下保持视觉比例一致。
- **增量重绘**：仅在温度变化或参数更新时调用 `update()`，避免无谓重绘；这对高频数据更新场景尤为重要。
- **最小尺寸约束**：通过 `minimumSizeHint()` 返回 `QSize(60, 150)`，确保控件在小尺寸下仍可清晰显示。

### 可扩展方向

- **水平温度计**：将垂直布局改为水平布局，适用于横向空间受限的场景。
- **双温度显示**：支持同时显示目标温度与当前温度，用不同颜色或虚线标记目标值。
- **历史温度曲线**：在温度计旁叠加小型折线图，展示温度变化趋势。
- **自定义主题**：开放颜色、渐变参数，支持夜间模式、高对比模式等主题切换。
- **动画过渡**：在 `setTemperature` 时通过 `QPropertyAnimation` 实现液柱上升/下降的缓动效果，提升“物理感”。
- **交互提示**：悬停显示详细温度信息，点击阈值条目弹出配置对话框。

### 与业务集成

- **线程模型**：温度采集线程与 UI 线程解耦，使用信号槽或线程安全队列把最新温度值推送到主线程再调用 `setTemperature`。
- **告警策略**：颜色分区对应状态表达；阈值变更、时间过滤（持续超限）与历史记录用于形成告警闭环。
- **多温度计组合**：在仪表盘中并排显示多个温度计，分别监控不同设备或区域，形成温度矩阵视图。
- **国际化与可访问性**：温度单位（°C/°F）可配置；颜色同时配合文字/图标，覆盖色弱/高对比场景。

### 简要用法示例

```cpp
// 创建与基础配置
auto thermometer = new ThermometerWidget(this);
thermometer->setRange(-20.0, 120.0);
thermometer->setWarningRange(80.0, 100.0);

// 周期更新温度（示意）
// 可在定时器或数据回调中调用：
thermometer->setTemperature(currentTemp);
```

> 注：数据采集与 UI 更新解耦，非主线程不直接操作控件。

### 设计取舍回顾

- **选择垂直布局**：相比水平布局，垂直温度计更符合“液柱上升”的物理直觉，且占用宽度更小，便于在仪表盘中密集排列。
- **固定管宽比例**：管宽设为控件宽度的 1/4，在保证可读性的同时留出刻度标注空间，避免拥挤。
- **三段颜色分区**：比渐变色带更具“区间”语义，在监控场景中更不易误读；颜色选择遵循工业界通用认知。
- **球泡与液柱合并绘制**：使用 `QPainterPath` 确保视觉连续性，避免因分层绘制导致的接缝问题。

### 结语

`ThermometerWidget` 以直观的物理隐喻和清晰的视觉层次，覆盖了工业监控中“看温度、看状态”的核心需求；同时保持足够的扩展空间，便于适配不同产品线的主题风格与业务规则。  
如果你正在打造一个既专业又易用的可视化界面，不妨把它作为“温度监控”的基础组件，再根据项目特性逐步演进：加动画、加交互、加多温度对比与历史趋势。期待你基于它构建出更具表现力的工业 UI。

### 附录｜源码

源码放置方式：
- **文内"关键片段"**：展示核心接口与关键绘制逻辑，控制篇幅。
- **仓库/附件"完整源码"**：附仓库链接或网盘附件，标注 commit 与分支。
- **标注文件路径与版本**：便于在工程中定位。
- **构建指引简述**：包含 Qt 版本、CMake 配置与最小可运行示例。

以下为关键片段预览（完整代码请见工程仓库）：

关键头文件：`src/Thermometer/ThermometerWidget.h`

```8:52:src/Thermometer/ThermometerWidget.h
/**
 * @brief Industrial style thermometer widget
 * 
 * This widget displays a vertical thermometer with customizable temperature
 * range and visual appearance suitable for industrial monitoring.
 */
class ThermometerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ThermometerWidget(QWidget *parent = nullptr);
    ~ThermometerWidget() override;

    // Getters
    double temperature() const { return m_temperature; }
    double minTemperature() const { return m_minTemp; }
    double maxTemperature() const { return m_maxTemp; }

    // Setters
    void setTemperature(double temp);
    void setRange(double minTemp, double maxTemp);
    void setWarningRange(double warningTemp, double criticalTemp);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(100, 300); }
    QSize minimumSizeHint() const override { return QSize(60, 150); }

private:
    void drawBackground(QPainter &painter);
    void drawTube(QPainter &painter);
    void drawMercury(QPainter &painter);
    void drawBulb(QPainter &painter);
    void drawScale(QPainter &painter);
    void drawValue(QPainter &painter);

    double m_temperature;
    double m_minTemp;
    double m_maxTemp;
    double m_warningTemp;
    double m_criticalTemp;

    QColor m_normalColor;
    QColor m_warningColor;
    QColor m_criticalColor;
};
```

关键实现：`src/Thermometer/ThermometerWidget.cpp`

构造与基础参数：

```5:17:src/Thermometer/ThermometerWidget.cpp
ThermometerWidget::ThermometerWidget(QWidget *parent)
    : QWidget(parent)
    , m_temperature(20.0)
    , m_minTemp(-20.0)
    , m_maxTemp(120.0)
    , m_warningTemp(80.0)
    , m_criticalTemp(100.0)
    , m_normalColor(QColor(0, 180, 255))
    , m_warningColor(QColor(255, 165, 0))
    , m_criticalColor(QColor(255, 0, 0))
{
    setMinimumSize(60, 150);
}
```

温度设置与重绘：

```23:34:src/Thermometer/ThermometerWidget.cpp
void ThermometerWidget::setTemperature(double temp)
{
    if (temp < m_minTemp)
        temp = m_minTemp;
    if (temp > m_maxTemp)
        temp = m_maxTemp;

    if (qAbs(m_temperature - temp) > 0.001) {
        m_temperature = temp;
        update();
    }
}
```

温度计管绘制（含渐变效果）：

```72:89:src/Thermometer/ThermometerWidget.cpp
void ThermometerWidget::drawTube(QPainter &painter)
{
    int tubeWidth = width() / 4;
    int tubeHeight = height() - height() / 5;
    int tubeX = width() / 2 - tubeWidth / 2;
    int tubeY = height() / 10;

    // Draw tube background
    QLinearGradient gradient(tubeX, 0, tubeX + tubeWidth, 0);
    gradient.setColorAt(0.0, QColor(200, 200, 200));
    gradient.setColorAt(0.5, QColor(240, 240, 240));
    gradient.setColorAt(1.0, QColor(200, 200, 200));

    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(120, 120, 120), 2));
    painter.drawRoundedRect(tubeX, tubeY, tubeWidth, tubeHeight, 
                           tubeWidth / 2, tubeWidth / 2);
}
```

液柱绘制（含颜色联动与球泡连接）：

```91:128:src/Thermometer/ThermometerWidget.cpp
void ThermometerWidget::drawMercury(QPainter &painter)
{
    int tubeWidth = width() / 4;
    int tubeHeight = height() - height() / 5;
    int tubeX = width() / 2 - tubeWidth / 2;
    int tubeY = height() / 10;
    int bulbRadius = width() / 3;

    // Calculate mercury height based on temperature
    double percentage = (m_temperature - m_minTemp) / (m_maxTemp - m_minTemp);
    int mercuryHeight = static_cast<int>(percentage * tubeHeight);
    int mercuryY = tubeY + tubeHeight - mercuryHeight;

    // Choose color based on temperature
    QColor mercuryColor;
    if (m_temperature >= m_criticalTemp)
        mercuryColor = m_criticalColor;
    else if (m_temperature >= m_warningTemp)
        mercuryColor = m_warningColor;
    else
        mercuryColor = m_normalColor;

    // Draw mercury in tube
    painter.setBrush(mercuryColor);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    int mercuryWidth = tubeWidth - 8;
    path.addRoundedRect(tubeX + 4, mercuryY, mercuryWidth, mercuryHeight + 4,
                       mercuryWidth / 2, mercuryWidth / 2);
    
    // Connect to bulb
    path.addEllipse(width() / 2 - bulbRadius / 2, 
                   height() - bulbRadius - 5,
                   bulbRadius, bulbRadius);

    painter.drawPath(path);
}
```

球泡绘制（径向渐变）：

```130:145:src/Thermometer/ThermometerWidget.cpp
void ThermometerWidget::drawBulb(QPainter &painter)
{
    int bulbRadius = width() / 3;
    int bulbX = width() / 2 - bulbRadius / 2;
    int bulbY = height() - bulbRadius - 5;

    // Draw bulb outline
    QRadialGradient gradient(width() / 2, bulbY + bulbRadius / 2, bulbRadius / 2);
    gradient.setColorAt(0.0, QColor(220, 220, 220));
    gradient.setColorAt(0.7, QColor(180, 180, 180));
    gradient.setColorAt(1.0, QColor(140, 140, 140));

    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(120, 120, 120), 2));
    painter.drawEllipse(bulbX, bulbY, bulbRadius, bulbRadius);
}
```

刻度绘制：

```147:174:src/Thermometer/ThermometerWidget.cpp
void ThermometerWidget::drawScale(QPainter &painter)
{
    int tubeHeight = height() - height() / 5;
    int tubeY = height() / 10;
    int tubeWidth = width() / 4;
    int tubeRight = width() / 2 + tubeWidth / 2;

    painter.setPen(QPen(Qt::white, 1));
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    int numMarks = 10;
    for (int i = 0; i <= numMarks; ++i) {
        int y = tubeY + tubeHeight - (i * tubeHeight / numMarks);
        double temp = m_minTemp + (m_maxTemp - m_minTemp) * i / numMarks;

        // Draw major marks
        if (i % 2 == 0) {
            painter.drawLine(tubeRight, y, tubeRight + 10, y);
            painter.drawText(tubeRight + 12, y - 8, 40, 16, 
                           Qt::AlignLeft | Qt::AlignVCenter,
                           QString::number(static_cast<int>(temp)));
        } else {
            painter.drawLine(tubeRight, y, tubeRight + 5, y);
        }
    }
}
```

数字读数：

```176:186:src/Thermometer/ThermometerWidget.cpp
void ThermometerWidget::drawValue(QPainter &painter)
{
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);

    QString valueText = QString::number(m_temperature, 'f', 1) + QString::fromUtf8("\xC2\xB0") + "C";
    painter.drawText(QRectF(0, 5, width(), 20), Qt::AlignCenter, valueText);
}
```

### 最小构建与集成指引

最小可运行步骤：

- **环境**：Qt 6.x（或 Qt 5.15+），CMake 3.20+，编译器与平台按团队规范即可。
- **依赖**：无额外三方依赖，Qt Widgets 模块即可。
- **CMake 示例**：

```cmake
cmake_minimum_required(VERSION 3.20)
project(ThermometerMiniDemo LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(ThermometerMiniDemo
    src/main.cpp
    src/ThermometerWidget.cpp
    src/ThermometerWidget.h
)
target_link_libraries(ThermometerMiniDemo PRIVATE Qt6::Widgets)
```

- **最小 `main.cpp`**：

```cpp
#include <QApplication>
#include "ThermometerWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ThermometerWidget w;
    w.setRange(-20, 120);
    w.setWarningRange(80, 100);
    w.setTemperature(25.5);
    w.resize(120, 400);
    w.show();
    return app.exec();
}
```
