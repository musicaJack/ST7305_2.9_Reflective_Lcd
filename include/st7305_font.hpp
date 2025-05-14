#ifndef ST7305_FONT_HPP
#define ST7305_FONT_HPP

#include <cstdint>
#include <array>
#include <string_view>

namespace st7305 {

// 字体结构体
struct Font {
    const uint8_t* data;        // 字体数据
    uint8_t width;              // 字体宽度
    uint8_t height;             // 字体高度
    uint8_t first_char;         // 第一个字符的ASCII码
    uint8_t last_char;          // 最后一个字符的ASCII码
    uint8_t bytes_per_char;     // 每个字符占用的字节数
};

// 预定义字体
extern const Font font_6x8;     // 6x8像素字体
extern const Font font_8x16;    // 8x16像素字体
extern const Font font_12x24;   // 12x24像素字体

// 字体渲染类
class FontRenderer {
public:
    FontRenderer(const Font& font) : current_font_(font) {}
    
    // 获取字符宽度
    uint8_t getCharWidth() const { return current_font_.width; }
    
    // 获取字符高度
    uint8_t getCharHeight() const { return current_font_.height; }
    
    // 获取字符串宽度
    uint16_t getStringWidth(std::string_view str) const;
    
    // 获取字符数据
    const uint8_t* getCharData(char c) const;
    
    // 检查字符是否在字体范围内
    bool isCharInFont(char c) const {
        return c >= current_font_.first_char && c <= current_font_.last_char;
    }

private:
    const Font& current_font_;
};

} // namespace st7305

#endif // ST7305_FONT_HPP 