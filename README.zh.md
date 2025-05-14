# ST7305 Pico 显示驱动

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi%20Pico-red.svg)](https://www.raspberrypi.com/products/raspberry-pi-pico/)
[![Version](https://img.shields.io/badge/Version-1.0.0-green.svg)](https://github.com/musicaJack/ST7305_2.9_Reflective_Lcd/releases)

[English](README.md) | 中文

这是一个用于树莓派Pico的ST7305反射式LCD显示屏的高性能驱动库。该库提供了全面的显示控制功能，包括图形渲染、文本显示和基本显示操作。

## 特性

- 完整的ST7305显示驱动支持
- 硬件抽象层，便于移植
- 内置图形绘制功能
- 优化的SPI通信
- 用户友好的API接口
- 支持自定义字体

## 项目架构

项目采用模块化设计，包含以下组件：

### 核心模块

- `st7305_driver.hpp/cpp`：核心驱动实现，包括显示初始化和基本控制功能
- `st7305_font.cpp`：字体支持实现
- `st7305_config.hpp`：配置文件，包含引脚定义和显示参数

### 目录结构

```
├── src/                    # 源代码目录
│   ├── st7305_driver.cpp  # 核心驱动实现
│   └── st7305_font.cpp    # 字体支持
├── include/               # 头文件目录
│   ├── st7305_driver.hpp # 核心驱动头文件
│   └── st7305_config.hpp # 配置文件
├── examples/              # 示例代码
├── build/                 # 构建输出目录
└── CMakeLists.txt        # CMake构建配置
```

## 快速开始

### 硬件连接

1. 将ST7305显示屏连接到树莓派Pico：
   - SCK: GPIO18
   - MOSI: GPIO19
   - CS: GPIO17
   - DC: GPIO20
   - RST: GPIO15
   - VCC: 3.3V
   - GND: GND

连接图：
```
树莓派Pico              ST7305显示屏
+---------------+         +---------------+
|               |         |               |
|  GPIO18 (SCK) |-------->| SCK          |
|  GPIO19 (MOSI)|-------->| MOSI         |
|  GPIO17 (CS)  |-------->| CS           |
|  GPIO20 (DC)  |-------->| DC           |
|  GPIO15 (RST) |-------->| RST          |
|  3.3V         |-------->| VCC          |
|  GND          |-------->| GND          |
|               |         |               |
+---------------+         +---------------+
```

注意事项：
1. 确保所有连接牢固，避免松动
2. 通电前仔细检查电源连接
3. 如果显示不正常，请检查：
   - 电源电压是否稳定在3.3V
   - 所有信号连接是否正确
   - SPI通信时序是否正确

### 软件设置

1. 克隆仓库：
```bash
git clone https://github.com/musicaJack/ST7305_2.9_Reflective_Lcd.git
cd ST7305_2.9_Reflective_Lcd
```

2. 构建项目：
```bash
./build_pico.bat
```

## API使用指南

### 基本配置

```cpp
#include "st7305_driver.hpp"

// 创建显示对象
st7305::ST7305Driver display(DC_PIN, RES_PIN, CS_PIN, SCK_PIN, SDIN_PIN);

// 初始化显示
display.initialize();
```

### 基本显示操作

```cpp
// 清屏
display.clear();

// 填充屏幕
display.fill(0xFF);

// 显示开关
display.displayOn(true);
display.displayOn(false);

// 睡眠模式
display.displaySleep(true);
display.displaySleep(false);

// 显示反转
display.displayInversion(true);
display.displayInversion(false);
```

### 图形绘制

```cpp
// 绘制像素
display.drawPixel(x, y, true);

// 绘制字符
display.drawChar(x, y, 'A', true);

// 绘制字符串
display.drawString(x, y, "Hello World!", true);
```

### 电源管理

```cpp
// 低功耗模式
display.lowPowerMode();

// 高功耗模式
display.highPowerMode();
```

## 常见问题

1. 显示不工作怎么办？
   - 检查电源电压是否为3.3V
   - 确认所有引脚连接正确
   - 验证SPI通信是否正常

2. 显示效果不理想怎么办？
   - 检查初始化序列是否正确
   - 确认显示区域设置是否正确
   - 验证像素写入逻辑是否正确

3. 如何降低功耗？
   - 使用低功耗模式
   - 适当降低SPI通信速率
   - 不使用显示时进入睡眠模式

## 贡献

欢迎贡献代码！请随时提交Pull Request。提交代码前，请确保：

1. 代码符合项目编码规范
2. 添加了必要的注释和文档
3. 所有测试都能通过

## 许可证

本项目采用MIT许可证。详见[LICENSE](LICENSE)文件。

## 联系方式

如有问题或建议，请联系：

- 邮箱：yinyue@beingdigital.cn

## 致谢

感谢所有为本项目做出贡献的人。 