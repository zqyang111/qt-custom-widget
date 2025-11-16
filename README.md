<img width="1402" height="932" alt="demo1" src="https://github.com/user-attachments/assets/0973a336-b169-45d5-92ee-18ca929c1147" /><img width="1202" height="1067" alt="demo13" src="https://github.com/user-attachments/assets/dfdcf492-1791-4399-a24b-371517563f86" /># 工业仪表控件库

## 概述

**工业仪表控件库（Industrial Instrument Control Library）** 是一套专为工业控制软件开发设计的高品质Qt控件集合。包含**44个**精心设计的工业级可视化控件，涵盖显示、控制、监控、分析等多个领域，是开发工业自动化、测试测量、数据采集、过程监控等应用的理想选择。

---

## 核心优势

###  专业的工业设计
- **专业工业风格**界面设计，符合工业软件审美标准
- **深色主题**优化，适合长时间监控使用
- **高对比度**显示，确保关键信息清晰可见
- **平滑动画**效果，提升用户体验

###  开箱即用
- **零依赖**配置，基于Qt 6框架
- **完整示例**代码，快速上手
- **模块化设计**，按需集成
- **跨平台支持**，Windows/Linux/macOS

###  高性能实时更新
- **优化渲染**算法，50Hz以上刷新率
- **低CPU占用**，支持多控件并发
- **流畅动画**，无卡顿
- **内存优化**，适合长时间运行

###  易于集成
- **标准Qt信号槽**机制
- **丰富的API**接口
- **灵活的配置**选项
- **完善的文档**支持

---

## 演示截图

以下是ComponentDemo演示程序的界面截图：

<img width="1402" height="932" alt="demo1" src="https://github.com/user-attachments/assets/65fc6c22-36b2-4135-b20c-00cf3ecf92f1" />


![Demo 1](picture/demo1.png)

<div style="page-break-after: always;"></div>

![Demo 2](picture/demo2.png)

<div style="page-break-after: always;"></div>

![Demo 3](picture/demo3.png)

<div style="page-break-after: always;"></div>

![Demo 4](picture/demo4.png)

<div style="page-break-after: always;"></div>

![Demo 5](picture/demo5.png)

<div style="page-break-after: always;"></div>

![Demo 6](picture/demo6.png)

<div style="page-break-after: always;"></div>

![Demo 7](picture/demo7.png)

<div style="page-break-after: always;"></div>

![Demo 8](picture/demo8.png)

<div style="page-break-after: always;"></div>

![Demo 9](picture/demo9.png)

<div style="page-break-after: always;"></div>

![Demo 10](picture/demo10.png)

<div style="page-break-after: always;"></div>

![Demo 11](picture/demo11.png)

<div style="page-break-after: always;"></div>

![Demo 12](picture/demo12.png)

<div style="page-break-after: always;"></div>

![Demo 13](picture/demo13.png)

---

## 组件清单（44个控件）

### ? 显示类组件（8个）

#### 1. **Gauge（仪表盘）**
![Gauge](picture/gauge.png)
- 经典指针式仪表，支持范围、单位、警告区设置
- 适用于：压力、速度、转速等参数显示

#### 2. **Thermometer（温度计）**
![Thermometer](picture/thermometer.png)
- 模拟水银温度计设计
- 支持华氏/摄氏度切换，警告区域高亮

#### 3. **ProgressBar（进度条）**
![ProgressBar](picture/progress_bar.png)
- 工业风格进度条，支持百分比、数值显示
- 适用于：任务进度、完成度展示

#### 4. **LED（LED指示灯）**
![LED](picture/led.png)
- 多色LED指示，支持常亮、闪烁、熄灭三种状态
- 颜色：红、黄、绿、蓝等多种选择

#### 5. **DigitalMeter（数字仪表）**
![DigitalMeter](picture/digital_meter.png)
- 高精度数字显示，支持自定义精度和单位
- 适用于：功率、电压、电流等精确测量

