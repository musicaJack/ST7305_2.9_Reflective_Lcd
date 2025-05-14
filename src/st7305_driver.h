#pragma once

#include <Arduino.h>
#include <SPI.h>

class ST7305Driver {
public:
    // 构造函数
    ST7305Driver(int dcPin, int resPin, int csPin, int sclkPin, int sdinPin);

    // 初始化
    void initialize();

    // 显示控制
    void displayOn(bool on);
    void sleep(bool sleep);
    void invertDisplay(bool invert);

    // 显示缓冲区操作
    void clear();
    void fill(uint8_t data);
    void writePoint(uint16_t x, uint16_t y, bool enabled);
    void display();

private:
    // 引脚定义
    const int DC_PIN;
    const int RES_PIN;
    const int CS_PIN;
    const int SCLK_PIN;
    const int SDIN_PIN;

    // 显示参数
    static constexpr uint16_t LCD_WIDTH = 168;
    static constexpr uint16_t LCD_HIGH = 384;
    static constexpr uint16_t LCD_DATA_WIDTH = 42;  // 168/4=42
    static constexpr uint16_t LCD_DATA_HIGH = 192;  // 384/2=192
    static constexpr uint16_t DISPLAY_BUFFER_LENGTH = 8064;  // 192*42=8064

    // 显示缓冲区
    uint8_t display_buffer_[DISPLAY_BUFFER_LENGTH];

    // 通信函数
    void writeCommand(uint8_t cmd);
    void writeData(uint8_t data);
    void writeData(const uint8_t* data, size_t len);
}; 