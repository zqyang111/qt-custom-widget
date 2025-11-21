### Qt工业控件实战·第03篇｜Valve 阀门控件的设计与实践

### 写在前面

在工业过程控制系统中，阀门是调节流体流量、压力与流向的关键设备。从管道输送、反应釜控制到冷却系统，阀门的开闭状态直接影响工艺流程的安全与效率。传统的文本或图标显示虽然简洁，但缺乏直观的视觉反馈；而阀门控件以“阀门本体+管道+开度指示”这一经典工业隐喻，让操作员能够“一眼看懂”当前阀门状态与开度。本文基于本项目的 `ValveWidget`，深入解析其设计思路、绘制技巧与工程实践，帮助你在 Qt 应用中快速集成一款专业的工业阀门控件。

<img width="1202" height="832" alt="valve_control" src="https://github.com/user-attachments/assets/5cf3ef30-c41c-4588-a03d-faf80f63fa8c" />

### 组件定位与典型场景

- **管道流量控制**：调节管道中流体（水、气、油等）的流量，通过开度百分比精确控制。
- **工艺过程控制**：反应釜、蒸馏塔、冷却系统等工艺设备的进料/出料阀门监控，结合自动化控制形成闭环。
- **多阀门系统监控**：在仪表盘中并排显示多个阀门，分别监控不同管道或设备，形成阀门矩阵视图。
- **状态可视化**：通过颜色与指示器直观传达阀门状态（关闭/开启中/开启/关闭中），降低误判风险。

### 视觉结构拆解

`ValveWidget` 的画面由四个层次组成：

1. **管道（Pipes）**：上下两条管道，颜色随阀门开度变化（红色/橙色/绿色），模拟流体状态。
2. **阀门本体（Valve Body）**：根据阀门类型（闸阀/球阀/蝶阀）绘制不同的几何形状，采用渐变效果增强立体感。
3. **开度指示器（Indicator）**：根据阀门类型显示不同的开度指示方式（闸阀为垂直滑块，球阀/蝶阀为旋转角度）。
4. **状态文本（Status）**：顶部显示状态文字（CLOSED/OPENING/OPEN/CLOSING），底部显示开度百分比。

这种“由外到内、由静到动”的层级设计既保留了传统工业阀门的物理隐喻，又融入了现代界面的数字化表达。

### API 设计与可用性

`ValveWidget` 提供了简洁直观的接口：

- **开度设置**：`setOpening(double percentage)` - 设置阀门开度（0-100%），自动更新状态并触发重绘。
- **阀门类型**：`setType(ValveType type)` - 设置阀门类型（GateValve/BallValve/ButterflyValve），影响绘制样式。
- **状态设置**：`setState(ValveState state)` - 直接设置阀门状态（Closed/Opening/Open/Closing），用于动画或外部控制。

核心原则是“先定义类型，再设置开度”；内部会在 `setOpening` 时自动计算状态并更新显示，确保状态与开度的一致性。

### 阀门类型与绘制策略

`ValveWidget` 支持三种常见的工业阀门类型：

1. **闸阀（GateValve）**：
   - 本体形状：菱形（钻石形），采用线性渐变模拟金属质感。
   - 开度指示：垂直滑块，从顶部（关闭）到底部（开启）移动。
   - 适用场景：全开/全关控制，不适合频繁调节。

2. **球阀（BallValve）**：
   - 本体形状：圆形，采用径向渐变模拟球体立体感。
   - 开度指示：旋转线条，0-90度旋转表示开度。
   - 适用场景：快速开关，适合频繁操作。

3. **蝶阀（ButterflyValve）**：
   - 本体形状：圆角矩形，采用线性渐变模拟阀板。
   - 开度指示：椭圆形阀板旋转，0-90度旋转表示开度。
   - 适用场景：大流量调节，成本低、体积小。

### 开度映射与颜色联动