#### 6. **LevelIndicator（液位指示器）**
![LevelIndicator](picture/level_indicator.png)
- 直观的液位显示，支持百分比和容量单位
- 颜色分级显示，一目了然

#### 7. **SevenSegment（七段数码管）**
![SevenSegment](picture/seven_segment.png)
- 经典数码管显示，支持多位数字
- 复古工业风格，适合计数器、计时器

#### 8. **BarGraph（柱状图）**
![BarGraph](picture/bar_graph.png)
- 实时柱状图，支持多系列对比
- 适用于：多通道数据对比

---

### ?? 控制类组件（3个）

#### 9. **SwitchButton（开关按钮）**
![SwitchButton](picture/switch_button.png)
- 拟物化开关设计，开/关状态清晰
- 适用于：设备启停控制

#### 10. **Knob（旋钮控制器）**
![Knob](picture/knob_control.png)
- 旋转旋钮控制，支持鼠标拖拽和滚轮
- 适用于：音量、速度、参数调节

#### 11. **IndustrialSlider（工业滑块）**
![IndustrialSlider](picture/industrial_slider.png)
- 工业级滑块控制，支持刻度显示
- 适用于：精确参数调整

---

### ?? 过程控制组件（5个）

#### 12. **Valve（阀门）**
![Valve](picture/valve_control.png)
- 支持球阀、蝶阀等多种类型
- 实时显示开度百分比

#### 13. **Pump（泵）**
![Pump](picture/pump.png)
- 旋转动画显示泵运行状态
- 支持启停控制和转速调节

#### 14. **Tank（储罐）**
![Tank](picture/tank.png)
- 圆柱形/矩形储罐可选
- 实时显示液位、温度、压力

#### 15. **Pipe（管道）**
![Pipe](picture/pipe_flow.png)
- 流体流动动画
- 支持正向/反向流动，流速可调

#### 16. **FlowMeter（流量计）**
![FlowMeter](picture/flow_meter.png)
- 实时流量显示
- 累计流量统计，支持清零

---

### ? 监控分析组件（3个）

#### 17. **TrendChart（趋势图）**
![TrendChart](picture/trend_chart.png)
- 多通道实时趋势记录
- 自动缩放，历史数据回放

#### 18. **Speedometer（速度表）**
![Speedometer](picture/speedo_meter.png)
- 汽车风格速度表
- 警告区、红线区设置

#### 19. **AlarmIndicator（报警指示器）**
![AlarmIndicator](picture/aralm_indicator.png)
- 分级报警显示（严重/警告/信息）
- 时间戳记录，报警历史

---

### ?? 时间导航组件（4个）

#### 20. **AnalogClock（模拟时钟）**
![AnalogClock](picture/analog_clock.png)
- 经典指针式时钟
- 实时系统时间显示

#### 21. **DigitalClock（数字时钟）**
![DigitalClock](picture/digital_clock.png)
- 数字式时钟显示
- 支持12/24小时制

#### 22. **TimerWidget（计时器）**
![TimerWidget](picture/timer_stopwatch.png)
- 秒表/倒计时双模式
- 适用于：过程计时、倒计时提醒

#### 23. **Compass（指南针）**
![Compass](picture/compass.png)
- 方位角度指示
- 适用于：方向导航、角度显示

---

### ? 电气能源组件（3个）

#### 24. **MotorController（电机控制器）**
![MotorController](picture/motor_control.png)
- 电机状态监控（正转/反转/停止）
- 转速、扭矩、功率实时显示

#### 25. **BatteryIndicator（电池指示器）**
![BatteryIndicator](picture/battery_indicator.png)
- 电池电量、电压、电流显示
- 充电/放电状态动画

#### 26. **ThreePhaseMeter（三相电表）**
![ThreePhaseMeter](picture/three_phase_meter.png)
- ABC三相电压电流监控
- 频率、功率因数显示

---

### ? 机械组件（4个）

#### 27. **Gear（齿轮）**
![Gear](picture/gear_train.png)
- 齿轮传动动画
- 转速可调，支持多级传动

