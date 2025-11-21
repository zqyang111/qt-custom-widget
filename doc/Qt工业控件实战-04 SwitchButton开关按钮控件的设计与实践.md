### Qt工业控件实战·第04篇｜SwitchButton 开关按钮控件的设计与实践

### 写在前面

在工业控制界面中，开关按钮是最基础也最常用的交互控件之一。从设备启停、模式切换到功能开关，开关按钮以“滑动切换”这一直观的交互方式，让操作员能够快速、准确地执行二元操作。传统的复选框或单选按钮虽然功能完整，但缺乏工业界面的视觉冲击力；而开关按钮控件以“轨道+滑块+状态文字”这一现代 UI 设计，在保留易用性的同时提升了视觉质感。本文基于本项目的 `SwitchButtonWidget`，深入解析其设计思路、绘制技巧与工程实践，帮助你在 Qt 应用中快速集成一款专业的工业开关按钮控件。

![](../picture/switch_button.png)

### 组件定位与典型场景

- **设备启停控制**：电机、泵、压缩机等设备的启动/停止控制，需要明确的视觉反馈。
- **模式切换**：自动/手动模式、本地/远程控制等二元状态切换，降低误操作风险。
- **功能开关**：报警使能、数据记录、通信使能等功能开关，支持批量配置。
- **状态指示**：在仪表盘中显示关键设备或功能的开关状态，便于快速查看。

### 视觉结构拆解

`SwitchButtonWidget` 的画面由三个层次组成：

1. **背景轨道（Background Track）**：圆角矩形轨道，颜色随状态变化（灰色/绿色），采用内阴影增强立体感。
2. **状态文字（ON/OFF Text）**：轨道两侧显示 "ON" 和 "OFF" 文字，当前状态侧文字可见，另一侧被滑块遮挡。
3. **滑块（Handle）**：圆形滑块，采用径向渐变模拟立体按钮，带有阴影效果，在轨道两端滑动。

这种“轨道+滑块”的设计既符合现代 UI 设计规范，又保持了工业界面的简洁与专业。

### API 设计与可用性

`SwitchButtonWidget` 提供了简洁直观的接口：

- **状态设置**：`setChecked(bool checked)` - 设置开关状态（true=ON, false=OFF），自动触发重绘并发射信号。
- **启用/禁用**：`setEnabled(bool enabled)` - 设置控件是否可用，禁用时显示灰色并阻止交互。
- **切换状态**：`toggle()` - 切换当前状态，用于程序化控制。

核心原则是“状态驱动显示”；内部会在状态变化时自动更新颜色、滑块位置与文字显示，确保视觉与状态的一致性。

### 状态映射与颜色联动

- 状态计算：布尔值 `m_checked` 直接映射到滑块位置（false=左侧，true=右侧）。
- 颜色逻辑：
  - 关闭状态（checked = false）：灰色（RGB 150, 150, 150），传达“未激活、关闭”的语义。
  - 开启状态（checked = true）：绿色（RGB 0, 200, 0），表示“激活、开启”。
  - 禁用状态（enabled = false）：深灰色（RGB 80, 80, 80），提示“不可操作”。
- 滑块位置：根据状态计算滑块 X 坐标，关闭时在左侧（x=4），开启时在右侧（x=width-handleSize-4）。

这种映射策略简单直接，适用于所有二元状态切换场景。

### 细节打磨：绘制技巧与真实感

- **圆角轨道**：使用 `drawRoundedRect` 绘制轨道，圆角半径设为高度的一半，形成流畅的视觉边界。
- **内阴影效果**：在轨道内部绘制半透明黑色矩形，形成内阴影，增强立体感与层次。
- **径向渐变滑块**：滑块采用径向渐变，中心亮（白色）到边缘暗（灰色），模拟真实按钮的立体感。
- **滑块阴影**：在滑块下方绘制半透明黑色椭圆，形成投影效果，增强浮起感。
- **状态文字**：使用粗体白色文字，在深色轨道上形成高对比度，确保可读性。
- **光标反馈**：启用时显示手型光标，禁用时显示禁止光标，提供交互反馈。

### 性能与适配

