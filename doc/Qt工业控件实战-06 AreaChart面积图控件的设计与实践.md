### Qt工业控件实战·第06篇｜AreaChart 面积图控件的设计与实践

### 写在前面

在工业数据可视化中，面积图是最常用也最直观的趋势展示方式之一。从生产数据趋势、设备运行状态到能耗分析，面积图以“填充区域”这一视觉隐喻，让操作员能够快速识别数据变化趋势与累积效果。传统的折线图虽然精确，但缺乏视觉冲击力；而面积图控件以“多系列支持+堆叠模式+自动缩放”这一专业设计，在保留数据精确性的同时提升了视觉表现力。本文基于本项目的 `AreaChart`，深入解析其设计思路、绘制技巧与工程实践，帮助你在 Qt 应用中快速集成一款专业的工业面积图控件。

<img width="1202" height="832" alt="area_chart" src="https://github.com/user-attachments/assets/cb73a52a-a344-4242-8166-c0f43464ff1c" />

### 组件定位与典型场景

- **生产数据趋势**：产量、效率、良品率等生产指标的实时趋势展示，支持多指标对比。
- **设备运行监控**：温度、压力、流量等工艺参数的连续监控，通过堆叠模式展示多参数累积效果。
- **能耗分析**：电力、水、气等能源消耗的趋势分析，支持按时间段、设备类型分组展示。
- **质量统计**：合格率、不良率、返工率等质量指标的长期趋势跟踪，便于发现异常模式。

### 视觉结构拆解

`AreaChart` 的画面由五个层次组成：

1. **背景与标题（Background）**：深色背景（RGB 43, 43, 43）提供工业质感，顶部居中显示图表标题。
2. **网格系统（Grid）**：黑色绘图区域，10×8 网格线提供精确读数参考，深色网格线增强层次感。
3. **面积填充（Areas）**：根据数据系列绘制填充区域，支持堆叠或重叠模式，采用半透明填充增强视觉层次。
4. **坐标轴标签（Labels）**：左侧 Y 轴显示数值标签，底部 X 轴显示时间标签，右侧 Y 轴标签旋转显示。
5. **图例（Legend）**：右侧图例显示各系列名称与颜色，便于识别不同数据系列。

这种“背景+网格+数据+标签+图例”的层级设计既保留了数据精确性，又突出了趋势可视化。

### API 设计与可用性

`AreaChart` 提供了完整的数据管理接口：

- **系列管理**：`addSeries(const QString &name, const QColor &color)` - 添加数据系列，指定名称与颜色。
- **数据添加**：`addDataPoint(const QString &name, double value)` - 向指定系列添加单个数据点，自动滚动显示。
- **批量设置**：`setSeriesData(const QString &name, const QVector<double> &data)` - 批量设置系列数据。
- **数据清除**：`clearSeries(const QString &name)` / `clearAllSeries()` - 清除指定系列或所有系列数据。
- **配置选项**：`setYRange(double min, double max)` - 设置 Y 轴范围；`setAutoScale(bool)` - 启用/禁用自动缩放；`setStacked(bool)` - 设置堆叠模式；`setTitle(const QString &)` - 设置标题；`setYLabel(const QString &)` - 设置 Y 轴标签；`setShowLegend(bool)` - 显示/隐藏图例。

核心原则是“先添加系列，再添加数据”；内部会在数据变化时自动更新显示，支持实时数据流。

### 数据映射与坐标转换

- X 轴映射：数据点索引线性映射到 X 坐标，`x = plotArea.left() + plotArea.width() * i / (maxPoints - 1.0)`。
- Y 轴映射：数据值线性映射到 Y 坐标，`normalizedY = (value - yMin) / yRange`，`y = plotArea.bottom() - normalizedY * plotArea.height()`。
- 堆叠模式：在堆叠模式下，每个系列的值累加到前一系列的值上，形成堆叠效果。
- 自动缩放：自动计算所有系列的最小值与最大值，并添加 10% 的边距，确保数据完整显示。

这种映射策略适用于线性数据；若需支持非线性映射（如对数刻度），可在计算时替换为自定义函数。

### 堆叠模式与重叠模式