#### 28. **BeltDrive（皮带传动）**
![BeltDrive](picture/belt_drive.png)
- 皮带轮传动系统动画
- 实时转速显示

#### 29. **Piston（活塞）**
![Piston](picture/piston_engine.png)
- 活塞运动动画
- 压力、转速监控

#### 30. **Pendulum（摆锤）**
![Pendulum](picture/Pendulum.png)
- 物理摆动仿真
- 幅度、阻尼可调

---

### ? 图表分析组件（14个）

#### 31. **WaveformChart（波形图）**
![WaveformChart](picture/waveform_chart.png)
- 专业级滚动波形显示
- 适用于：信号监控、示波器应用

#### 32. **XYGraph（XY图）**
![XYGraph](picture/xy_graph.png)
- X-Y坐标绘图
- 适用于：轨迹显示、相位图

#### 33. **IntensityChart（强度热图）**
![IntensityChart](picture/intensity_chart.png)
- 2D强度分布显示
- 支持Rainbow、Heat、灰度等颜色映射

#### 34. **PieChart（饼图）**
![PieChart](picture/pie_chart.png)
- 数据占比可视化
- 支持百分比、图例显示

#### 35. **LineChart（折线图）**
![LineChart](picture/line_chart.png)
- 多系列折线对比
- 适用于：趋势分析、数据对比

#### 36. **Histogram（直方图）**
![Histogram](picture/histogram.png)
- 统计分布图
- 自动分箱，统计信息显示

#### 37. **PolarPlot（极坐标图）**
![PolarPlot](picture/polar_plot.png)
- 极坐标数据显示
- 适用于：雷达扫描、天线方向图

#### 38. **SpectrumAnalyzer（频谱分析仪）**
![SpectrumAnalyzer](picture/spectrum_analyzer.png)
- 频域分析显示
- 支持峰值保持功能

#### 39. **ScatterPlot（散点图）**
![ScatterPlot](picture/scatter_plot.png)
- 数据相关性分析
- 支持趋势线拟合

#### 40. **AreaChart（区域图）**
![AreaChart](picture/area_chart.png)
- 填充式趋势图
- 支持堆叠/覆盖模式

#### 41. **RadarChart（雷达图）**
![RadarChart](picture/radar_chart.png)
- 多维数据对比
- 适用于：性能评估、能力图

#### 42. **BubbleChart（气泡图）**
![BubbleChart](picture/bubble_chart.png)
- 三变量可视化（X、Y、大小）
- 适用于：多维数据分析

#### 43. **DigitalWaveform（数字波形）**
![DigitalWaveform](picture/digital_waveform.png)
- 逻辑信号时序图
- 适用于：数字电路分析、协议分析

#### 44. **StackedBarChart（堆叠柱状图）**
![StackedBarChart](picture/stacked_bar_chart.png)
- 组合数据堆叠显示
- 适用于：构成分析、对比展示

---

## 技术规格

### 开发环境
- **框架**: Qt 6.x
- **语言**: C++ 17
- **构建**: CMake 3.20+
- **平台**: Windows / Linux / macOS

### 性能指标
- **刷新率**: 最高支持50Hz+
- **响应时间**: < 20ms
- **CPU占用**: 单控件 < 1%
- **内存占用**: 单控件 < 5MB

### 代码质量
- **架构**: 面向对象设计
- **规范**: 遵循Qt编码规范
- **注释**: 完整的英文代码注释
- **示例**: 每个组件配套完整Demo

---

## 应用场景

### ? 工业自动化
- SCADA系统开发
- PLC上位机软件
- 工业过程监控
- 设备状态管理

### ? 测试测量
- 数据采集系统
- 虚拟仪器开发
- 信号分析软件
- 测试平台界面

### ? 能源管理
- 智能电网监控
- 能耗管理系统
- 配电监控
- 新能源监测

### ? 交通运输
- 车辆仪表盘
- 调度监控系统
- 导航显示
- 状态监测

