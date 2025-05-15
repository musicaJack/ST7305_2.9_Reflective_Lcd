#include "st7305_driver.hpp"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico_display_gfx.hpp"
#include "st7305_font.hpp"
#include "gfx_colors.hpp"
#include <cstdio>
#include <vector>
#include <string>

// SPI和硬件引脚定义
#define SPI_PORT spi0
#define PIN_DC   20
#define PIN_RST  15
#define PIN_CS   17
#define PIN_SCLK 18
#define PIN_SDIN 19

// 文字内容
const char* lines[] = {
    "Satellites whisper, Pixels dance.",
    "Pico brings them both to life.",
    "Tiny circuits hum a cosmic tune,",
    "while LEDs paint the void in bloom.",
    "A microcontroller's quiet might,",
    "turns stardust into blinking light.",
    "Through silicon veins, electrons race,",
    "crafting dreams in this small space.",
    "The universe fits in RAM's embrace,",
    "as Pico charts its stellar chase.",
    "From Earth to where the comets roam,",
    "it codes a path to bring them home."
};

const int NUM_LINES = sizeof(lines) / sizeof(lines[0]);

// 自动分割字符串为多行，保证每行不超过max_chars个字符
std::vector<std::string> wrapText(const char* text, int max_chars) {
    std::vector<std::string> lines;
    std::string str(text);
    size_t start = 0;
    while (start < str.length()) {
        size_t len = std::min((size_t)max_chars, str.length() - start);
        lines.push_back(str.substr(start, len));
        start += len;
    }
    return lines;
}

int main() {
    stdio_init_all();

    st7305::ST7305Driver RF_lcd(PIN_DC, PIN_RST, PIN_CS, PIN_SCLK, PIN_SDIN);
    pico_gfx::PicoDisplayGFX gfx(RF_lcd, st7305::ST7305Driver::LCD_WIDTH, st7305::ST7305Driver::LCD_HEIGHT);

    printf("Initializing ST7305 display...\n");
    RF_lcd.initialize();
    printf("Display initialized.\n");

    const int rotation = 0;
    gfx.setRotation(rotation);
    RF_lcd.setRotation(rotation);

    // 演示1：显示诗歌
    printf("Displaying poem...\n");
    RF_lcd.clearDisplay();
    
    // 计算每行最多能显示多少字符
    int max_chars_per_line = gfx.width() / font::FONT_WIDTH;
    std::vector<std::string> wrapped_lines;
    for (int i = 0; i < NUM_LINES; i++) {
        std::vector<std::string> temp = wrapText(lines[i], max_chars_per_line);
        wrapped_lines.insert(wrapped_lines.end(), temp.begin(), temp.end());
    }
    int total_lines = wrapped_lines.size();
    int line_height = font::FONT_HEIGHT + 2;
    int start_y = (gfx.height() - (total_lines * line_height)) / 2;
    
    // 居中显示每一行
    for (int i = 0; i < total_lines; i++) {
        int x = (gfx.width() - wrapped_lines[i].length() * font::FONT_WIDTH) / 2;
        int y = start_y + (i * line_height);
        RF_lcd.drawString(x, y, wrapped_lines[i].c_str(), BLACK);
    }
    RF_lcd.display();
    sleep_ms(20000); // 显示20秒

    // 演示2：显示棋盘
    printf("Displaying checkerboard pattern...\n");
    RF_lcd.clearDisplay();
    for (uint16_t y = 0; y < st7305::ST7305Driver::LCD_HEIGHT; y++) {
        for (uint16_t x = 0; x < st7305::ST7305Driver::LCD_WIDTH; x++) {
            bool color = ((x / 8) + (y / 8)) % 2 == 0;
            RF_lcd.drawPixel(x, y, color);
        }
    }
    RF_lcd.display();
    sleep_ms(10000); // 显示10秒

    // 结束演示
    printf("Demo end.\n");
    RF_lcd.clearDisplay();
    
    // 显示结束信息
    const char* end_text = "DEMO END.";
    int end_text_len = strlen(end_text);
    int end_x = (gfx.width() - end_text_len * font::FONT_WIDTH) / 2;
    int end_y = (gfx.height() - font::FONT_HEIGHT) / 2;
    RF_lcd.drawString(end_x, end_y, end_text, BLACK);
    RF_lcd.display();
    sleep_ms(2000);

    return 0;
}