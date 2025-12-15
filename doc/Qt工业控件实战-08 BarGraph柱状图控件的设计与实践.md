### Qt工业控件实战·第08篇｜BarGraph 柱状图控件的设计与实践

### 写在前面

在工业数据可视化中，柱状图是最常用也最直观的对比展示方式之一。从多参数对比、设备性能评估到产量统计，柱状图以“垂直柱体高度”这一经典视觉隐喻，让操作员能够快速识别不同数据项之间的差异与关系。传统的表格虽然精确，但缺乏直观的视觉对比；而柱状图控件以“多柱体+自动缩放+渐变效果”这一专业设计，在保留数据精确性的同时提升了视觉表现力。本文基于本项目的 `BarGraphWidget`，深入解析其设计思路、绘制技巧与工程实践，帮助你在 Qt 应用中快速集成一款专业的工业柱状图控件。

![](../picture/bar_graph.png)

### 组件定位与典型场景

- **多参数对比**：温度、压力、流量等工艺参数的实时对比，支持多设备、多时间点的数据对比。
- **设备性能评估**：不同设备的效率、产量、能耗等性能指标的对比展示，便于快速识别最优设备。
- **产量统计**：不同生产线、不同班次、不同产品的产量统计，支持按时间段、类别分组展示。
- **质量分析**：合格率、不良率、返工率等质量指标的对比分析，便于发现异常模式。

### 视觉结构拆解

`BarGraphWidget` 的画面由三个层次组成：

1. **背景（Background）**：深色背景（RGB 30, 30, 30）提供工业质感，突出前景元素。
2. **坐标轴系统（Axes）**：左侧 Y 轴显示数值标签（0-最大值），底部 X 轴显示数据项标签，坐标轴采用浅灰色（RGB 150, 150, 150）增强可读性。
3. **柱状图（Bars）**：多个垂直柱体，每个柱体采用线性渐变（从亮到暗），柱体顶部显示数值，底部显示标签，支持自定义颜色。

这种“背景+坐标轴+数据柱”的层级设计既保留了数据精确性，又突出了对比可视化。

### API 设计与可用性

`BarGraphWidget` 提供了简洁直观的接口：

- **数据添加**：`addBar(const QString &label, double value, const QColor &color)` - 添加一个数据条，指定标签、数值与颜色。
- **数据更新**：`setBarValue(int index, double value)` / `setBarValue(const QString &label, double value)` - 通过索引或标签更新数据条数值。
- **数据清除**：`clearBars()` - 清除所有数据条。
- **配置选项**：`setMaxValue(double maxValue)` - 设置 Y 轴最大值；`setAutoScale(bool enabled)` - 启用/禁用自动缩放；`setShowLabels(bool show)` - 显示/隐藏标签；`setShowValues(bool show)` - 显示/隐藏数值。

核心原则是“先添加数据，再配置显示”；内部会在数据变化时自动更新显示，支持实时数据流。

### 数据映射与坐标转换

- X 轴映射：数据条索引线性映射到 X 坐标，柱体宽度根据数据条数量自动计算，柱体间距固定为 10px。
- Y 轴映射：数据值线性映射到 Y 坐标，`percentage = value / maxValue`，`barHeight = chartHeight * percentage`。
- 自动缩放：启用时自动计算所有数据条的最大值，并添加 10% 的边距，确保数据完整显示。
- 固定范围：禁用自动缩放时使用用户指定的最大值，适用于需要固定参考基准的场景。

这种映射策略适用于线性数据；若需支持非线性映射（如对数刻度），可在计算时替换为自定义函数。

### 柱体布局与间距计算

- **柱体宽度**：根据数据条数量和可用宽度自动计算，`barWidth = (chartWidth - totalSpacing) / numBars`，最小宽度为 10px。
- **柱体间距**：固定为 10px，确保柱体之间有足够的视觉分离。
- **边距设置**：左侧边距 50px（用于 Y 轴标签），右侧边距 20px，顶部边距 20px，底部边距 30-50px（根据是否显示标签调整）。