### ? 医疗设备
- 监护仪界面
- 医疗设备控制面板
- 实验室仪器
- 数据可视化

---

## 产品特色功能

### 1. 完整的演示程序
随产品提供三个完整的Demo应用：
- **MainApp**: 集成式主应用，展示所有基础控件
- **ChartDemo**: 图表组件专项演示
- **ComponentDemo**: 按钮式交互演示（适合客户展示）

### 2. 灵活的主题支持
- 深色主题（默认）
- 浅色主题
- 自定义配色方案
- CSS样式表定制

### 3. 丰富的动画效果
- 平滑的数值过渡动画
- 流体流动动画
- 旋转运动动画
- LED闪烁效果
- 波形滚动显示

### 4. 实时数据绑定
- 信号槽机制，数据实时更新
- 支持高频数据刷新
- 自动缩放功能
- 历史数据缓存

### 5. 多语言支持
- 国际化框架（i18n）
- 支持中英文切换
- 易于扩展其他语言

---

## 产品组件分类详解

### ? 显示组件系列（8个）
专注于数据的可视化呈现，提供多种展示形式：

| 组件名称 | 主要功能 | 典型应用 |
|---------|---------|---------|
| Gauge | 指针式仪表 | 压力、温度、速度监控 |
| Thermometer | 温度计显示 | 温度监测 |
| ProgressBar | 进度展示 | 任务进度、完成度 |
| LED | 状态指示灯 | 设备状态、报警提示 |
| DigitalMeter | 数字表头 | 高精度数值显示 |
| LevelIndicator | 液位指示 | 液体容器监控 |
| SevenSegment | 数码管 | 计数器、计时器 |
| BarGraph | 条形图 | 多通道数据对比 |

**特点**: 实时更新、警告区域、自定义单位、颜色分级

---

### ?? 控制组件系列（3个）
提供用户交互控制界面：

| 组件名称 | 交互方式 | 精度控制 |
|---------|---------|---------|
| SwitchButton | 拨动开关 | ON/OFF |
| Knob | 旋转调节 | 连续可调 |
| IndustrialSlider | 滑块调节 | 带刻度精确调节 |

**特点**: 拟物化设计、平滑响应、双向数据绑定

---

### ? 过程控制组件系列（5个）
模拟实际工业设备：

| 组件名称 | 设备类型 | 动画效果 |
|---------|---------|---------|
| Valve | 球阀/蝶阀 | 开度动画 |
| Pump | 离心泵 | 旋转动画 |
| Tank | 储罐 | 液位变化 |
| Pipe | 管道 | 流体流动 |
| FlowMeter | 流量计 | 瞬时/累计流量 |

**特点**: 仿真动画、状态反馈、参数监控

---

### ? 监控分析组件系列（3个）
专业级数据监控工具：

- **TrendChart**: 多通道趋势记录，支持100+数据点
- **Speedometer**: 速度监控，警告区/红线区设置
- **AlarmIndicator**: 分级报警系统，时间戳记录

**特点**: 历史数据、自动缩放、多通道并发

---

### ? 时间导航组件系列（4个）
时间和方位显示：

- **AnalogClock**: 模拟时钟
- **DigitalClock**: 数字时钟
- **TimerWidget**: 秒表/倒计时
- **Compass**: 电子罗盘

**特点**: 实时更新、高精度、直观显示

---

### ? 电气能源组件系列（3个）
电力系统监控专用：

- **MotorController**: 电机控制面板（转速/扭矩/功率）
- **BatteryIndicator**: 电池状态监控（电量/电压/电流）
- **ThreePhaseMeter**: 三相电力参数（电压/电流/频率/功率因数）

**特点**: 专业参数、实时监控、状态动画

---

### ? 机械组件系列（4个）
机械运动仿真：

- **Gear**: 齿轮传动系统
- **BeltDrive**: 皮带轮传动
- **Piston**: 活塞往复运动
- **Pendulum**: 摆锤运动

