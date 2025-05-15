#include "st7305_driver.hpp"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico_display_gfx.hpp"
#include "st7305_font.hpp"
#include "gfx_colors.hpp"
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>

// SPI和硬件引脚定义
#define SPI_PORT spi0
#define PIN_DC   20
#define PIN_RST  15
#define PIN_CS   17
#define PIN_SCLK 18
#define PIN_SDIN 19

// 风车动画参数配置
namespace windmill_config {
    // 时间配置（单位：秒）
    constexpr int TOTAL_DURATION = 30;    // 总演示时长
    constexpr int ACCEL_TIME = 8;         // 加速阶段时长
    constexpr int CONST_TIME = 14;        // 匀速阶段时长
    constexpr int DECEL_TIME = 8;         // 减速阶段时长
    
    // 帧率配置
    constexpr int TOTAL_FRAMES = 1000;    // 总帧数
    constexpr int ACCEL_FRAMES = TOTAL_FRAMES * ACCEL_TIME / TOTAL_DURATION;    // 加速阶段帧数
    constexpr int CONST_FRAMES = TOTAL_FRAMES * CONST_TIME / TOTAL_DURATION;    // 匀速阶段帧数
    constexpr int DECEL_FRAMES = TOTAL_FRAMES * DECEL_TIME / TOTAL_DURATION;    // 减速阶段帧数
    
    // 转速配置
    constexpr float MAX_RPM = 5000.0f;     // 最高转速（转/分钟）
    constexpr float MIN_RPM = 100.0f;      // 最低转速（转/分钟）
    
    // 速度配置（单位：毫秒）
    constexpr int MAX_DELAY = static_cast<int>(60000.0f / (MIN_RPM * NUM_BLADES));    // 最慢速度（最大延时）
    constexpr int MIN_DELAY = static_cast<int>(60000.0f / (MAX_RPM * NUM_BLADES));    // 最快速度（最小延时）
    
