#pragma once

#include <cstdint>
#include <cstring>
#include <string_view>
#include "pico/stdlib.h"

namespace st7305 {

// 字体结构体定义
struct Font {
    const uint8_t* data;
    uint8_t first_char;
    uint8_t last_char;
    uint8_t width;
    uint8_t height;
    uint8_t bytes_per_char;
};

// 预定义字体
extern const Font font_6x8;
extern const Font font_8x16;

class ST7305Driver {
public:
    // 颜色定义
    static constexpr uint8_t COLOR_WHITE = 0x00;
    static constexpr uint8_t COLOR_BLACK = 0x01;

    // 显示参数
    static constexpr uint16_t LCD_WIDTH = 168;
    static constexpr uint16_t LCD_HEIGHT = 384;
    static constexpr uint16_t LCD_DATA_WIDTH = 42;  // LCD_WIDTH / 4
    static constexpr uint16_t LCD_DATA_HEIGHT = 192; // LCD_HEIGHT / 2
    static constexpr uint32_t DISPLAY_BUFFER_LENGTH = LCD_DATA_WIDTH * LCD_DATA_HEIGHT;

    // 构造函数
    ST7305Driver(uint dc_pin, uint res_pin, uint cs_pin, uint sclk_pin, uint sdin_pin);
    ~ST7305Driver();

    // 初始化函数
    void initialize();
    void clear();
    void display();

    // 绘图函数
    void drawPixel(uint16_t x, uint16_t y, bool color);
    void fill(uint8_t data);

    // 文本显示函数
    void setFont(const Font& font) { current_font_ = font; }
    void drawChar(uint16_t x, uint16_t y, char c, bool color);
    void drawString(uint16_t x, uint16_t y, std::string_view str, bool color);
    uint16_t getStringWidth(std::string_view str) const;

    // 显示控制
    void displayOn(bool enabled);
    void displaySleep(bool enabled);
    void displayInversion(bool enabled);
    void lowPowerMode();
    void highPowerMode();

private:
    void writeCommand(uint8_t cmd);
    void writeData(uint8_t data);
    void writeData(const uint8_t* data, size_t len);
    void writePoint(uint16_t x, uint16_t y, bool enabled);

    const uint dc_pin_;
    const uint res_pin_;
    const uint cs_pin_;
    const uint sclk_pin_;
    const uint sdin_pin_;
    uint8_t* display_buffer_;

    bool hpm_mode_ = false;
    bool lpm_mode_ = false;

    Font current_font_ = font_6x8;  // 默认使用6x8字体

    // 私有辅助函数
    void setAddress();
    void initST7305();
};

} // namespace st7305 