**特点**: 物理仿真、流畅动画、参数可调

---

### ? 图表组件系列（14个）
专业级数据分析图表：

#### 实时波形类
- **WaveformChart**: 滚动波形图
- **XYGraph**: X-Y坐标图
- **DigitalWaveform**: 数字逻辑波形

#### 统计分析类
- **PieChart**: 饼图
- **LineChart**: 折线图
- **AreaChart**: 区域图
- **Histogram**: 直方图
- **StackedBarChart**: 堆叠柱状图

#### 高级可视化
- **IntensityChart**: 强度热图
- **PolarPlot**: 极坐标图
- **SpectrumAnalyzer**: 频谱分析
- **ScatterPlot**: 散点图
- **RadarChart**: 雷达图
- **BubbleChart**: 气泡图

**特点**: 多通道、自动缩放、图例支持、网格线、数据导出

---

## 技术支持与服务

### ? 完善的文档
- 快速入门指南
- API参考手册
- 示例代码集
- 常见问题解答

### ?? 技术服务
- **在线咨询**: 即时通讯技术答疑
- **远程协助**: 集成问题远程支持
- **定制开发**: 根据客户需求定制专业组件（收费服务）
  - 新组件开发：按需求评估报价
  - 现有组件扩展：增加特殊功能
  - 项目整合服务：提供完整解决方案

### ? 持续更新
- **免费升级**: 购买后一年内免费升级
- **Bug修复**: 及时修复已知问题
- **新组件更新**: 后续开发的新组件**永久免费更新**
- **性能优化**: 持续性能改进
- **版本兼容**: 保证向后兼容性

---

## 快速开始

### 环境要求
```
Qt 6.x
CMake 3.20+
C++ 17编译器
```

### 集成步骤
```cmake
# 1. 添加到CMakeLists.txt
add_subdirectory(InstrumentControl)

# 2. 链接所需组件
target_link_libraries(YourApp
    GaugeWidget
    ThermometerWidget
    # ... 其他组件
)

# 3. 在代码中使用
#include "GaugeWidget.h"

auto gauge = new GaugeWidget(parent);
gauge->setRange(0, 100);
gauge->setValue(50);
```

### 3行代码，即刻运行
```cpp
GaugeWidget *gauge = new GaugeWidget(parent);
gauge->setRange(0, 100);
gauge->setValue(75);
```

---

## 为什么选择我们

### ? 高性价比
- 一次购买，终身使用
- 免费技术支持
- 定期功能更新

### ? 安全可靠
- 无后门、无遥测
- 源码授权
- 符合工业标准

### ? 易于使用
- 标准Qt接口
- 丰富的示例
- 详细的文档

---

## 立即体验

### ? 在线演示
下载 **ComponentDemo.exe** 即刻体验全部44个组件的交互式演示！

### ? 联系我们
- **咨询**: WX: yanzq987 



---

## 常见问题

**Q: 是否支持Qt 5？**  
A: 当前版本基于Qt 6，如有Qt 5需求可联系定制。

**Q: 可以商业使用吗？**  
A: 购买后可用于商业项目。

**Q: 是否提供源码？**  
A: 提供源码授权选项，包含完整源代码。

**Q: 能否定制新组件？**  
A: 可以，我们提供定制开发服务。


**Q: 后续新组件是否免费？**  
A: 是的！购买后，我们未来开发的新组件将永久免费提供给所有客户。

**Q: 可以定制特殊组件吗？**  
A: 可以！我们提供专业的定制开发服务。根据您的行业需求，我们可以开发专属组件，费用根据复杂度单独评估。常见定制如：特殊传感器显示、行业专用图表、定制动画效果等。

---

## 立即行动

? **现在下载ComponentDemo演示程序，亲自体验44个专业工业控件！**

? 联系我们获取正式报价和技术方案, WX: yanzq987 

---

*工业仪表控件库 - 让工业软件开发更简单、更高效、更专业！*

**版权所有 ? 2025 | 保留所有权利**

