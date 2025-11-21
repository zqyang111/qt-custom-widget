### Qt工业控件实战·第01篇｜Gauge 表盘控件的设计与实践

### 写在前面

在工业监控、大屏可视化与测试测量软件中，圆形刻度表盘是最常见也最易读的视觉控件之一。它以“ glanceable ”的方式传达关键信息：正常、警告、危险。本文基于本项目的 `GaugeWidget`，系统介绍其设计理念、核心实现与扩展思路，帮助你在 Qt 应用中快速落地一款高质感的工业风表盘控件。

<img width="1202" height="832" alt="gauge" src="https://github.com/user-attachments/assets/bd4dce5f-399f-4cda-8ff8-f32e54d7c96c" />

### 组件定位与典型场景

- **实时数值监控**：如转速、压力、温度、电压/电流等，要求在变化过程中保持稳定与顺滑的视觉反馈。
- **阈值分区告警**：以颜色分区（正常/警告/危险）直观传达安全边界，降低误读风险。
- **多端适配**：在不同窗口尺寸下保持清晰度与版式美感，便于集成到复杂仪表盘或工控界面中。

### 视觉结构拆解

`GaugeWidget` 的画面由五个层次组成：

1. **背景（Background）**：外圈径向渐变形成金属/工业质感；统一质感基调，承载后续元素。  
2. **刻度与分区（Scale）**：270° 扇形表盘，起始于左下（约 225°），按范围等比分为 10 段；通过三段弧色标记正常、警告、危险区间。  
3. **指针（Needle）**：依据数值比例旋转，颜色随区间联动；同时绘制细微阴影增强立体层次。  
4. **中心帽（Center Circle）**：小圆点的多级径向渐变，强化结构“轴心”的真实感。  
5. **数字读数（Value）**：在下方以粗体文本显示当前数值与单位（如 ℃、RPM、kPa）。

这种“由外到内”的层级设计能够在保留信息密度的同时保持视觉均衡，且利于后续样式扩展（主题肤色、线宽、字体）。

### API 设计与可用性

`GaugeWidget` 面向最常用的表盘诉求提供了直观的接口：

- **取值与范围**：`setValue(double)`, `setRange(double min, double max)`  
- **单位展示**：`setUnit(const QString&)`  
- **告警分界**：`setWarningRange(double warningStart, double criticalStart)`  

核心原则是“先定义范围，再投射数值”；内部会在 `setValue` 时自动钳制到合法区间并触发重绘，避免越界导致的异常角度。

### 角度映射与分区逻辑

- 表盘弧长为 270°，起始角度约为 225°（左下方向），终止于 225°+270° ≈ 495°（右下）。  
- 数值比例 \(p = (value - min) / (max - min)\) 线性映射到角度 \(θ = 225° + p \times 270°\)。  
- 三段颜色弧通过 `warningStart`、`criticalStart` 把线性区间切分为“绿色/橙色/红色”，并用固定线宽的 `drawArc` 分别绘制。

这种映射策略适用于线性量程；支持在计算角度处替换为自定义映射函数（例如对数/根号映射）。

### 细节打磨：读数与可读性

- **主/次刻度**：每 10 段等分；偶数段绘制主刻度线并标注数字，奇数段为短刻度。这样既提供精确读数（标签），又维持密度与节奏感。  
- **字体与对齐**：数字读数采用加粗字体，位于底部居中区域；单位与数值拼接，避免多控件布局导致的抖动。  
- **色彩语义**：默认采用 “绿/橙/红” 的直觉配色，满足工业告警的一致认知；颜色可替换为品牌/主题色。

### 性能与适配

- **抗锯齿**：启用 `QPainter::Antialiasing`，在旋转与曲线绘制下保持平滑边缘。  
- **分辨率自适应**：以 `side / 300.0` 为统一缩放系数，确保在不同控件尺寸下相对比例一致，避免失真。  
- **增量重绘**：仅在数值变化或参数更新时调用 `update()`，避免无谓重绘；这对实时数据场景尤为重要。

### 可扩展方向

- **非线性刻度**：支持指数或对数映射，提升在极端范围下的可读性。  
- **多指针/多量程**：用于双量程对比（如目标值与当前值）、或双单位并行显示。  
- **自定义主题**：开放线宽、字体、渐变参数，结合样式表或主题管理器构建多皮肤体系。  
- **动画过渡**：在 `setValue` 时通过插值或 `QPropertyAnimation` 实现缓动，提升“物理感”。  
- **交互提示**：悬停高亮、点击阈值条目弹出说明，增强可解释性。

### 与业务集成

- **线程模型**：采集线程与 UI 线程解耦，使用信号槽或线程安全队列把最新数值推送到主线程再调用 `setValue`。  
- **告警策略**：颜色分区对应状态表达；阈值变更、时间过滤（持续超限）与历史记录用于形成告警闭环。  
- **国际化与可访问性**：单位与标签文本集中管理；颜色同时配合文字/图标，覆盖色弱/高对比场景。

### 简要用法示例

```cpp
// 创建与基础配置
auto gauge = new GaugeWidget(this);
gauge->setRange(0.0, 100.0);
gauge->setWarningRange(70.0, 90.0);
gauge->setUnit("RPM");

// 周期更新数值（示意）
// 可在定时器或数据回调中调用：
gauge->setValue(currentRpm);
```

> 注：数据采集与 UI 更新解耦，非主线程不直接操作控件。

### 设计取舍回顾