这种布局策略能够在保证可读性的同时，最大化数据展示区域。

### 细节打磨：绘制技巧与真实感

- **线性渐变**：每个柱体采用线性渐变，从顶部亮（`color.lighter(120)`）到底部暗（`color`），增强立体感与真实感。
- **边框线条**：柱体边框采用深色（`color.darker(130)`），线宽 2px，增强柱体边界与层次感。
- **数值显示**：柱体顶部显示数值（保留 1 位小数），采用白色粗体字体，确保在深色背景下清晰可读。
- **标签显示**：底部标签采用旋转 -45 度显示，避免标签重叠，提升可读性。
- **坐标轴标签**：Y 轴显示 5 个等分标签，从 0 到最大值，采用右对齐显示，确保数值对齐。
- **抗锯齿**：启用 `QPainter::Antialiasing`，在矩形与文字绘制下保持平滑边缘。

### 性能与适配

- **抗锯齿**：启用 `QPainter::Antialiasing`，在复杂渐变与文字绘制下保持平滑边缘。
- **增量重绘**：仅在数据变化或配置更新时调用 `update()`，避免无谓重绘；这对实时数据更新场景尤为重要。
- **尺寸自适应**：柱体宽度、高度、标签位置均基于控件尺寸计算，确保在不同尺寸下保持视觉比例一致。
- **最小尺寸约束**：通过 `minimumSizeHint()` 返回 `QSize(200, 150)`，确保控件在小尺寸下仍可清晰显示。

### 可扩展方向

- **水平柱状图**：支持水平布局，适用于标签较长的场景。
- **分组柱状图**：支持多系列分组显示，形成分组对比效果。
- **堆叠柱状图**：支持多系列堆叠显示，展示累积效果。
- **数据点标记**：支持在柱体上标记特定数据点（如目标值、平均值），增强信息表达。
- **动画过渡**：在数据变化时通过动画实现柱体高度的平滑过渡，提升视觉体验。
- **交互提示**：鼠标悬停显示数据详细信息，点击柱体弹出详情对话框。
- **自定义主题**：开放颜色、字体参数，支持夜间模式、高对比模式等主题切换。
- **导出功能**：支持将图表导出为图片（PNG/SVG）或数据导出为 CSV，便于报告生成。

### 与业务集成

- **线程模型**：数据采集线程与 UI 线程解耦，使用信号槽或线程安全队列把最新数据推送到主线程再调用 `setBarValue`。
- **数据持久化**：结合数据库或配置文件，保存历史数据，实现数据追溯与回放。
- **多图表组合**：在仪表盘中并排显示多个柱状图，分别监控不同指标，形成数据矩阵视图。
- **告警集成**：结合阈值检测，当数据超出安全范围时在柱体上标记告警颜色，形成可视化告警。
- **国际化与可访问性**：标签、数值文本可配置；颜色同时配合文字/图标，覆盖色弱/高对比场景。

### 简要用法示例

```cpp
// 创建与基础配置
auto barGraph = new BarGraphWidget(this);
barGraph->setAutoScale(true);
barGraph->setShowLabels(true);
barGraph->setShowValues(true);

// 添加数据条
barGraph->addBar("Device A", 85.5, QColor(0, 150, 255));
barGraph->addBar("Device B", 92.3, QColor(255, 165, 0));
barGraph->addBar("Device C", 78.1, QColor(0, 200, 0));

// 更新数据值
barGraph->setBarValue("Device A", 90.0);
```

> 注：数据采集与 UI 更新解耦，非主线程不直接操作控件。

### 设计取舍回顾

- **选择垂直柱状图**：垂直柱状图更符合“数值越大高度越高”的直觉，且便于显示数值标签。
- **固定柱体间距**：固定间距简化了布局计算，但可通过扩展支持自适应间距。
- **自动缩放默认启用**：简化使用，但允许用户禁用以设置固定范围，适用于需要固定参考的场景。
- **标签旋转显示**：-45 度旋转避免标签重叠，但占用更多垂直空间。