- **抗锯齿**：启用 `QPainter::Antialiasing`，在圆角、椭圆与渐变绘制下保持平滑边缘。
- **尺寸自适应**：轨道高度、滑块大小、文字位置均基于控件宽高比例计算，确保在不同尺寸下保持视觉比例一致。
- **增量重绘**：仅在状态变化或参数更新时调用 `update()`，避免无谓重绘；这对频繁切换场景尤为重要。
- **最小尺寸约束**：通过 `minimumSizeHint()` 返回 `QSize(60, 30)`，确保控件在小尺寸下仍可清晰显示。

### 交互设计

- **鼠标按下**：记录按下状态，更新视觉反馈（可选：滑块略微缩小或变色）。
- **鼠标释放**：切换状态，发射 `toggled(bool)` 和 `clicked()` 信号，触发业务逻辑。
- **禁用状态**：禁用时阻止所有鼠标交互，显示灰色，光标变为禁止图标。
- **信号机制**：提供 `toggled(bool)` 和 `clicked()` 信号，支持状态变化监听与点击事件处理。

### 可扩展方向

- **动画过渡**：在 `setChecked` 时通过 `QPropertyAnimation` 实现滑块滑动的缓动效果，提升“物理感”。
- **自定义颜色**：开放开启/关闭/禁用颜色参数，支持主题切换与品牌定制。
- **图标支持**：在轨道两侧或滑块上显示图标（如电源图标、锁图标），增强语义表达。
- **多状态支持**：扩展为三态开关（关闭/中间/开启），用于更复杂的控制场景。
- **尺寸变体**：提供小、中、大三种尺寸预设，适应不同界面密度需求。
- **键盘支持**：支持空格键切换状态，提升可访问性。

### 与业务集成

- **信号槽连接**：通过 `toggled(bool)` 信号连接业务逻辑，实现状态同步与设备控制。
- **状态持久化**：结合配置文件或数据库，保存开关状态，实现状态恢复。
- **权限控制**：根据用户权限动态设置 `enabled` 状态，实现操作权限管理。
- **批量操作**：在仪表盘中并排显示多个开关，支持批量配置与状态查看。
- **国际化与可访问性**：状态文字（ON/OFF）可配置；颜色同时配合文字/图标，覆盖色弱/高对比场景。

### 简要用法示例

```cpp
// 创建与基础配置
auto switchBtn = new SwitchButtonWidget(this);
switchBtn->setChecked(true);  // Set to ON

// 连接信号
connect(switchBtn, &SwitchButtonWidget::toggled, this, [](bool checked) {
    if (checked) {
        // Handle ON state
        qDebug() << "Switch turned ON";
    } else {
        // Handle OFF state
        qDebug() << "Switch turned OFF";
    }
});

// 程序化切换
switchBtn->toggle();
```

> 注：信号槽机制确保线程安全，可在不同线程中安全使用。

### 设计取舍回顾

- **选择滑动式设计**：相比传统复选框，滑动式开关更具现代感与视觉冲击力，且符合移动端用户习惯。
- **固定颜色方案**：采用绿色/灰色/深灰色的固定配色，符合工业界通用认知，但可通过扩展支持自定义颜色。
- **状态文字显示**：在轨道上显示 ON/OFF 文字，增强可读性，但占用空间；可通过配置选项隐藏。
- **禁用状态处理**：禁用时显示深灰色并阻止交互，避免误操作，但需注意与正常关闭状态的视觉区分。

### 结语

`SwitchButtonWidget` 以直观的交互方式和清晰的视觉反馈，覆盖了工业控制中“开关控制、状态指示”的核心需求；同时保持足够的扩展空间，便于适配不同产品线的主题风格与业务规则。  
如果你正在打造一个既专业又易用的可视化界面，不妨把它作为“开关控制”的基础组件，再根据项目特性逐步演进：加动画、加图标、加多状态与键盘支持。期待你基于它构建出更具表现力的工业 UI。

### 附录｜源码

源码放置方式：
- **文内"关键片段"**：展示核心接口与关键绘制逻辑，控制篇幅。
- **仓库/附件"完整源码"**：附仓库链接或网盘附件，标注 commit 与分支。
- **标注文件路径与版本**：便于在工程中定位。
- **构建指引简述**：包含 Qt 版本、CMake 配置与最小可运行示例。

以下为关键片段预览（完整代码请见工程仓库）：

关键头文件：`src/SwitchButton/SwitchButtonWidget.h`

```8:53:src/SwitchButton/SwitchButtonWidget.h
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
```

