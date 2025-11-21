### Qt工业控件实战·第05篇｜AlarmIndicator 报警指示器控件的设计与实践

### 写在前面

在工业监控系统中，报警管理是最关键也最复杂的功能之一。从设备故障、参数超限到系统异常，报警信息直接影响生产安全与运维效率。传统的列表或弹窗虽然功能完整，但缺乏层次感与优先级表达；而报警指示器控件以“摘要统计+列表展示+闪烁提醒”这一专业设计，让操作员能够快速识别关键报警并采取行动。本文基于本项目的 `AlarmIndicatorWidget`，深入解析其设计思路、绘制技巧与工程实践，帮助你在 Qt 应用中快速集成一款专业的工业报警指示器控件。

![](../picture/aralm_indicator.png)

### 组件定位与典型场景

- **设备故障监控**：电机过载、泵异常、传感器失效等设备故障的实时报警展示。
- **参数超限告警**：温度、压力、流量等工艺参数超出安全范围的报警提示。
- **系统状态监控**：通信中断、数据异常、配置错误等系统级报警的集中管理。
- **多级报警管理**：支持信息/警告/严重三级报警，通过颜色与闪烁区分优先级。

### 视觉结构拆解

`AlarmIndicatorWidget` 的画面由三个层次组成：

1. **背景与边框（Background）**：深色背景（RGB 30, 30, 30）提供工业质感，圆角边框增强视觉边界。
2. **摘要统计（Summary）**：顶部摘要栏显示各级报警数量（严重/警告/信息），采用彩色标签直观展示。
3. **报警列表（Alarm List）**：列表区域显示所有报警项，每项包含级别标签、报警消息、确认状态，未确认的严重/警告级报警会闪烁提醒。

这种“摘要+列表”的设计既保留了信息密度，又突出了关键报警，便于快速决策。

### API 设计与可用性

`AlarmIndicatorWidget` 提供了完整的报警管理接口：

- **添加报警**：`addAlarm(const QString &message, AlarmLevel level)` - 添加新报警，若已存在则更新状态。
- **清除报警**：`clearAlarm(const QString &message)` - 清除指定报警（标记为非激活）。
- **确认报警**：`acknowledgeAlarm(int index)` - 确认指定报警，停止闪烁。
- **清除所有**：`clearAllAlarms()` - 清除所有报警。
- **统计查询**：`activeAlarmCount()` - 返回未确认的激活报警数量。

核心原则是“消息唯一性+状态管理”；内部通过消息文本识别报警，支持同一报警的重复添加与状态更新。

### 报警级别与颜色语义

`AlarmIndicatorWidget` 支持四级报警：

1. **无报警（NoAlarm）**：灰色（RGB 100, 100, 100），表示正常状态。
2. **信息（Info）**：蓝色（RGB 0, 150, 255），表示一般信息提示，无需立即处理。
3. **警告（Warning）**：橙色（RGB 255, 200, 0），表示需要注意的情况，建议尽快处理。
4. **严重（Critical）**：红色（RGB 255, 0, 0），表示严重故障，需要立即处理。

颜色选择遵循工业界通用认知，确保在不同场景下语义一致。

### 闪烁机制与视觉提醒

- **闪烁条件**：未确认的警告级和严重级报警会闪烁，信息级报警不闪烁。
- **闪烁频率**：通过 `QTimer` 每 500ms 切换一次状态，形成明显的视觉提醒。
- **闪烁效果**：闪烁时背景色在正常色与暗色之间切换，确保在深色背景下清晰可见。
- **确认后停止**：报警被确认后停止闪烁，但仍保留在列表中，便于后续查看。

这种闪烁机制能够在保持界面整洁的同时，有效吸引操作员注意关键报警。

### 细节打磨：绘制技巧与真实感

- **摘要栏设计**：顶部摘要栏采用深灰色背景，各级报警数量以彩色圆角矩形标签展示，数字居中显示。
- **列表项布局**：每项包含左侧级别标签（60px宽）和右侧消息文本，级别标签采用对应颜色，消息文本支持省略显示。
- **确认状态区分**：已确认的报警采用深灰色背景，未确认的激活报警采用对应级别颜色（含闪烁）。
- **空状态提示**：无报警时显示 "No Active Alarms" 提示文字，避免空白带来的困惑。
- **文本省略**：消息文本过长时自动省略，使用 `QFontMetrics::elidedText` 确保布局稳定。
- **抗锯齿**：启用 `QPainter::Antialiasing`，在圆角矩形与文字绘制下保持平滑边缘。