### 结语

`BarGraphWidget` 以直观的视觉对比和灵活的数据管理，覆盖了工业数据可视化中“多参数对比、性能评估、统计分析”的核心需求；同时保持足够的扩展空间，便于适配不同产品线的主题风格与业务规则。  
如果你正在打造一个既专业又易用的可视化界面，不妨把它作为“数据对比”的基础组件，再根据项目特性逐步演进：加分组、加堆叠、加交互与导出功能。期待你基于它构建出更具表现力的工业 UI。

### 附录｜源码

源码放置方式：
- **文内"关键片段"**：展示核心接口与关键绘制逻辑，控制篇幅。
- **仓库/附件"完整源码"**：附仓库链接或网盘附件，标注 commit 与分支。
- **标注文件路径与版本**：便于在工程中定位。
- **构建指引简述**：包含 Qt 版本、CMake 配置与最小可运行示例。

以下为关键片段预览（完整代码请见工程仓库）：

关键头文件：`src/BarGraph/BarGraphWidget.h`

```10:60:src/BarGraph/BarGraphWidget.h
/**
 * @brief Bar graph widget
 * 
 * This widget displays multiple data values as vertical bars,
 * suitable for comparing multiple parameters or values.
 */
class BarGraphWidget : public QWidget
{
    Q_OBJECT

public:
    struct BarData {
        QString label;
        double value;
        QColor color;
    };

    explicit BarGraphWidget(QWidget *parent = nullptr);
    ~BarGraphWidget() override;

    // Data management
    void addBar(const QString &label, double value, const QColor &color);
    void setBarValue(int index, double value);
    void setBarValue(const QString &label, double value);
    void clearBars();

    // Setters
    void setMaxValue(double maxValue);
    void setAutoScale(bool enabled);
    void setShowLabels(bool show);
    void setShowValues(bool show);
```

关键实现：`src/BarGraph/BarGraphWidget.cpp`

构造与基础参数：

```4:14:src/BarGraph/BarGraphWidget.cpp
BarGraphWidget::BarGraphWidget(QWidget *parent)
    : QWidget(parent)
    , m_maxValue(100.0)
    , m_autoScale(true)
    , m_showLabels(true)
    , m_showValues(true)
    , m_backgroundColor(QColor(30, 30, 30))
    , m_axisColor(QColor(150, 150, 150))
{
    setMinimumSize(200, 150);
}
```

添加数据条：

```20:28:src/BarGraph/BarGraphWidget.cpp
void BarGraphWidget::addBar(const QString &label, double value, const QColor &color)
{
    BarData bar;
    bar.label = label;
    bar.value = value;
    bar.color = color;
    m_bars.append(bar);
    update();
}
```

坐标轴绘制（含自动缩放）：

```99:142:src/BarGraph/BarGraphWidget.cpp
void BarGraphWidget::drawAxes(QPainter &painter)
{
    int marginLeft = 50;
    int marginRight = 20;
    int marginTop = 20;
    int marginBottom = m_showLabels ? 50 : 30;

    int chartWidth = width() - marginLeft - marginRight;
    int chartHeight = height() - marginTop - marginBottom;

    // Draw axes
    painter.setPen(QPen(m_axisColor, 2));
    painter.drawLine(marginLeft, marginTop, marginLeft, marginTop + chartHeight);
    painter.drawLine(marginLeft, marginTop + chartHeight, 
                    marginLeft + chartWidth, marginTop + chartHeight);

    // Draw Y-axis labels
    double maxVal = m_maxValue;
    if (m_autoScale && !m_bars.isEmpty()) {
        maxVal = 0.0;
        for (const auto &bar : m_bars) {
            if (bar.value > maxVal)
                maxVal = bar.value;
        }
        maxVal *= 1.1;  // Add 10% margin
    }

    painter.setPen(m_axisColor);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    int numYLabels = 5;
    for (int i = 0; i <= numYLabels; ++i) {
        int y = marginTop + chartHeight - (i * chartHeight / numYLabels);
        double value = (maxVal * i) / numYLabels;

        painter.drawLine(marginLeft - 5, y, marginLeft, y);
        
        QString label = QString::number(value, 'f', 0);
        painter.drawText(5, y - 8, marginLeft - 15, 16,
                        Qt::AlignRight | Qt::AlignVCenter, label);
    }
}
```