关键实现：`src/SwitchButton/SwitchButtonWidget.cpp`

构造与基础参数：

```3:14:src/SwitchButton/SwitchButtonWidget.cpp
SwitchButtonWidget::SwitchButtonWidget(QWidget *parent)
    : QWidget(parent)
    , m_checked(false)
    , m_enabled(true)
    , m_pressed(false)
    , m_onColor(QColor(0, 200, 0))
    , m_offColor(QColor(150, 150, 150))
    , m_disabledColor(QColor(80, 80, 80))
{
    setMinimumSize(60, 30);
    setCursor(Qt::PointingHandCursor);
}
```

状态设置与信号发射：

```20:27:src/SwitchButton/SwitchButtonWidget.cpp
void SwitchButtonWidget::setChecked(bool checked)
{
    if (m_checked != checked) {
        m_checked = checked;
        update();
        emit toggled(m_checked);
    }
}
```

背景轨道绘制（含颜色联动与内阴影）：

```54:90:src/SwitchButton/SwitchButtonWidget.cpp
void SwitchButtonWidget::drawBackground(QPainter &painter)
{
    int h = height();
    int w = width();
    int radius = h / 2;

    // Choose color based on state
    QColor bgColor;
    if (!m_enabled)
        bgColor = m_disabledColor;
    else if (m_checked)
        bgColor = m_onColor;
    else
        bgColor = m_offColor;

    // Draw background track
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
    painter.drawRoundedRect(0, 0, w, h, radius, radius);

    // Add inner shadow
    painter.setBrush(QColor(0, 0, 0, 50));
    painter.drawRoundedRect(2, 2, w - 4, h - 4, radius - 2, radius - 2);

    // Draw ON/OFF text
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(8);
    font.setBold(true);
    painter.setFont(font);

    if (m_checked) {
        painter.drawText(QRect(5, 0, w / 2, h), Qt::AlignCenter, "ON");
    } else {
        painter.drawText(QRect(w / 2, 0, w / 2 - 5, h), Qt::AlignCenter, "OFF");
    }
}
```

滑块绘制（含阴影与渐变）：

```92:118:src/SwitchButton/SwitchButtonWidget.cpp
void SwitchButtonWidget::drawHandle(QPainter &painter)
{
    int h = height();
    int w = width();
    int handleSize = h - 8;
    int handleX;

    if (m_checked)
        handleX = w - handleSize - 4;
    else
        handleX = 4;

    // Draw shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 100));
    painter.drawEllipse(handleX + 2, 6, handleSize, handleSize);

    // Draw handle
    QRadialGradient gradient(handleX + handleSize / 2, 4 + handleSize / 2, handleSize / 2);
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.7, QColor(240, 240, 240));
    gradient.setColorAt(1.0, QColor(200, 200, 200));

    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(150, 150, 150), 1));
    painter.drawEllipse(handleX, 4, handleSize, handleSize);
}
```

鼠标事件处理：

```120:137:src/SwitchButton/SwitchButtonWidget.cpp
void SwitchButtonWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_enabled && event->button() == Qt::LeftButton) {
        m_pressed = true;
        update();
    }
    QWidget::mousePressEvent(event);
}

void SwitchButtonWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_enabled && m_pressed && event->button() == Qt::LeftButton) {
        m_pressed = false;
        toggle();
        emit clicked();
    }
    QWidget::mouseReleaseEvent(event);
}
```

### 最小构建与集成指引

最小可运行步骤：

- **环境**：Qt 6.x（或 Qt 5.15+），CMake 3.20+，编译器与平台按团队规范即可。
- **依赖**：无额外三方依赖，Qt Widgets 模块即可。
- **CMake 示例**：

```cmake
cmake_minimum_required(VERSION 3.20)
project(SwitchButtonMiniDemo LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(SwitchButtonMiniDemo
    src/main.cpp
    src/SwitchButtonWidget.cpp
    src/SwitchButtonWidget.h
)
target_link_libraries(SwitchButtonMiniDemo PRIVATE Qt6::Widgets)
```

- **最小 `main.cpp`**：

```cpp
#include <QApplication>
#include "SwitchButtonWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    SwitchButtonWidget w;
    w.setChecked(true);
    w.resize(100, 50);
    w.show();
    return app.exec();
}
```

