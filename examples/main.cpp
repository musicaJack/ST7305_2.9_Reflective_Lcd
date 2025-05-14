#include "st7305_driver.hpp"
#include "pico/stdlib.h"
#include <cstdio>

// 定义引脚
const uint DC_PIN = 20;
const uint RES_PIN = 15;
const uint CS_PIN = 17;
const uint SCK_PIN = 18;
const uint SDIN_PIN = 19;

int main() {
    // 初始化标准库
    stdio_init_all();

    // 创建显示驱动实例
    st7305::ST7305Driver display(DC_PIN, RES_PIN, CS_PIN, SCK_PIN, SDIN_PIN);

    // 初始化显示
    display.initialize();
    printf("ST7305 Display Test\n");

    // 绘制棋盘图案
    for (uint16_t y = 0; y < st7305::ST7305Driver::LCD_HEIGHT; y++) {
        for (uint16_t x = 0; x < st7305::ST7305Driver::LCD_WIDTH; x++) {
            bool color = ((x / 8) + (y / 8)) % 2 == 0;
            display.drawPixel(x, y, color);
        }
    }

    // 更新显示
    display.display();
    printf("Checkerboard pattern displayed\n");

    // 主循环
    while (true) {
        sleep_ms(1000);
    }

    return 0;
} 