柱状图绘制（含渐变效果）：

```144:226:src/BarGraph/BarGraphWidget.cpp
void BarGraphWidget::drawBars(QPainter &painter)
{
    if (m_bars.isEmpty())
        return;

    int marginLeft = 50;
    int marginRight = 20;
    int marginTop = 20;
    int marginBottom = m_showLabels ? 50 : 30;

    int chartWidth = width() - marginLeft - marginRight;
    int chartHeight = height() - marginTop - marginBottom;

    // Calculate max value for scaling
    double maxVal = m_maxValue;
    if (m_autoScale) {
        maxVal = 0.0;
        for (const auto &bar : m_bars) {
            if (bar.value > maxVal)
                maxVal = bar.value;
        }
        maxVal *= 1.1;
    }

    if (maxVal == 0.0)
        maxVal = 1.0;

    // Calculate bar dimensions
    int numBars = m_bars.size();
    int barSpacing = 10;
    int totalSpacing = (numBars + 1) * barSpacing;
    int barWidth = (chartWidth - totalSpacing) / numBars;

    if (barWidth < 10)
        barWidth = 10;

    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    // Draw each bar
    for (int i = 0; i < numBars; ++i) {
        const BarData &bar = m_bars[i];

        int x = marginLeft + barSpacing + i * (barWidth + barSpacing);
        double percentage = bar.value / maxVal;
        int barHeight = static_cast<int>(chartHeight * percentage);
        int y = marginTop + chartHeight - barHeight;

        // Draw bar
        QLinearGradient gradient(x, y, x, y + barHeight);
        gradient.setColorAt(0.0, bar.color.lighter(120));
        gradient.setColorAt(1.0, bar.color);

        painter.setBrush(gradient);
        painter.setPen(QPen(bar.color.darker(130), 2));
        painter.drawRect(x, y, barWidth, barHeight);

        // Draw value on top of bar
        if (m_showValues) {
            painter.setPen(Qt::white);
            font.setBold(true);
            painter.setFont(font);
            
            QString valueText = QString::number(bar.value, 'f', 1);
            painter.drawText(x, y - 15, barWidth, 12,
                           Qt::AlignCenter, valueText);
        }

        // Draw label below bar
        if (m_showLabels) {
            painter.setPen(m_axisColor);
            font.setBold(false);
            painter.setFont(font);

            painter.save();
            painter.translate(x + barWidth / 2, marginTop + chartHeight + 10);
            painter.rotate(-45);
            painter.drawText(-50, 0, 100, 20, Qt::AlignLeft | Qt::AlignVCenter, bar.label);
            painter.restore();
        }
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
project(BarGraphMiniDemo LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(BarGraphMiniDemo
    src/main.cpp
    src/BarGraphWidget.cpp
    src/BarGraphWidget.h
)
target_link_libraries(BarGraphMiniDemo PRIVATE Qt6::Widgets)
```

- **最小 `main.cpp`**：

```cpp
#include <QApplication>
#include "BarGraphWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    BarGraphWidget w;
    w.addBar("Item A", 75.0, QColor(0, 150, 255));
    w.addBar("Item B", 90.0, QColor(255, 165, 0));
    w.addBar("Item C", 60.0, QColor(0, 200, 0));
    w.resize(600, 400);
    w.show();
    return app.exec();
}
```

