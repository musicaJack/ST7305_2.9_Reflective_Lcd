# ST7305 Pico Display Driver

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi%20Pico-red.svg)](https://www.raspberrypi.com/products/raspberry-pi-pico/)
[![Version](https://img.shields.io/badge/Version-1.0.0-green.svg)](https://github.com/musicaJack/ST7305_2.9_Reflective_Lcd/releases)

English | [中文](README.zh.md)

A high-performance display driver library for ST7305 reflective LCD displays on Raspberry Pi Pico. This library provides comprehensive display control features including graphics rendering, text display, and basic display operations.

## Features

- Full ST7305 display driver support
- Hardware abstraction layer for easy porting
- Built-in graphics drawing functions
- Optimized SPI communication
- User-friendly API interface
- Support for custom fonts

## Project Architecture

The project uses a modular design with the following components:

### Core Modules

- `st7305_driver.hpp/cpp`: Core driver implementation, including display initialization and basic control functions
- `st7305_font.cpp`: Font support implementation
- `st7305_config.hpp`: Configuration file, containing pin definitions and display parameters

### Directory Structure

```
├── src/                    # Source code directory
│   ├── st7305_driver.cpp  # Core driver implementation
│   └── st7305_font.cpp    # Font support
├── include/               # Header files directory
│   ├── st7305_driver.hpp # Core driver header
│   └── st7305_config.hpp # Configuration file
├── examples/              # Example code
├── build/                 # Build output directory
└── CMakeLists.txt        # CMake build configuration
```

## Quick Start

### Hardware Connection

1. Connect the ST7305 display to Raspberry Pi Pico:
   - SCK: GPIO18
   - MOSI: GPIO19
   - CS: GPIO17
   - DC: GPIO20
   - RST: GPIO15
   - VCC: 3.3V
   - GND: GND

Connection diagram:
```
Raspberry Pi Pico         ST7305 Display
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

Notes:
1. Ensure all connections are secure to avoid loose connections
2. Check power connections carefully before powering on
3. If display doesn't work, check:
   - Power voltage is stable at 3.3V
   - All signal connections are correct
   - SPI communication timing is correct

### Software Setup

1. Clone the repository:
```bash
git clone https://github.com/musicaJack/ST7305_2.9_Reflective_Lcd.git
cd ST7305_2.9_Reflective_Lcd
```

2. Build the project:
```bash
./build_pico.bat
```

## API Usage Guide

### Basic Configuration

```cpp
#include "st7305_driver.hpp"

// Create display object
st7305::ST7305Driver display(DC_PIN, RST_PIN, CS_PIN, SCLK_PIN, SDIN_PIN);

// Initialize display
display.initialize();
```

### Basic Display Operations

```cpp
// Clear screen
display.clear();

// Fill screen with pattern
display.fill(0xFF);

// Display on/off
display.displayOn(true);
display.displayOn(false);

// Sleep mode
display.displaySleep(true);
display.displaySleep(false);

// Display inversion
display.displayInversion(true);
display.displayInversion(false);
```

### Graphics Drawing

```cpp
// Clear display
display.clearDisplay();
display.display();

// Draw pixel
display.drawPixel(x, y, 1);

// Draw line, rectangle, circle, triangle, etc.
display.drawLine(10, 10, 100, 10, 1);
display.drawRectangle(20, 20, 50, 30, 1);
display.drawCircle(60, 60, 15, 1);
display.drawFilledRectangle(30, 30, 40, 20, 0);

// Draw text (driver layer)
display.drawString(10, 10, "Hello LCD!", 1);

// Refresh display
display.display();
```

### Power Management

```cpp
// Low power mode
display.lowPowerMode();

// High power mode
display.highPowerMode();
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. Before submitting code, please ensure:

1. Code follows project coding standards
2. Necessary comments and documentation are added
3. All tests pass

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For questions or suggestions, please contact:

- Email: yinyue@beingdigital.cn

## Acknowledgments

Thanks to all contributors to this project. 