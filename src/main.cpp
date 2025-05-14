#include <Arduino.h>
#include "st7305_driver.h"

// 引脚定义
const int DC_PIN = 8;
const int RES_PIN = 9;
const int CS_PIN = 10;
const int SCLK_PIN = 11;
const int SDIN_PIN = 12;

// 创建显示驱动实例
ST7305Driver display(DC_PIN, RES_PIN, CS_PIN, SCLK_PIN, SDIN_PIN);

void setup() {
    // 初始化串口
    Serial.begin(115200);
    Serial.println("ST7305 Display Test");

    // 初始化显示
    display.initialize();
    display.clear();
    display.display();

    // 显示测试图案
    for (int x = 0; x < 168; x++) {
        for (int y = 0; y < 384; y++) {
            if ((x + y) % 2 == 0) {
                display.writePoint(x, y, true);
            }
        }
    }
    display.display();
}

void loop() {
    // 反转显示
    display.invertDisplay(true);
    delay(1000);
    display.invertDisplay(false);
    delay(1000);
} 