- 开度计算：开度百分比 \(p = opening / 100.0\) 线性映射到指示器位置或角度。
- 管道颜色逻辑：
  - 关闭状态（opening = 0%）：红色（RGB 200, 0, 0），传达“阻断、停止”的语义。
  - 部分开启（0% < opening ≤ 50%）：橙色（RGB 255, 165, 0），提示“调节中、注意”。
  - 开启状态（opening > 50%）：绿色（RGB 0, 200, 0），表示“流通、正常”。
- 状态自动更新：根据开度自动设置状态（0%→Closed, 100%→Open, 其他→Opening）。

这种映射策略适用于线性开度控制；若需支持非线性映射（如对数响应），可在计算时替换为自定义函数。

### 细节打磨：绘制技巧与真实感

- **渐变效果**：阀门本体采用线性或径向渐变，从中心亮到边缘暗，增强立体感与金属质感。
- **管道渐变**：管道采用线性渐变，模拟圆柱形管道的反光效果，左右两侧较暗、中间较亮。
- **指示器颜色**：开度指示器采用黄色（RGB 255, 255, 0），在深色背景下形成高对比度，便于识别。
- **状态文本**：顶部状态文字采用粗体，底部开度百分比采用加粗字体，确保信息清晰可读。
- **抗锯齿**：启用 `QPainter::Antialiasing`，在圆角、椭圆与渐变绘制下保持平滑边缘。

### 性能与适配

- **抗锯齿**：启用 `QPainter::Antialiasing`，在复杂几何形状与渐变绘制下保持平滑边缘。
- **尺寸自适应**：阀门大小、管道宽度、指示器位置均基于控件宽高比例计算，确保在不同尺寸下保持视觉比例一致。
- **增量重绘**：仅在开度变化或参数更新时调用 `update()`，避免无谓重绘；这对实时数据更新场景尤为重要。
- **最小尺寸约束**：通过 `minimumSizeHint()` 返回 `QSize(60, 80)`，确保控件在小尺寸下仍可清晰显示。

### 可扩展方向

- **动画过渡**：在 `setOpening` 时通过 `QPropertyAnimation` 实现开度变化的缓动效果，提升“物理感”。
- **流体动画**：在管道中绘制流动粒子或波纹效果，模拟流体流动，增强真实感。
- **多阀门组合**：支持水平/垂直排列的阀门组，用于复杂管道系统的可视化。
- **自定义主题**：开放颜色、渐变参数，支持夜间模式、高对比模式等主题切换。
- **交互控制**：支持鼠标拖拽调节开度，或点击切换全开/全关状态。
- **压力/流量显示**：在阀门旁叠加压力或流量数值，形成更完整的过程监控视图。

### 与业务集成

- **线程模型**：阀门控制线程与 UI 线程解耦，使用信号槽或线程安全队列把最新开度值推送到主线程再调用 `setOpening`。
- **状态同步**：阀门状态与开度值保持一致，避免状态与显示不匹配；可通过 `setState` 覆盖自动状态（如用于动画）。
- **多阀门系统**：在仪表盘中并排显示多个阀门，分别监控不同管道或设备，形成阀门矩阵视图。
- **告警策略**：结合开度阈值（如开度异常、卡死检测）与时间过滤，形成告警闭环。
- **国际化与可访问性**：状态文本可配置；颜色同时配合文字/图标，覆盖色弱/高对比场景。

### 简要用法示例

```cpp
// 创建与基础配置
auto valve = new ValveWidget(this);
valve->setType(ValveWidget::BallValve);
valve->setOpening(75.0);  // 75% open

// 周期更新开度（示意）
// 可在定时器或数据回调中调用：
valve->setOpening(currentOpening);
```

> 注：数据采集与 UI 更新解耦，非主线程不直接操作控件。

### 设计取舍回顾

- **选择三种阀门类型**：覆盖最常见的工业阀门类型，满足不同场景需求；类型切换通过枚举实现，便于扩展。
- **管道颜色联动**：管道颜色随开度变化，比固定颜色更具信息量，但需注意颜色语义的一致性。
- **状态自动计算**：根据开度自动计算状态，简化 API 使用，但允许通过 `setState` 覆盖（用于动画）。
- **指示器差异化**：不同阀门类型采用不同的指示方式，更符合实际物理特性，但增加了绘制复杂度。

