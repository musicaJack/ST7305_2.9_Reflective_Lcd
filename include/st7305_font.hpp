#pragma once

#include <cstdint>

namespace font {

// Font size constants
constexpr int FONT_WIDTH = 8;
constexpr int FONT_HEIGHT = 16;
constexpr int FONT_SIZE = 4096;  // 16 * 256 characters

// Font data declaration
extern const uint8_t ST7305_FONT[FONT_SIZE];

// Helper function to get character data
inline const uint8_t* get_char_data(char c) {
    return &ST7305_FONT[static_cast<unsigned char>(c) * FONT_HEIGHT];
}

} // namespace font