`AreaChart` 支持两种显示模式：

1. **重叠模式（Overlapping）**：
   - 各系列独立绘制，区域可能重叠。
   - 填充透明度设为 80，便于观察重叠区域。
   - 适用于对比不同系列的趋势，便于识别交叉点。

2. **堆叠模式（Stacked）**：
   - 各系列值累加，形成堆叠效果。
   - 填充透明度设为 180，增强视觉对比。
   - 适用于展示累积效果，如总产量、总能耗等。

模式切换通过 `setStacked(bool)` 实现，可在运行时动态切换。

### 自动缩放与范围管理

- **自动缩放**：启用时自动计算所有系列的最小值与最大值，并添加 10% 边距，确保数据完整显示。
- **固定范围**：禁用自动缩放时使用用户指定的 Y 轴范围，适用于需要固定参考基准的场景。
- **范围更新**：在添加数据点或设置数据时自动更新范围（如果启用自动缩放）。
- **边界处理**：当数据范围过小时（< 0.001），自动设置为默认范围（0-100），避免除零错误。

### 细节打磨：绘制技巧与真实感

- **路径绘制**：使用 `QPainterPath` 绘制面积区域，先绘制顶部曲线，再连接底部形成封闭路径。
- **半透明填充**：填充颜色采用半透明（重叠模式 80，堆叠模式 180），增强视觉层次与真实感。
- **边框线条**：每个系列使用对应颜色的实线边框（线宽 2），增强系列区分度。
- **网格系统**：10×8 网格提供精确读数参考，网格线颜色（RGB 40, 40, 40）与背景形成适度对比。
- **图例布局**：图例采用颜色块+文字标签的经典布局，每项高度 25px，支持滚动显示。
- **抗锯齿**：启用 `QPainter::Antialiasing`，在曲线与填充绘制下保持平滑边缘。

### 性能与适配

- **抗锯齿**：启用 `QPainter::Antialiasing`，在复杂路径与填充绘制下保持平滑边缘。
- **数据限制**：每个系列最多保留 100 个数据点，超出时自动移除最旧数据，避免内存无限增长。
- **增量重绘**：仅在数据变化或配置更新时调用 `update()`，避免无谓重绘；这对实时数据流场景尤为重要。
- **尺寸自适应**：绘图区域、图例位置、标签位置均基于控件尺寸计算，确保在不同尺寸下保持视觉比例一致。

### 可扩展方向

- **时间轴支持**：X 轴支持显示实际时间标签，而非简单的索引，适用于历史数据分析。
- **数据点标记**：支持在曲线上标记特定数据点（如最大值、最小值、异常值），增强信息表达。
- **缩放与平移**：支持鼠标滚轮缩放与拖拽平移，适用于查看大量历史数据。
- **导出功能**：支持将图表导出为图片（PNG/SVG）或数据导出为 CSV，便于报告生成。
- **动画过渡**：在添加新数据点时通过动画实现平滑过渡，提升视觉体验。
- **自定义主题**：开放颜色、字体参数，支持夜间模式、高对比模式等主题切换。
- **交互提示**：鼠标悬停显示数据点详细信息（时间、数值），点击数据点弹出详情对话框。

### 与业务集成

- **线程模型**：数据采集线程与 UI 线程解耦，使用信号槽或线程安全队列把最新数据推送到主线程再调用 `addDataPoint`。
- **数据持久化**：结合数据库或配置文件，保存历史数据，实现数据追溯与回放。
- **多图表组合**：在仪表盘中并排显示多个面积图，分别监控不同指标，形成数据矩阵视图。
- **告警集成**：结合阈值检测，当数据超出安全范围时在图表上标记告警区域，形成可视化告警。
- **国际化与可访问性**：标题、标签、图例文本可配置；颜色同时配合文字/图标，覆盖色弱/高对比场景。

### 简要用法示例

```cpp
// 创建与基础配置
auto chart = new AreaChart(this);
chart->setTitle("Production Trend");
chart->setYLabel("Production (units)");
chart->setStacked(false);  // Overlapping mode
chart->setAutoScale(true);

// 添加数据系列
chart->addSeries("Line A", QColor(0, 150, 255));
chart->addSeries("Line B", QColor(255, 165, 0));

// 实时添加数据点
chart->addDataPoint("Line A", 45.5);
chart->addDataPoint("Line B", 32.3);

// 批量设置数据
QVector<double> data = {10, 20, 30, 25, 35, 40};
chart->setSeriesData("Line A", data);
```