- **选择 270° 表盘**：相比 360°，顶部留白便于布局标题/图例，读数更聚焦；相比 180°，有更长的刻度弧和更细腻的角度分辨率。  
- **固定线宽的三段弧**：比渐变色带更具“区间”语义，在监控场景中更不易误读。  
- **字体与阴影克制**：以信息可读为先，适当的阴影仅用于层级区分，避免过度装饰。

### 结语

`GaugeWidget` 以简明 API 和清晰视觉规范，覆盖了工业监控中“看数值、看状态”的核心需求；同时保持足够的扩展空间，便于适配不同产品线的主题风格与业务规则。  
如果你正在打造一个既专业又易用的可视化界面，不妨把它作为“基础表盘”的起点，再根据项目特性逐步演进：加动画、加交互、加多量程与历史对比。期待你基于它构建出更具表现力的工业 UI。

### 附录｜源码

源码放置方式：
- **文内“关键片段”**：展示核心接口与关键绘制逻辑，控制篇幅。
- **仓库/附件“完整源码”**：附仓库链接或网盘附件，标注 commit 与分支。
- **标注文件路径与版本**：便于在工程中定位。
- **构建指引简述**：包含 Qt 版本、CMake 配置与最小可运行示例。

以下为关键片段预览（完整代码请见工程仓库）：

关键头文件：`src/Gauge/GaugeWidget.h`

```8:36:src/Gauge/GaugeWidget.h
/**
 * @brief Industrial style gauge widget
 * 
 * This widget displays a circular gauge with customizable range,
 * value, and visual appearance suitable for industrial applications.
 */
class GaugeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GaugeWidget(QWidget *parent = nullptr);
    ~GaugeWidget() override;

    // Getters
    double value() const { return m_value; }
    double minValue() const { return m_minValue; }
    double maxValue() const { return m_maxValue; }
    QString unit() const { return m_unit; }

    // Setters
    void setValue(double value);
    void setRange(double minValue, double maxValue);
    void setUnit(const QString &unit);
    void setWarningRange(double warningStart, double criticalStart);
}
```

关键实现：`src/Gauge/GaugeWidget.cpp`

构造与基础参数：

```5:18:src/Gauge/GaugeWidget.cpp
GaugeWidget::GaugeWidget(QWidget *parent)
    : QWidget(parent)
    , m_value(0.0)
    , m_minValue(0.0)
    , m_maxValue(100.0)
    , m_warningStart(70.0)
    , m_criticalStart(90.0)
    , m_unit("")
    , m_normalColor(QColor(0, 200, 0))
    , m_warningColor(QColor(255, 165, 0))
    , m_criticalColor(QColor(255, 0, 0))
{
    setMinimumSize(150, 150);
}
```

数值设置与重绘：

```24:35:src/Gauge/GaugeWidget.cpp
void GaugeWidget::setValue(double value)
{
    if (value < m_minValue)
        value = m_minValue;
    if (value > m_maxValue)
        value = m_maxValue;

    if (qAbs(m_value - value) > 0.001) {
        m_value = value;
        update();
    }
}
```

刻度分区绘制（含正常/警告/危险三段）：

```92:116:src/Gauge/GaugeWidget.cpp
void GaugeWidget::drawScale(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 300.0, side / 300.0);

    // Draw arc segments with different colors
    double startAngle = 225.0; // Bottom left
    double spanAngle = 270.0;  // 3/4 circle

    // Calculate angles for warning and critical zones
    double range = m_maxValue - m_minValue;
    double warningAngle = ((m_warningStart - m_minValue) / range) * spanAngle;
}
```

指针与颜色联动：

```160:181:src/Gauge/GaugeWidget.cpp
void GaugeWidget::drawNeedle(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 300.0, side / 300.0);

    // Calculate needle angle
    double percentage = (m_value - m_minValue) / (m_maxValue - m_minValue);
    double angle = 225.0 + percentage * 270.0;

    painter.rotate(angle);

    // Choose needle color based on value
    QColor needleColor;
    if (m_value >= m_criticalStart)
        needleColor = m_criticalColor;
    else if (m_value >= m_warningStart)
        needleColor = m_warningColor;
    else
        needleColor = m_normalColor;
}
```

数字读数：

```216:235:src/Gauge/GaugeWidget.cpp
void GaugeWidget::drawValue(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 300.0, side / 300.0);

    // Draw digital value display
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(18);
    font.setBold(true);
    painter.setFont(font);

    QString valueText = QString::number(m_value, 'f', 1);
    if (!m_unit.isEmpty())
        valueText += " " + m_unit;

    painter.drawText(QRectF(-60, 40, 120, 30), Qt::AlignCenter, valueText);
}
```

### 最小构建与集成指引

最小可运行步骤：

- **环境**：Qt 6.x（或 Qt 5.15+），CMake 3.20+，编译器与平台按团队规范即可。
- **依赖**：无额外三方依赖，Qt Widgets 模块即可。
- **CMake 示例**：

```cmake
cmake_minimum_required(VERSION 3.20)
project(GaugeMiniDemo LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(GaugeMiniDemo
    src/main.cpp
    src/GaugeWidget.cpp
    src/GaugeWidget.h
)
target_link_libraries(GaugeMiniDemo PRIVATE Qt6::Widgets)
```

- **最小 `main.cpp`**：

```cpp
#include <QApplication>
#include "GaugeWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GaugeWidget w;
    w.setRange(0, 100);
    w.setWarningRange(70, 90);
    w.setUnit("RPM");
    w.resize(360, 360);
    w.show();
    return app.exec();
}
```
