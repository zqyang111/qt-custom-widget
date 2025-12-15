### Qt工业控件实战·第07篇｜AnalogClock 模拟时钟控件的设计与实践

### 写在前面

在工业监控界面中，时钟显示是最基础也最常用的信息展示之一。从系统时间显示、操作记录时间戳到定时任务触发，时钟信息直接影响操作效率与系统可追溯性。传统的数字时钟虽然精确，但缺乏视觉美感；而模拟时钟控件以“圆形表盘+指针旋转”这一经典设计，在保留时间精确性的同时提升了界面质感。本文基于本项目的 `AnalogClockWidget`，深入解析其设计思路、绘制技巧与工程实践，帮助你在 Qt 应用中快速集成一款专业的工业模拟时钟控件。

<img width="1202" height="832" alt="analog_clock" src="https://github.com/user-attachments/assets/52c78311-2cf1-4f30-b95c-51691eb381cd" />

### 组件定位与典型场景

- **系统时间显示**：在仪表盘或主界面显示当前系统时间，提供直观的时间参考。
- **操作记录时间戳**：在操作日志或事件记录中显示时间，便于快速识别操作时间点。
- **定时任务监控**：显示定时任务的执行时间，结合任务状态形成完整的任务监控视图。
- **多时区显示**：在仪表盘中并排显示多个时钟，分别显示不同时区的时间，适用于跨时区协作。

### 视觉结构拆解

`AnalogClockWidget` 的画面由四个层次组成：

1. **表盘背景（Background）**：浅色圆形表盘（RGB 240, 240, 240），采用径向渐变模拟立体感，外圈深色边框增强边界。
2. **刻度系统（Ticks）**：12 个整点刻度，其中 12、3、6、9 点为主刻度（粗线+数字），其他为次刻度（细线）。
3. **指针系统（Hands）**：时针（深色、短粗）、分针（中色、中等）、秒针（红色、细长），采用多边形或线条绘制。
4. **中心点（Center Dot）**：红色圆形中心点，与秒针颜色呼应，增强视觉统一性。

这种“表盘+刻度+指针+中心点”的层级设计既保留了传统时钟的物理隐喻，又融入了现代界面的数字化表达。

### API 设计与可用性

`AnalogClockWidget` 提供了简洁直观的接口：

- **时间设置**：`setTime(const QTime &time)` - 设置时钟显示时间，立即更新显示。
- **启动/停止**：`start()` - 启动时钟，自动更新为当前系统时间；`stop()` - 停止时钟，保持当前显示时间。
- **重置**：`reset()` - 重置时间为 00:00:00，用于计时器场景。
- **状态查询**：`time()` - 获取当前显示时间；`isRunning()` - 查询时钟是否运行中。

核心原则是“时间驱动显示”；内部通过 `QTimer` 每秒更新一次，确保时间显示的实时性与准确性。

### 角度计算与指针旋转

- 时针角度：`angle = 30.0 * hour + 0.5 * minute`（30度/小时 + 0.5度/分钟），实现时针的平滑移动。
- 分针角度：`angle = 6.0 * minute + 0.1 * second`（6度/分钟 + 0.1度/秒），实现分针的平滑移动。
- 秒针角度：`angle = 6.0 * second`（6度/秒），实现秒针的精确移动。
- 坐标转换：使用 `QPainter::translate` 和 `QPainter::rotate` 实现坐标系的平移与旋转，简化指针绘制。

这种角度计算策略实现了指针的平滑移动，避免了跳跃感，提升了视觉体验。

### 刻度系统设计

`AnalogClockWidget` 采用 12 小时制刻度系统：

- **主刻度**：12、3、6、9 点位置采用粗线（线宽 4）和数字标签，便于快速识别整点。
- **次刻度**：其他 8 个位置采用细线（线宽 2），提供精细的时间参考。
- **刻度长度**：主刻度从 -85 到 -70（半径单位），次刻度从 -85 到 -75，形成视觉层次。
- **数字标签**：主刻度位置显示数字（12 点显示 12，其他显示对应小时），字体加粗，确保可读性。

