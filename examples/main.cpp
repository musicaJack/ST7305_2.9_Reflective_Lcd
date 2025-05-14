#include "st7305_driver.hpp"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico_display_gfx.hpp"
#include "st7305_font.hpp"
#include "gfx_colors.hpp"
#include <cstdio>

// SPI和硬件引脚定义
#define SPI_PORT spi0 // SPI端口，ST7305Driver内部固定使用spi0，此处仅为注释说明
#define PIN_DC   20
#define PIN_RST  15
#define PIN_CS   17
#define PIN_SCLK 18 // SPI Clock
#define PIN_SDIN 19 // SPI Data In (MOSI)
// 注意：ST7305Driver 的构造函数目前不包含 BUSY 引脚参数

int main() {
    // 初始化标准库
    stdio_init_all();

    // 使用定义的引脚号实例化驱动
    st7305::ST7305Driver RF_lcd(PIN_DC, PIN_RST, PIN_CS, PIN_SCLK, PIN_SDIN);
    pico_gfx::PicoDisplayGFX gfx(RF_lcd, st7305::ST7305Driver::LCD_WIDTH, st7305::ST7305Driver::LCD_HEIGHT);

    printf("Initializing ST7305 display...\n");
    RF_lcd.initialize();
    printf("Display initialized.\n");

    // 设置旋转 (UI层和Driver层都需要设置，以确保图形和文本方向一致)
    // 0: 0 deg, 1: 90 deg, 2: 180 deg, 3: 270 deg
    const int rotation = 0;
    gfx.setRotation(rotation);      // 设置 GFX UI 的旋转
    RF_lcd.setRotation(rotation);   // 设置底层驱动的旋转 (主要影响driver自带的drawString等)

    printf("Clearing display...\n");
    RF_lcd.clearDisplay(); // 清屏仍用 driver
    RF_lcd.display();      // 刷新显示也用 driver
    sleep_ms(500);

    // 使用 GFX UI 进行绘图
    printf("Drawing using PicoDisplayGFX...\n");
    gfx.drawPixel(10, 10, BLACK);
    gfx.drawPixel(11, 10, BLACK);
    gfx.drawPixel(10, 11, BLACK);
    gfx.drawPixel(11, 11, BLACK);

    gfx.drawLine(20, 20, 100, 20, BLACK);
    gfx.drawLine(20, 20, 20, 100, BLACK);

    gfx.drawRectangle(30, 30, 50, 40, BLACK);
    gfx.drawFilledRectangle(35, 35, 40, 30, WHITE);

    gfx.drawCircle(100, 70, 20, BLACK);
    gfx.drawFilledCircle(150, 70, 20, BLACK);

    gfx.drawTriangle(50, 120, 30, 150, 70, 150, BLACK);
    gfx.drawFilledTriangle(100, 120, 80, 150, 120, 150, WHITE);


    // 使用 ST7305Driver 进行文本绘制 (因为 ST73XX_UI 当前没有文本功能)
    printf("Drawing text using ST7305Driver...\n");
    char test_str[] = "Hello GFX!";
    // 计算文本位置，注意坐标系是基于旋转后的
    // 对于 90 度旋转，原点在左上角，x 向下，y 向左
    // 我们需要根据 gfx.width() 和 gfx.height() (即 ST73XX_UI 的宽高) 来定位
    uint16_t text_x, text_y;

    // 简单的居中逻辑 (基于 GFX 尺寸)
    int text_width = sizeof(test_str) * RF_lcd.getCurrentFontWidth();

    switch (gfx.getRotation()) {
        case 0:
            text_x = (gfx.width() - text_width) / 2;
            text_y = gfx.height() / 2;
            break;
        case 1: // 90 deg
            text_x = gfx.width() / 2;
            text_y = (gfx.height() + text_width) / 2; // Y 轴反向
            break;
        case 2: // 180 deg
            text_x = (gfx.width() + text_width) / 2;
            text_y = gfx.height() / 2;
            break;
        case 3: // 270 deg
            text_x = gfx.width() / 2;
            text_y = (gfx.height() - text_width) / 2;
            break;
        default:
            text_x = 20; text_y = 180;
            break;
    }
    if (text_x < 0) text_x = 0;
    if (text_y < 0) text_y = 0;
    
    RF_lcd.drawString(text_x, text_y, test_str, BLACK);
    RF_lcd.drawString(5, 5, "(0,0)@GFX", BLACK); // 在gfx坐标系(0,0)附近写字

    printf("GFX width: %d, height: %d\n", gfx.width(), gfx.height());

    printf("Refreshing display...\n");
    RF_lcd.display();

    sleep_ms(10000);

    printf("Clearing display...\n");
    RF_lcd.clearDisplay(); // 清屏仍用 driver
    RF_lcd.display();      // 刷新显示也用 driver
    sleep_ms(200);
    
    // 绘制棋盘图案
    for (uint16_t y = 0; y < st7305::ST7305Driver::LCD_HEIGHT; y++) {
        for (uint16_t x = 0; x < st7305::ST7305Driver::LCD_WIDTH; x++) {
            bool color = ((x / 8) + (y / 8)) % 2 == 0;
            RF_lcd.drawPixel(x, y, color);
        }
    }
    RF_lcd.display();
    sleep_ms(10000);
    printf("Done.\n");

    return 0;
}