### 结语

`ValveWidget` 以直观的工业隐喻和清晰的视觉层次，覆盖了过程控制中“看阀门、看开度、看状态”的核心需求；同时保持足够的扩展空间，便于适配不同产品线的主题风格与业务规则。  
如果你正在打造一个既专业又易用的可视化界面，不妨把它作为“阀门监控”的基础组件，再根据项目特性逐步演进：加动画、加交互、加多阀门组合与流体效果。期待你基于它构建出更具表现力的工业 UI。

### 附录｜源码

源码放置方式：
- **文内"关键片段"**：展示核心接口与关键绘制逻辑，控制篇幅。
- **仓库/附件"完整源码"**：附仓库链接或网盘附件，标注 commit 与分支。
- **标注文件路径与版本**：便于在工程中定位。
- **构建指引简述**：包含 Qt 版本、CMake 配置与最小可运行示例。

以下为关键片段预览（完整代码请见工程仓库）：

关键头文件：`src/Valve/ValveWidget.h`

```8:62:src/Valve/ValveWidget.h
/**
 * @brief Industrial valve widget
 * 
 * This widget displays a valve with open/closed states and opening percentage,
 * suitable for process control visualization.
 */
class ValveWidget : public QWidget
{
    Q_OBJECT

public:
    enum ValveType {
        GateValve,
        BallValve,
        ButterflyValve
    };

    enum ValveState {
        Closed,
        Opening,
        Open,
        Closing
    };

    explicit ValveWidget(QWidget *parent = nullptr);
    ~ValveWidget() override;

    // Getters
    double opening() const { return m_opening; }
    ValveType type() const { return m_type; }
    ValveState state() const { return m_state; }

    // Setters
    void setOpening(double percentage);  // 0-100%
    void setType(ValveType type);
    void setState(ValveState state);
```

关键实现：`src/Valve/ValveWidget.cpp`

构造与基础参数：

```4:14:src/Valve/ValveWidget.cpp
ValveWidget::ValveWidget(QWidget *parent)
    : QWidget(parent)
    , m_opening(0.0)
    , m_type(GateValve)
    , m_state(Closed)
    , m_openColor(QColor(0, 200, 0))
    , m_closedColor(QColor(200, 0, 0))
    , m_bodyColor(QColor(150, 150, 150))
{
    setMinimumSize(60, 80);
}
```

开度设置与状态更新：

```20:40:src/Valve/ValveWidget.cpp
void ValveWidget::setOpening(double percentage)
{
    if (percentage < 0.0)
        percentage = 0.0;
    if (percentage > 100.0)
        percentage = 100.0;

    if (qAbs(m_opening - percentage) > 0.1) {
        m_opening = percentage;
        
        // Update state based on opening
        if (m_opening <= 0.0)
            m_state = Closed;
        else if (m_opening >= 100.0)
            m_state = Open;
        else
            m_state = Opening;
        
        update();
    }
}
```

阀门本体绘制（闸阀示例）：

```73:98:src/Valve/ValveWidget.cpp
void ValveWidget::drawValveBody(QPainter &painter)
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int valveSize = qMin(width(), height()) / 2;

    // Draw valve body based on type
    switch (m_type) {
    case GateValve: {
        // Draw diamond shape for gate valve
        QPolygon valveBody;
        valveBody << QPoint(centerX, centerY - valveSize)
                  << QPoint(centerX + valveSize, centerY)
                  << QPoint(centerX, centerY + valveSize)
                  << QPoint(centerX - valveSize, centerY);

        QLinearGradient gradient(centerX - valveSize, centerY, 
                                centerX + valveSize, centerY);
        gradient.setColorAt(0.0, m_bodyColor.darker(120));
        gradient.setColorAt(0.5, m_bodyColor);
        gradient.setColorAt(1.0, m_bodyColor.darker(120));

        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(80, 80, 80), 2));
        painter.drawPolygon(valveBody);
        break;
    }
```