### 性能与适配

- **抗锯齿**：启用 `QPainter::Antialiasing`，在复杂绘制下保持平滑边缘。
- **尺寸自适应**：摘要栏高度、列表项高度、标签宽度均基于控件尺寸计算，确保在不同尺寸下保持视觉比例一致。
- **增量重绘**：仅在报警变化或闪烁状态切换时调用 `update()`，避免无谓重绘；闪烁定时器仅在存在未确认报警时运行。
- **最小尺寸约束**：通过 `minimumSizeHint()` 返回 `QSize(200, 100)`，确保控件在小尺寸下仍可清晰显示。

### 交互设计

- **点击确认**：点击列表项可确认该报警，停止闪烁并更新显示。
- **滚动支持**：列表项超出可视区域时支持滚动（可通过扩展实现滚动条）。
- **消息唯一性**：相同消息的报警视为同一报警，重复添加时更新状态而非新增项。

### 可扩展方向

- **滚动条支持**：当报警项过多时，添加滚动条支持，提升可浏览性。
- **报警详情**：点击报警项弹出详情对话框，显示报警时间、来源、处理建议等信息。
- **报警过滤**：支持按级别、确认状态、时间范围过滤报警列表。
- **报警历史**：保存报警历史记录，支持查看已清除的报警。
- **声音提醒**：严重级报警时播放提示音，增强多模态提醒。
- **报警分组**：支持按设备、区域、类型分组显示报警，形成层次化视图。
- **自定义主题**：开放颜色、字体参数，支持夜间模式、高对比模式等主题切换。

### 与业务集成

- **线程模型**：报警采集线程与 UI 线程解耦，使用信号槽或线程安全队列把最新报警推送到主线程再调用 `addAlarm`。
- **报警策略**：结合报警级别、持续时间、确认状态形成告警闭环；可通过扩展支持报警升级（如警告持续超时升级为严重）。
- **多指示器组合**：在仪表盘中显示多个报警指示器，分别监控不同系统或区域，形成报警矩阵视图。
- **数据持久化**：结合数据库或配置文件，保存报警历史与确认记录，实现报警追溯。
- **国际化与可访问性**：报警消息与级别文本可配置；颜色同时配合文字/图标，覆盖色弱/高对比场景。

### 简要用法示例

```cpp
// 创建与基础配置
auto alarmIndicator = new AlarmIndicatorWidget(this);

// 添加报警
alarmIndicator->addAlarm("Motor Overload", AlarmIndicatorWidget::Critical);
alarmIndicator->addAlarm("Temperature High", AlarmIndicatorWidget::Warning);
alarmIndicator->addAlarm("Data Logging Enabled", AlarmIndicatorWidget::Info);

// 清除报警
alarmIndicator->clearAlarm("Motor Overload");

// 查询统计
int activeCount = alarmIndicator->activeAlarmCount();
```

> 注：数据采集与 UI 更新解耦，非主线程不直接操作控件。

### 设计取舍回顾

- **选择三级报警**：信息/警告/严重三级覆盖大多数工业场景，避免过度细分带来的复杂度。
- **消息唯一性**：相同消息视为同一报警，简化管理逻辑，但需注意消息文本的唯一性设计。
- **闪烁机制**：仅警告和严重级报警闪烁，信息级不闪烁，避免界面过于嘈杂。
- **确认后保留**：确认后报警仍保留在列表中，便于后续查看，但需注意列表长度管理。

### 结语

`AlarmIndicatorWidget` 以清晰的层次结构和有效的视觉提醒，覆盖了工业监控中“报警展示、优先级区分、状态管理”的核心需求；同时保持足够的扩展空间，便于适配不同产品线的主题风格与业务规则。  
如果你正在打造一个既专业又易用的可视化界面，不妨把它作为“报警管理”的基础组件，再根据项目特性逐步演进：加滚动、加详情、加过滤与历史记录。期待你基于它构建出更具表现力的工业 UI。

### 附录｜源码