> 注：数据采集与 UI 更新解耦，非主线程不直接操作控件。

### 设计取舍回顾

- **选择面积图而非折线图**：面积填充比折线更具视觉冲击力，且能更好地展示趋势与累积效果。
- **固定最大数据点数**：限制每个系列最多 100 个点，避免内存无限增长，但可通过配置扩展。
- **堆叠与重叠模式**：两种模式覆盖不同场景需求，但增加了绘制复杂度。
- **自动缩放默认启用**：简化使用，但允许用户禁用以设置固定范围，适用于需要固定参考的场景。

### 结语

`AreaChart` 以直观的视觉表现和灵活的数据管理，覆盖了工业数据可视化中“趋势展示、多系列对比、累积效果”的核心需求；同时保持足够的扩展空间，便于适配不同产品线的主题风格与业务规则。  
如果你正在打造一个既专业又易用的可视化界面，不妨把它作为“趋势分析”的基础组件，再根据项目特性逐步演进：加缩放、加交互、加导出与历史回放。期待你基于它构建出更具表现力的工业 UI。

### 附录｜源码

源码放置方式：
- **文内"关键片段"**：展示核心接口与关键绘制逻辑，控制篇幅。
- **仓库/附件"完整源码"**：附仓库链接或网盘附件，标注 commit 与分支。
- **标注文件路径与版本**：便于在工程中定位。
- **构建指引简述**：包含 Qt 版本、CMake 配置与最小可运行示例。

以下为关键片段预览（完整代码请见工程仓库）：

关键头文件：`src/AreaChart/AreaChart.h`

```10:69:src/AreaChart/AreaChart.h
/**
 * @brief Area Chart - Stacked or overlapping area chart
 * Shows trends with filled areas under curves
 */
class AreaChart : public QWidget
{
    Q_OBJECT

public:
    explicit AreaChart(QWidget *parent = nullptr);
    ~AreaChart();

    // Series management
    void addSeries(const QString &name, const QColor &color);
    void addDataPoint(const QString &name, double value);
    void setSeriesData(const QString &name, const QVector<double> &data);
    void clearSeries(const QString &name);
    void clearAllSeries();
    
    // Configuration
    void setYRange(double min, double max);
    void setAutoScale(bool autoScale);
    void setStacked(bool stacked);
    void setTitle(const QString &title);
    void setYLabel(const QString &label);
    void setShowLegend(bool show);
    
    QSize sizeHint() const override { return QSize(600, 400); }
```

关键实现：`src/AreaChart/AreaChart.cpp`

构造与基础参数：

```7:20:src/AreaChart/AreaChart.cpp
AreaChart::AreaChart(QWidget *parent)
    : QWidget(parent)
    , m_yMin(0.0)
    , m_yMax(100.0)
    , m_autoScale(true)
    , m_stacked(false)
    , m_title("Area Chart")
    , m_yLabel("Value")
    , m_showLegend(true)
    , m_maxPoints(100)
    , m_autoYMin(0.0)
    , m_autoYMax(100.0)
{
}
```

添加数据点（含自动滚动与自动缩放）：

```36:51:src/AreaChart/AreaChart.cpp
void AreaChart::addDataPoint(const QString &name, double value)
{
    if (m_series.contains(name)) {
        m_series[name].data.append(value);
        
        if (m_series[name].data.size() > m_maxPoints) {
            m_series[name].data.removeFirst();
        }
        
        if (m_autoScale) {
            updateAutoScale();
        }
        
        update();
    }
}
```

自动缩放计算：