开度指示器绘制（球阀示例）：

```151:162:src/Valve/ValveWidget.cpp
    case BallValve: {
        // Draw ball rotation indicator
        painter.save();
        painter.translate(centerX, centerY);
        double angle = (m_opening / 100.0) * 90.0;  // 0-90 degrees
        painter.rotate(angle);

        painter.setPen(QPen(QColor(255, 255, 0), 3));
        painter.drawLine(-valveSize / 2, 0, valveSize / 2, 0);

        painter.restore();
        break;
    }
```

管道绘制（含颜色联动）：

```187:223:src/Valve/ValveWidget.cpp
void ValveWidget::drawPipes(QPainter &painter)
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int pipeWidth = width() / 4;

    // Determine pipe color based on valve opening
    QColor pipeColor;
    if (m_opening > 50.0)
        pipeColor = m_openColor;
    else if (m_opening > 0.0)
        pipeColor = QColor(255, 165, 0);  // Orange for partial opening
    else
        pipeColor = m_closedColor;

    // Top pipe
    QLinearGradient topGradient(centerX - pipeWidth / 2, 0,
                                centerX + pipeWidth / 2, 0);
    topGradient.setColorAt(0.0, pipeColor.darker(150));
    topGradient.setColorAt(0.5, pipeColor);
    topGradient.setColorAt(1.0, pipeColor.darker(150));

    painter.setBrush(topGradient);
    painter.setPen(QPen(QColor(80, 80, 80), 1));
    painter.drawRect(centerX - pipeWidth / 2, 0, pipeWidth, centerY - height() / 4);

    // Bottom pipe
    QLinearGradient bottomGradient(centerX - pipeWidth / 2, 0,
                                   centerX + pipeWidth / 2, 0);
    bottomGradient.setColorAt(0.0, pipeColor.darker(150));
    bottomGradient.setColorAt(0.5, pipeColor);
    bottomGradient.setColorAt(1.0, pipeColor.darker(150));

    painter.setBrush(bottomGradient);
    painter.drawRect(centerX - pipeWidth / 2, centerY + height() / 4,
                    pipeWidth, centerY - height() / 4);
}
```

状态文本绘制：

```225:250:src/Valve/ValveWidget.cpp
void ValveWidget::drawStatus(QPainter &painter)
{
    // Draw opening percentage
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);

    QString statusText = QString::number(m_opening, 'f', 0) + "%";
    painter.drawText(QRectF(0, height() - 20, width(), 18),
                    Qt::AlignCenter, statusText);

    // Draw state indicator
    QString stateText;
    switch (m_state) {
    case Closed:   stateText = "CLOSED"; break;
    case Opening:  stateText = "OPENING"; break;
    case Open:     stateText = "OPEN"; break;
    case Closing:  stateText = "CLOSING"; break;
    }

    font.setPointSize(8);
    painter.setFont(font);
    painter.drawText(QRectF(0, 2, width(), 16), Qt::AlignCenter, stateText);
}
```

### 最小构建与集成指引

最小可运行步骤：

- **环境**：Qt 6.x（或 Qt 5.15+），CMake 3.20+，编译器与平台按团队规范即可。
- **依赖**：无额外三方依赖，Qt Widgets 模块即可。
- **CMake 示例**：

```cmake
cmake_minimum_required(VERSION 3.20)
project(ValveMiniDemo LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(ValveMiniDemo
    src/main.cpp
    src/ValveWidget.cpp
    src/ValveWidget.h
)
target_link_libraries(ValveMiniDemo PRIVATE Qt6::Widgets)
```

- **最小 `main.cpp`**：

```cpp
#include <QApplication>
#include "ValveWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ValveWidget w;
    w.setType(ValveWidget::BallValve);
    w.setOpening(75.0);
    w.resize(120, 200);
    w.show();
    return app.exec();
}
```