    // 风车外观配置
    constexpr int BLADE_LENGTH = 60;      // 叶片长度
    constexpr int HUB_RADIUS = 10;        // 中心圆半径
    constexpr int NUM_BLADES = 4;         // 叶片数量
    constexpr int BLADE_WIDTH = 6;        // 叶片宽度
    constexpr int TOTAL_ROTATIONS = 3;    // 总旋转圈数
}

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
    "as Pico charts its stellar chase."
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

    // // 演示1：显示诗歌
    // printf("Displaying poem...\n");
    // RF_lcd.clearDisplay();
    
    // // 计算每行最多能显示多少字符
    // int max_chars_per_line = (gfx.width() - 10) / font::FONT_WIDTH; // 减去左右各5像素的边距
    // std::vector<std::string> wrapped_lines;
    // for (int i = 0; i < NUM_LINES; i++) {
    //     std::vector<std::string> temp = wrapText(lines[i], max_chars_per_line);
    //     wrapped_lines.insert(wrapped_lines.end(), temp.begin(), temp.end());
    // }
    // int total_lines = wrapped_lines.size();
    // int line_height = font::FONT_HEIGHT + 2;
    
    // // 计算总显示高度
    // int total_display_height = total_lines * line_height;
    // // 计算起始Y坐标，确保有足够的空间显示所有文字
    // int start_y = 5; // 从顶部5像素开始
    
    // // 左对齐显示每一行
    // for (int i = 0; i < total_lines; i++) {
    //     int x = 5; // 左侧固定5像素边距
    //     int y = start_y + (i * line_height);
    //     // 确保不会超出显示区域
    //     if (y + font::FONT_HEIGHT <= gfx.height() - 5) { // 底部保留5像素边距
    //         RF_lcd.drawString(x, y, wrapped_lines[i].c_str(), BLACK);
    //     }
    // }
    // RF_lcd.display();
    // sleep_ms(5000); // 显示5秒

    // // 演示2：显示棋盘
    // printf("Displaying checkerboard pattern...\n");
    // RF_lcd.clearDisplay();
    // for (uint16_t y = 0; y < st7305::ST7305Driver::LCD_HEIGHT; y++) {
    //     for (uint16_t x = 0; x < st7305::ST7305Driver::LCD_WIDTH; x++) {
    //         bool color = ((x / 8) + (y / 8)) % 2 == 0;
    //         RF_lcd.drawPixel(x, y, color);
    //     }
    // }
    // RF_lcd.display();
    // sleep_ms(2000); // 显示2秒

    // 演示3：动态风车
    printf("Displaying windmill animation...\n");
    RF_lcd.clearDisplay();
    
    // 风车参数
    const int center_x = gfx.width() / 2;
    const int center_y = gfx.height() / 2;
    
    // 动画循环
    for (int frame = 0; frame < windmill_config::TOTAL_FRAMES; frame++) {
        RF_lcd.clearDisplay();
        
        // 计算当前帧的延时
        int current_delay;
        if (frame < windmill_config::ACCEL_FRAMES) {  // 加速阶段
            current_delay = windmill_config::MAX_DELAY - 
                (frame * (windmill_config::MAX_DELAY - windmill_config::MIN_DELAY) / windmill_config::ACCEL_FRAMES);
        } else if (frame >= windmill_config::ACCEL_FRAMES + windmill_config::CONST_FRAMES) {  // 减速阶段
            current_delay = windmill_config::MIN_DELAY + 
                ((frame - (windmill_config::ACCEL_FRAMES + windmill_config::CONST_FRAMES)) * 
                (windmill_config::MAX_DELAY - windmill_config::MIN_DELAY) / windmill_config::DECEL_FRAMES);
        } else {  // 匀速阶段
            current_delay = windmill_config::MIN_DELAY;
        }
        
        // 计算当前转速（RPM）
        float rpm = 60000.0f / (current_delay * windmill_config::NUM_BLADES);
        
        // 显示转速信息
        char rpm_text[32];
        char frame_text[32];
        snprintf(rpm_text, sizeof(rpm_text), "RPM: %.1f/%.1f", rpm, windmill_config::MAX_RPM);
        snprintf(frame_text, sizeof(frame_text), "Frame: %d/%d", frame + 1, windmill_config::TOTAL_FRAMES);
        RF_lcd.drawString(5, 5, rpm_text, BLACK);
        RF_lcd.drawString(5, 5 + font::FONT_HEIGHT + 2, frame_text, BLACK);
        
        // 计算旋转角度
        float rotation_angle = frame * (360.0f * windmill_config::TOTAL_ROTATIONS / windmill_config::TOTAL_FRAMES);
        
        // 绘制风车中心圆
        gfx.drawFilledCircle(center_x, center_y, windmill_config::HUB_RADIUS, BLACK);
        
        // 绘制风车叶片
        for (int i = 0; i < windmill_config::NUM_BLADES; i++) {
            float angle = (rotation_angle + i * (360.0f / windmill_config::NUM_BLADES)) * M_PI / 180.0f;
            int end_x = center_x + windmill_config::BLADE_LENGTH * cos(angle);
            int end_y = center_y + windmill_config::BLADE_LENGTH * sin(angle);
            
            // 绘制加粗的叶片
            for (int w = -windmill_config::BLADE_WIDTH/2; w <= windmill_config::BLADE_WIDTH/2; w++) {
                float perp_angle = angle + M_PI/2;  // 垂直角度
                int offset_x = w * cos(perp_angle);
                int offset_y = w * sin(perp_angle);
                gfx.drawLine(
                    center_x + offset_x, 
                    center_y + offset_y, 
                    end_x + offset_x, 
                    end_y + offset_y, 
                    BLACK
                );
            }
            
            // 绘制叶片尖端
            gfx.drawFilledCircle(end_x, end_y, windmill_config::BLADE_WIDTH/2, BLACK);
        }
        
        RF_lcd.display();
        sleep_ms(current_delay);
    }
    
    sleep_ms(1000);  // 暂停1秒

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