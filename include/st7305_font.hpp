#pragma once
#include <cstdint>

// 字体宽度和高度（像素）
#define ST7305_FONT_WIDTH 5
#define ST7305_FONT_HEIGHT 7

// 标准ASCII 5x7点阵字库（范围：space~DEL）
extern const unsigned char font[]; 