这种刻度设计既保留了传统时钟的经典布局，又适应了工业界面的简洁风格。

### 细节打磨：绘制技巧与真实感

- **径向渐变表盘**：表盘采用径向渐变，从中心亮到边缘暗，模拟真实表盘的立体感与反光效果。
- **内阴影效果**：在表盘内部绘制径向渐变阴影，增强表盘的立体感与层次。
- **指针形状**：时针和分针采用多边形绘制，形成宽窄渐变的视觉效果，更接近真实时钟指针。
- **秒针设计**：秒针采用细线（线宽 2）和红色，在深色表盘上形成高对比度，便于快速识别。
- **中心点设计**：中心点采用红色圆形，与秒针颜色呼应，增强视觉统一性。
- **抗锯齿**：启用 `QPainter::Antialiasing`，在圆形、多边形与旋转绘制下保持平滑边缘。
- **尺寸自适应**：所有绘制元素基于控件尺寸的缩放系数（side / 200.0）计算，确保在不同尺寸下保持视觉比例一致。

### 性能与适配

- **抗锯齿**：启用 `QPainter::Antialiasing`，在复杂旋转与渐变绘制下保持平滑边缘。
- **定时器优化**：使用 `QTimer` 每秒更新一次，避免过于频繁的重绘；仅在时间变化时调用 `update()`。
- **尺寸自适应**：表盘大小、刻度位置、指针长度均基于控件尺寸比例计算，确保在不同尺寸下保持视觉比例一致。
- **最小尺寸约束**：通过 `minimumSizeHint()` 返回 `QSize(120, 120)`，确保控件在小尺寸下仍可清晰显示。

### 交互设计

- **自动更新**：启动后自动每秒更新一次，显示当前系统时间，无需手动刷新。
- **停止控制**：停止后保持当前显示时间，适用于计时器或固定时间显示场景。
- **时间设置**：支持手动设置时间，适用于显示特定时间点或时区时间。

### 可扩展方向

- **24 小时制支持**：支持 24 小时制刻度，适用于军用或国际标准时间显示。
- **数字时间显示**：在表盘下方或内部显示数字时间（HH:MM:SS），形成模拟+数字的双重显示。
- **时区支持**：支持设置时区，自动计算并显示对应时区的时间。
- **自定义主题**：开放表盘颜色、指针颜色、刻度样式参数，支持夜间模式、高对比模式等主题切换。
- **动画过渡**：在时间变化时通过动画实现指针的平滑过渡，提升“物理感”。
- **闹钟功能**：支持设置闹钟时间，到达时间时触发信号或播放提示音。
- **计时器模式**：支持倒计时模式，从设定时间倒计时到 00:00:00。

### 与业务集成

- **线程模型**：时间更新在主线程中通过 `QTimer` 实现，确保线程安全；外部时间源可通过信号槽推送时间。
- **多时钟组合**：在仪表盘中并排显示多个时钟，分别显示不同时区或不同功能的时间，形成时间矩阵视图。
- **时间同步**：结合网络时间协议（NTP）或系统时间同步，确保时钟显示的准确性。
- **事件记录**：结合操作日志或事件记录，在时钟上标记关键时间点，形成时间线视图。
- **国际化与可访问性**：支持 12/24 小时制切换；颜色同时配合文字/图标，覆盖色弱/高对比场景。

### 简要用法示例

```cpp
// 创建与基础配置
auto clock = new AnalogClockWidget(this);
clock->start();  // Start automatic update

// 手动设置时间
QTime customTime(14, 30, 0);  // 2:30 PM
clock->setTime(customTime);

// 停止时钟（保持当前时间）
clock->stop();

// 重置为 00:00:00
clock->reset();
```

> 注：时钟自动更新在主线程中执行，无需额外线程管理。

