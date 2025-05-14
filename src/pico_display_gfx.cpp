#include "pico_display_gfx.hpp"

namespace pico_gfx {

PicoDisplayGFX::PicoDisplayGFX(st7305::ST7305Driver& driver, int16_t w, int16_t h)
    : ST73XX_UI(w, h), driver_(driver) {}

PicoDisplayGFX::~PicoDisplayGFX() {}

void PicoDisplayGFX::writePoint(uint x, uint y, bool enabled) {
    // x, y 是由 ST73XX_UI::drawPixel 传递过来的，已经过 ST73XX_UI 内部的旋转处理。
    // 直接调用驱动的原始画点函数，在物理坐标 (x,y) 上画点。
    driver_.plotPixelRaw(x, y, enabled);
}

void PicoDisplayGFX::writePoint(uint x, uint y, uint16_t color) {
    // 对于单色屏幕，将 uint16_t 类型的颜色转换为 bool 类型。
    // 通常约定：0 为关闭/背景色，非0 为点亮/前景色。
    driver_.plotPixelRaw(x, y, (color != 0));
}

} // namespace pico_gfx 