#include "st7305_font.hpp"
#include "fonts/font_6x8.hpp"
#include "fonts/font_8x16.hpp"

namespace st7305 {

// 定义字体结构
const Font font_6x8 = {
    .data = font_6x8_data,
    .width = 6,
    .height = 8,
    .first_char = 32,
    .last_char = 127,
    .bytes_per_char = 6  // (6 * 8) / 8 = 6 bytes per character
};

const Font font_8x16 = {
    .data = font_8x16_data,
    .width = 8,
    .height = 16,
    .first_char = 32,
    .last_char = 127,
    .bytes_per_char = 16  // (8 * 16) / 8 = 16 bytes per character
};

// 实现FontRenderer类的方法
uint16_t FontRenderer::getStringWidth(std::string_view str) const {
    if (str.empty()) return 0;
    
    uint16_t width = 0;
    for (char c : str) {
        if (isCharInFont(c)) {
            width += current_font_.width;
        }
    }
    return width;
}

const uint8_t* FontRenderer::getCharData(char c) const {
    // 确保字符在有效范围内
    if (!isCharInFont(c)) c = ' ';
    
    // 计算字符在字体数据中的偏移量
    size_t char_size = current_font_.bytes_per_char;
    return &current_font_.data[(c - current_font_.first_char) * char_size];
}

} // namespace st7305 