### 设计取舍回顾

- **选择 12 小时制**：符合传统时钟习惯，但可通过扩展支持 24 小时制。
- **固定更新频率**：每秒更新一次，平衡了实时性与性能，但可通过配置调整。
- **指针颜色区分**：时针、分针、秒针采用不同颜色，便于快速识别，但需注意颜色对比度。
- **简化刻度设计**：仅显示整点刻度，避免过度复杂，但可通过扩展支持分钟刻度。

### 结语

`AnalogClockWidget` 以直观的视觉表现和简洁的时间管理，覆盖了工业界面中“时间显示、时间参考、时间美感”的核心需求；同时保持足够的扩展空间，便于适配不同产品线的主题风格与业务规则。  
如果你正在打造一个既专业又易用的可视化界面，不妨把它作为“时间显示”的基础组件，再根据项目特性逐步演进：加数字显示、加时区支持、加闹钟与计时器功能。期待你基于它构建出更具表现力的工业 UI。

### 附录｜源码

源码放置方式：
- **文内"关键片段"**：展示核心接口与关键绘制逻辑，控制篇幅。
- **仓库/附件"完整源码"**：附仓库链接或网盘附件，标注 commit 与分支。
- **标注文件路径与版本**：便于在工程中定位。
- **构建指引简述**：包含 Qt 版本、CMake 配置与最小可运行示例。

以下为关键片段预览（完整代码请见工程仓库）：

关键头文件：`src/AnalogClock/AnalogClockWidget.h`

```9:54:src/AnalogClock/AnalogClockWidget.h
/**
 * @brief Analog clock widget
 * 
 * This widget displays an industrial-style analog clock with hour, minute, and second hands.
 */
class AnalogClockWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnalogClockWidget(QWidget *parent = nullptr);
    ~AnalogClockWidget() override;

    // Getters
    QTime time() const { return m_time; }
    bool isRunning() const { return m_running; }

    // Setters
    void setTime(const QTime &time);
    void start();
    void stop();
    void reset();

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(200, 200); }
    QSize minimumSizeHint() const override { return QSize(120, 120); }
```

关键实现：`src/AnalogClock/AnalogClockWidget.cpp`

构造与定时器设置：

```5:19:src/AnalogClock/AnalogClockWidget.cpp
AnalogClockWidget::AnalogClockWidget(QWidget *parent)
    : QWidget(parent)
    , m_time(QTime::currentTime())
    , m_timer(new QTimer(this))
    , m_running(true)
    , m_faceColor(QColor(240, 240, 240))
    , m_hourHandColor(QColor(40, 40, 40))
    , m_minuteHandColor(QColor(60, 60, 60))
    , m_secondHandColor(QColor(255, 0, 0))
{
    setMinimumSize(120, 120);
    
    connect(m_timer, &QTimer::timeout, this, &AnalogClockWidget::updateClock);
    m_timer->start(1000);  // Update every second
}
```

时间更新（定时器槽函数）：

```54:60:src/AnalogClock/AnalogClockWidget.cpp
void AnalogClockWidget::updateClock()
{
    if (m_running) {
        m_time = QTime::currentTime();
        update();
    }
}
```

表盘背景绘制（含径向渐变）：

```75:97:src/AnalogClock/AnalogClockWidget.cpp
void AnalogClockWidget::drawBackground(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    // Draw outer ring
    painter.setPen(QPen(QColor(100, 100, 100), 3));
    painter.setBrush(m_faceColor);
    painter.drawEllipse(-95, -95, 190, 190);

    // Draw inner shadow
    QRadialGradient gradient(0, -20, 100);
    gradient.setColorAt(0.0, QColor(255, 255, 255, 0));
    gradient.setColorAt(0.8, QColor(0, 0, 0, 20));
    gradient.setColorAt(1.0, QColor(0, 0, 0, 40));
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(-90, -90, 180, 180);

    painter.restore();
}
```