```123:145:src/AreaChart/AreaChart.cpp
void AreaChart::updateAutoScale()
{
    m_autoYMin = 1e10;
    m_autoYMax = -1e10;
    
    for (const SeriesData &series : m_series) {
        if (!series.data.isEmpty()) {
            auto minMax = std::minmax_element(series.data.begin(), series.data.end());
            m_autoYMin = qMin(m_autoYMin, *minMax.first);
            m_autoYMax = qMax(m_autoYMax, *minMax.second);
        }
    }
    
    if (m_autoYMin > m_autoYMax) {
        m_autoYMin = 0.0;
        m_autoYMax = 100.0;
    }
    
    double range = m_autoYMax - m_autoYMin;
    if (range < 0.001) range = 1.0;
    m_autoYMin -= range * 0.1;
    m_autoYMax += range * 0.1;
}
```

面积绘制（含堆叠模式）：

```200:253:src/AreaChart/AreaChart.cpp
void AreaChart::drawAreas(QPainter &painter, const QRect &plotArea)
{
    double yMin = m_autoScale ? m_autoYMin : m_yMin;
    double yMax = m_autoScale ? m_autoYMax : m_yMax;
    double yRange = yMax - yMin;
    if (yRange < 0.001) yRange = 1.0;
    
    QVector<double> stackedValues(m_maxPoints, 0.0);
    
    // Draw each series
    for (const QString &seriesName : m_seriesOrder) {
        const SeriesData &series = m_series[seriesName];
        if (series.data.size() < 2) continue;
        
        QPainterPath path;
        bool firstPoint = true;
        
        // Top line
        for (int i = 0; i < series.data.size(); ++i) {
            double x = plotArea.left() + plotArea.width() * i / (m_maxPoints - 1.0);
            
            double value = series.data[i];
            if (m_stacked) {
                value += stackedValues[i];
                stackedValues[i] = value;
            }
            
            double normalizedY = (value - yMin) / yRange;
            double y = plotArea.bottom() - normalizedY * plotArea.height();
            
            if (firstPoint) {
                path.moveTo(x, y);
                firstPoint = false;
            } else {
                path.lineTo(x, y);
            }
        }
        
        // Close path at bottom
        if (!series.data.isEmpty()) {
            path.lineTo(plotArea.left() + plotArea.width() * (series.data.size() - 1) / (m_maxPoints - 1.0), 
                       plotArea.bottom());
            path.lineTo(plotArea.left(), plotArea.bottom());
            path.closeSubpath();
        }
        
        // Draw filled area
        QColor fillColor = series.color;
        fillColor.setAlpha(m_stacked ? 180 : 80);
        painter.setBrush(fillColor);
        painter.setPen(QPen(series.color, 2));
        painter.drawPath(path);
    }
}
```

图例绘制：

```255:282:src/AreaChart/AreaChart.cpp
void AreaChart::drawLegend(QPainter &painter, const QRect &legendArea)
{
    painter.setPen(QColor(200, 200, 200));
    QFont legendFont = painter.font();
    legendFont.setPointSize(9);
    painter.setFont(legendFont);
    
    int y = legendArea.top();
    int itemHeight = 25;
    
    for (const QString &seriesName : m_seriesOrder) {
        const SeriesData &series = m_series[seriesName];
        
        // Color box
        painter.setBrush(series.color);
        painter.setPen(QPen(QColor(100, 100, 100), 1));
        painter.drawRect(legendArea.left(), y, 20, 15);
        
        // Label
        painter.setPen(QColor(200, 200, 200));
        painter.drawText(legendArea.left() + 25, y, legendArea.width() - 25, 20, 
                        Qt::AlignLeft | Qt::AlignVCenter, series.name);
        
        y += itemHeight;
        
        if (y > legendArea.bottom()) break;
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
project(AreaChartMiniDemo LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(AreaChartMiniDemo
    src/main.cpp
    src/AreaChart.cpp
    src/AreaChart.h
)
target_link_libraries(AreaChartMiniDemo PRIVATE Qt6::Widgets)
```

- **最小 `main.cpp`**：

```cpp
#include <QApplication>
#include "AreaChart.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    AreaChart w;
    w.setTitle("Production Trend");
    w.addSeries("Series A", QColor(0, 150, 255));
    w.addDataPoint("Series A", 50.0);
    w.addDataPoint("Series A", 55.0);
    w.addDataPoint("Series A", 60.0);
    w.resize(800, 600);
    w.show();
    return app.exec();
}
```