源码放置方式：
- **文内"关键片段"**：展示核心接口与关键绘制逻辑，控制篇幅。
- **仓库/附件"完整源码"**：附仓库链接或网盘附件，标注 commit 与分支。
- **标注文件路径与版本**：便于在工程中定位。
- **构建指引简述**：包含 Qt 版本、CMake 配置与最小可运行示例。

以下为关键片段预览（完整代码请见工程仓库）：

关键头文件：`src/AlarmIndicator/AlarmIndicatorWidget.h`

```8:64:src/AlarmIndicator/AlarmIndicatorWidget.h
/**
 * @brief Alarm indicator widget
 * 
 * This widget displays multiple alarm conditions with different priorities,
 * suitable for monitoring system alarms and warnings.
 */
class AlarmIndicatorWidget : public QWidget
{
    Q_OBJECT

public:
    enum AlarmLevel {
        NoAlarm,
        Info,
        Warning,
        Critical
    };

    struct AlarmItem {
        QString message;
        AlarmLevel level;
        bool active;
        bool acknowledged;
    };

    explicit AlarmIndicatorWidget(QWidget *parent = nullptr);
    ~AlarmIndicatorWidget() override;

    // Alarm management
    void addAlarm(const QString &message, AlarmLevel level);
    void clearAlarm(const QString &message);
    void acknowledgeAlarm(int index);
    void clearAllAlarms();
    int activeAlarmCount() const;
```

关键实现：`src/AlarmIndicator/AlarmIndicatorWidget.cpp`

构造与定时器设置：

```4:14:src/AlarmIndicator/AlarmIndicatorWidget.cpp
AlarmIndicatorWidget::AlarmIndicatorWidget(QWidget *parent)
    : QWidget(parent)
    , m_flashTimer(new QTimer(this))
    , m_flashState(false)
    , m_scrollOffset(0)
{
    setMinimumSize(200, 100);
    
    connect(m_flashTimer, &QTimer::timeout, this, &AlarmIndicatorWidget::onFlashTimer);
    m_flashTimer->start(500);  // Flash every 500ms
}
```

添加报警（含消息唯一性检查）：

```20:40:src/AlarmIndicator/AlarmIndicatorWidget.cpp
void AlarmIndicatorWidget::addAlarm(const QString &message, AlarmLevel level)
{
    // Check if alarm already exists
    for (auto &alarm : m_alarms) {
        if (alarm.message == message) {
            alarm.active = true;
            alarm.level = level;
            update();
            return;
        }
    }

    // Add new alarm
    AlarmItem item;
    item.message = message;
    item.level = level;
    item.active = true;
    item.acknowledged = false;
    m_alarms.prepend(item);  // Add to top
    update();
}
```

摘要统计绘制：

```105:167:src/AlarmIndicator/AlarmIndicatorWidget.cpp
void AlarmIndicatorWidget::drawSummary(QPainter &painter)
{
    QRect summaryRect(5, 5, width() - 10, 30);
    
    // Draw summary background
    painter.setBrush(QColor(50, 50, 50));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(summaryRect, 3, 3);
    
    // Count alarms by level
    int criticalCount = 0, warningCount = 0, infoCount = 0;
    for (const auto &alarm : m_alarms) {
        if (alarm.active && !alarm.acknowledged) {
            switch (alarm.level) {
            case Critical: criticalCount++; break;
            case Warning: warningCount++; break;
            case Info: infoCount++; break;
            default: break;
            }
        }
    }
    
    // Draw summary text
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(10);
    painter.setFont(font);
    
    QString summaryText = QString("Alarms: ");
    painter.drawText(summaryRect.adjusted(5, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, summaryText);
    
    int xOffset = 70;
    
    // Critical
    if (criticalCount > 0) {
        QRect critRect(xOffset, summaryRect.y() + 7, 50, 16);
        painter.setBrush(getLevelColor(Critical));
        painter.drawRoundedRect(critRect, 2, 2);
        painter.setPen(Qt::white);
        painter.drawText(critRect, Qt::AlignCenter, QString::number(criticalCount));
        xOffset += 55;
    }
    
    // Warning
    if (warningCount > 0) {
        QRect warnRect(xOffset, summaryRect.y() + 7, 50, 16);
        painter.setBrush(getLevelColor(Warning));
        painter.drawRoundedRect(warnRect, 2, 2);
        painter.setPen(Qt::black);
        painter.drawText(warnRect, Qt::AlignCenter, QString::number(warningCount));
        xOffset += 55;
    }
    
    // Info
    if (infoCount > 0) {
        QRect infoRect(xOffset, summaryRect.y() + 7, 50, 16);
        painter.setBrush(getLevelColor(Info));
        painter.drawRoundedRect(infoRect, 2, 2);
        painter.setPen(Qt::white);
        painter.drawText(infoRect, Qt::AlignCenter, QString::number(infoCount));
    }
}
```