刻度绘制（含主次刻度与数字标签）：

```99:143:src/AnalogClock/AnalogClockWidget.cpp
void AnalogClockWidget::drawTicks(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    painter.setPen(QPen(QColor(80, 80, 80), 2));

    // Draw hour marks
    for (int i = 0; i < 12; ++i) {
        painter.save();
        painter.rotate(30.0 * i);  // 360 / 12 = 30 degrees
        
        if (i % 3 == 0) {
            // Major marks at 12, 3, 6, 9
            painter.setPen(QPen(QColor(60, 60, 60), 4));
            painter.drawLine(0, -85, 0, -70);
            
            // Draw numbers
            painter.save();
            painter.translate(0, -60);
            painter.rotate(-30.0 * i);
            
            QFont font = painter.font();
            font.setPointSize(14);
            font.setBold(true);
            painter.setFont(font);
            painter.setPen(QColor(40, 40, 40));
            
            int hour = (i == 0) ? 12 : i;
            painter.drawText(QRectF(-15, -10, 30, 20), Qt::AlignCenter, 
                           QString::number(hour));
            painter.restore();
        } else {
            // Regular hour marks
            painter.setPen(QPen(QColor(100, 100, 100), 2));
            painter.drawLine(0, -85, 0, -75);
        }
        
        painter.restore();
    }

    painter.restore();
}
```

指针绘制（含角度计算）：

```145:184:src/AnalogClock/AnalogClockWidget.cpp
void AnalogClockWidget::drawHands(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    int hour = m_time.hour() % 12;
    int minute = m_time.minute();
    int second = m_time.second();

    // Hour hand
    painter.save();
    painter.rotate(30.0 * hour + 0.5 * minute);  // 30 deg/hour + 0.5 deg/minute
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_hourHandColor);
    QPolygon hourHand;
    hourHand << QPoint(-4, 8) << QPoint(4, 8) << QPoint(2, -50) << QPoint(-2, -50);
    painter.drawPolygon(hourHand);
    painter.restore();

    // Minute hand
    painter.save();
    painter.rotate(6.0 * minute + 0.1 * second);  // 6 deg/minute + 0.1 deg/second
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_minuteHandColor);
    QPolygon minuteHand;
    minuteHand << QPoint(-3, 8) << QPoint(3, 8) << QPoint(2, -70) << QPoint(-2, -70);
    painter.drawPolygon(minuteHand);
    painter.restore();

    // Second hand
    painter.save();
    painter.rotate(6.0 * second);  // 6 deg/second
    painter.setPen(QPen(m_secondHandColor, 2));
    painter.drawLine(0, 10, 0, -80);
    painter.restore();

    painter.restore();
}
```

中心点绘制：

```186:199:src/AnalogClock/AnalogClockWidget.cpp
void AnalogClockWidget::drawCenterDot(QPainter &painter)
{
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    // Draw center dot
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    painter.setBrush(m_secondHandColor);
    painter.drawEllipse(-6, -6, 12, 12);

    painter.restore();
}
```

### 最小构建与集成指引

最小可运行步骤：

- **环境**：Qt 6.x（或 Qt 5.15+），CMake 3.20+，编译器与平台按团队规范即可。
- **依赖**：无额外三方依赖，Qt Widgets 模块即可。
- **CMake 示例**：

```cmake
cmake_minimum_required(VERSION 3.20)
project(AnalogClockMiniDemo LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(AnalogClockMiniDemo
    src/main.cpp
    src/AnalogClockWidget.cpp
    src/AnalogClockWidget.h
)
target_link_libraries(AnalogClockMiniDemo PRIVATE Qt6::Widgets)
```

- **最小 `main.cpp`**：

```cpp
#include <QApplication>
#include "AnalogClockWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    AnalogClockWidget w;
    w.start();  // Start automatic update
    w.resize(300, 300);
    w.show();
    return app.exec();
}
```

