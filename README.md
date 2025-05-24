# ST73xx Reflective LCD Driver Library

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi%20Pico-red.svg)](https://www.raspberrypi.com/products/raspberry-pi-pico/)
[![Version](https://img.shields.io/badge/Version-2.0.0-green.svg)](https://github.com/musicaJack/ST7305_2.9_Reflective_Lcd/releases)

English | [中文](README.zh.md)

A comprehensive, high-performance display driver library for ST7305 and ST7306 reflective LCD displays on Raspberry Pi Pico. This library provides a complete graphics framework with hardware abstraction, optimized rendering, and easy-to-use APIs.

## ✨ Features

- **Multi-Device Support**: Full support for both ST7305 and ST7306 reflective LCD controllers
- **Graphics Framework**: Complete UI abstraction layer with Adafruit GFX-style API
- **Advanced Graphics**: Built-in drawing functions for shapes, lines, text, and complex animations
- **Template Design**: Modern C++ template-based architecture for type safety and performance
- **Hardware Abstraction**: Clean separation between hardware drivers and graphics rendering
- **Optimized SPI**: High-performance SPI communication with configurable parameters
- **Font System**: Built-in font support with customizable layouts and sizes
- **Color Management**: Support for grayscale and monochrome rendering modes
- **Rotation Support**: Display rotation and coordinate transformation
- **Power Management**: Low-power and high-power modes for energy efficiency

## 🏗️ Project Architecture

The project follows a modern modular design with clear separation of concerns:

### Core Components

- **Hardware Drivers** (`st7305_driver.cpp`, `st7306_driver.cpp`): Low-level display controller implementations
- **UI Abstraction** (`st73xx_ui.cpp/hpp`): Hardware-agnostic graphics interface (Adafruit GFX-style)
- **Graphics Engine** (`pico_display_gfx.hpp/inl`): Template-based graphics rendering engine
- **Font System** (`fonts/st73xx_font.cpp`): Comprehensive font rendering with layout options
- **Examples** (`examples/`): Comprehensive demo applications showcasing features

### Directory Structure

```
├── src/                           # Source code directory
│   ├── st7305_driver.cpp         # ST7305 controller driver
│   ├── st7306_driver.cpp         # ST7306 controller driver (NEW)
│   ├── st73xx_ui.cpp             # UI abstraction layer (NEW)
│   └── fonts/
│       └── st73xx_font.cpp       # Font data and rendering (ENHANCED)
├── include/                       # Header files directory
│   ├── st7305_driver.hpp         # ST7305 driver interface
│   ├── st7306_driver.hpp         # ST7306 driver interface (NEW)
│   ├── st73xx_ui.hpp             # UI abstraction interface (NEW)
│   ├── pico_display_gfx.hpp      # Template graphics engine (NEW)
│   ├── pico_display_gfx.inl      # Template implementation (NEW)
│   ├── st73xx_font.hpp           # Font system interface (NEW)
│   └── gfx_colors.hpp            # Color definitions (NEW)
├── examples/                      # Example applications
│   ├── st7305_demo.cpp           # ST7305 comprehensive demo (ENHANCED)
│   └── st7306_demo.cpp           # ST7306 demonstration (NEW)
├── build/                         # Build output directory
├── CMakeLists.txt                # CMake build configuration (UPDATED)
└── build_pico.bat                # Windows build script
```

## 🚀 Quick Start

### Hardware Connections

#### ST7305 Display Connection
```
Raspberry Pi Pico         ST7305 Display
+---------------+         +---------------+
|  GPIO18 (SCK) |-------->| SCK          |
|  GPIO19 (MOSI)|-------->| MOSI         |
|  GPIO17 (CS)  |-------->| CS           |
|  GPIO20 (DC)  |-------->| DC           |
|  GPIO15 (RST) |-------->| RST          |
|  3.3V         |-------->| VCC          |
|  GND          |-------->| GND          |
+---------------+         +---------------+
```

#### ST7306 Display Connection
```
Raspberry Pi Pico         ST7306 Display
+---------------+         +---------------+
|  GPIO18 (SCK) |-------->| SCK          |
|  GPIO19 (MOSI)|-------->| MOSI         |
|  GPIO17 (CS)  |-------->| CS           |
|  GPIO20 (DC)  |-------->| DC           |
|  GPIO15 (RST) |-------->| RST          |
|  3.3V         |-------->| VCC          |
|  GND          |-------->| GND          |
+---------------+         +---------------+
```

### Software Setup

1. **Clone the repository:**
```bash
git clone https://github.com/musicaJack/ST7305_2.9_Reflective_Lcd.git
cd ST7305_2.9_Reflective_Lcd
```

2. **Build the project:**
```bash
# Windows
./build_pico.bat

# Linux/Mac
mkdir build && cd build
cmake ..
make
```

## 📝 API Usage Guide

### Basic Setup (Template Approach)

```cpp
#include "st7305_driver.hpp"  // or "st7306_driver.hpp"
#include "pico_display_gfx.hpp"

// Hardware pin definitions
#define PIN_DC   20
#define PIN_RST  15
#define PIN_CS   17
#define PIN_SCLK 18
#define PIN_SDIN 19

// Initialize display driver
st7305::ST7305Driver display(PIN_DC, PIN_RST, PIN_CS, PIN_SCLK, PIN_SDIN);

// Create graphics context
pico_gfx::PicoDisplayGFX<st7305::ST7305Driver> gfx(
    display, 
    st7305::ST7305Driver::LCD_WIDTH, 
    st7305::ST7305Driver::LCD_HEIGHT
);

// Initialize
display.initialize();
```

### Graphics Operations

```cpp
// Clear and basic operations
display.clearDisplay();
gfx.fillScreen(WHITE);

// Drawing primitives
gfx.drawPixel(x, y, BLACK);
gfx.drawLine(10, 10, 100, 10, BLACK);
gfx.drawRectangle(20, 20, 50, 30, BLACK);
gfx.drawFilledRectangle(30, 30, 40, 20, BLACK);
gfx.drawCircle(60, 60, 15, BLACK);
gfx.drawFilledCircle(80, 80, 20, BLACK);
gfx.drawTriangle(10, 10, 50, 10, 30, 40, BLACK);

// Text rendering
display.drawString(10, 10, "Hello World!", BLACK);
display.drawChar(x, y, 'A', BLACK);

// Display rotation
gfx.setRotation(1);  // 0: 0°, 1: 90°, 2: 180°, 3: 270°
display.setRotation(1);

// Update display
display.display();
```

### Advanced Features

```cpp
// ST7306 Grayscale Support (4 levels)
st7306::ST7306Driver display306(/*pins*/);
display306.drawPixelGray(x, y, st7306::ST7306Driver::COLOR_GRAY1);
display306.drawPixelGray(x, y, st7306::ST7306Driver::COLOR_GRAY2);

// Font Layout Configuration
display.setFontLayout(st7306::FontLayout::Horizontal);
display.setFontLayout(st7306::FontLayout::Vertical);

// Power Management
display.lowPowerMode();
display.highPowerMode();
display.displaySleep(true);
display.displayOn(false);
```

### Advanced Graphics Example

```cpp
// Complex animation example
void windmillAnimation() {
    const int CENTER_X = gfx.width() / 2;
    const int CENTER_Y = gfx.height() / 2;
    const int BLADE_COUNT = 3;
    const int BLADE_LENGTH = 60;
    
    for (float angle = 0; angle < 2 * M_PI; angle += 0.1) {
        gfx.fillScreen(WHITE);
        
        // Draw windmill blades
        for (int i = 0; i < BLADE_COUNT; i++) {
            float blade_angle = angle + (i * 2 * M_PI / BLADE_COUNT);
            int end_x = CENTER_X + BLADE_LENGTH * cos(blade_angle);
            int end_y = CENTER_Y + BLADE_LENGTH * sin(blade_angle);
            
            gfx.drawLine(CENTER_X, CENTER_Y, end_x, end_y, BLACK);
        }
        
        // Draw center hub
        gfx.drawFilledCircle(CENTER_X, CENTER_Y, 5, BLACK);
        
        display.display();
        sleep_ms(50);
    }
}
```

## 🆕 What's New in Version 2.0

### Major Enhancements

1. **ST7306 Support**: Added complete support for ST7306 controllers with 4-level grayscale
2. **UI Abstraction Layer**: New hardware-agnostic graphics interface inspired by Adafruit GFX
3. **Template Architecture**: Modern C++ template design for better performance and type safety
4. **Enhanced Font System**: Improved font rendering with layout options and better performance
5. **Advanced Graphics**: Complex shape drawing, polygon support, and animation capabilities
6. **Dual-Target Build**: Support for building both ST7305 and ST7306 examples simultaneously

### API Improvements

- **Unified Interface**: Consistent API across different display controllers
- **Better Naming**: More intuitive function names and consistent parameter ordering
- **Enhanced Documentation**: Comprehensive inline documentation and examples
- **Error Handling**: Improved error checking and validation

### Performance Optimizations

- **Optimized SPI**: Faster communication with reduced overhead
- **Memory Efficiency**: Better buffer management and reduced memory footprint
- **Rendering Speed**: Optimized drawing algorithms for faster updates

## 🔧 Build Targets

The project now supports multiple build targets:

- `ST7305_Display`: ST7305 demonstration application
- `ST7306_Display`: ST7306 demonstration application

Each target includes comprehensive examples showcasing the respective controller's capabilities.

## 🐛 Troubleshooting

### Common Issues

1. **Display not working:**
   - Verify power supply is stable at 3.3V
   - Check all pin connections
   - Ensure SPI configuration matches your hardware

2. **Poor display quality:**
   - Check initialization sequence
   - Verify display area configuration
   - Confirm pixel write logic

3. **Performance issues:**
   - Use low-power mode when appropriate
   - Optimize SPI clock speed
   - Enable sleep mode when display is not in use

## 🤝 Contributing

We welcome contributions! Please ensure your submissions:

1. Follow the existing code style and conventions
2. Include comprehensive documentation and comments
3. Add appropriate test coverage
4. Update relevant documentation

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## 📧 Contact

For questions, suggestions, or support:

- **Email**: yinyue@beingdigital.cn
- **Issues**: [GitHub Issues](https://github.com/musicaJack/ST7305_2.9_Reflective_Lcd/issues)

## 🙏 Acknowledgments

Special thanks to all contributors who have helped improve this library and make it more accessible to the maker community. 