报警列表绘制（含闪烁效果）：

```169:239:src/AlarmIndicator/AlarmIndicatorWidget.cpp
void AlarmIndicatorWidget::drawAlarmList(QPainter &painter)
{
    QRect listRect(5, 40, width() - 10, height() - 45);
    int itemHeight = 30;
    int y = listRect.y();
    
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);
    
    int visibleItems = qMin(m_alarms.size(), (listRect.height() / itemHeight));
    
    for (int i = 0; i < visibleItems; ++i) {
        const AlarmItem &alarm = m_alarms[i];
        
        QRect itemRect(listRect.x(), y, listRect.width(), itemHeight - 2);
        
        // Determine background color
        QColor bgColor;
        if (alarm.acknowledged) {
            bgColor = QColor(60, 60, 60);
        } else if (alarm.active) {
            // Flash for unacknowledged alarms
            if (m_flashState && alarm.level >= Warning) {
                bgColor = getLevelColor(alarm.level);
            } else {
                bgColor = getLevelColor(alarm.level).darker(200);
            }
        } else {
            bgColor = QColor(40, 40, 40);
        }
        
        painter.setBrush(bgColor);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(itemRect, 3, 3);
        
        // Draw level indicator
        QRect levelRect(itemRect.x() + 3, itemRect.y() + 3, 60, itemRect.height() - 6);
        painter.setBrush(getLevelColor(alarm.level));
        painter.drawRoundedRect(levelRect, 2, 2);
        
        painter.setPen(alarm.level == Warning ? Qt::black : Qt::white);
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(levelRect, Qt::AlignCenter, getLevelText(alarm.level));
        
        // Draw message
        painter.setPen(Qt::white);
        font.setBold(false);
        painter.setFont(font);
        
        QString displayText = alarm.message;
        QFontMetrics fm(font);
        int maxWidth = itemRect.width() - 75;
        displayText = fm.elidedText(displayText, Qt::ElideRight, maxWidth);
        
        painter.drawText(itemRect.adjusted(70, 0, -5, 0), 
                        Qt::AlignLeft | Qt::AlignVCenter, displayText);
        
        y += itemHeight;
    }
    
    // Draw "No Alarms" if empty
    if (m_alarms.isEmpty()) {
        painter.setPen(QColor(120, 120, 120));
        font.setPointSize(12);
        font.setBold(false);
        painter.setFont(font);
        painter.drawText(listRect, Qt::AlignCenter, "No Active Alarms");
    }
}
```

级别颜色与文本获取：

```260:278:src/AlarmIndicator/AlarmIndicatorWidget.cpp
QColor AlarmIndicatorWidget::getLevelColor(AlarmLevel level) const
{
    switch (level) {
    case Critical: return QColor(255, 0, 0);
    case Warning: return QColor(255, 200, 0);
    case Info: return QColor(0, 150, 255);
    default: return QColor(100, 100, 100);
    }
}

QString AlarmIndicatorWidget::getLevelText(AlarmLevel level) const
{
    switch (level) {
    case Critical: return "CRITICAL";
    case Warning: return "WARNING";
    case Info: return "INFO";
    default: return "NONE";
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
project(AlarmIndicatorMiniDemo LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(AlarmIndicatorMiniDemo
    src/main.cpp
    src/AlarmIndicatorWidget.cpp
    src/AlarmIndicatorWidget.h
)
target_link_libraries(AlarmIndicatorMiniDemo PRIVATE Qt6::Widgets)
```

- **最小 `main.cpp`**：

```cpp
#include <QApplication>
#include "AlarmIndicatorWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    AlarmIndicatorWidget w;
    w.addAlarm("Motor Overload", AlarmIndicatorWidget::Critical);
    w.addAlarm("Temperature High", AlarmIndicatorWidget::Warning);
    w.resize(400, 300);
    w.show();
    return app.exec();
}
```

