#ifndef PICO_DISPLAY_GFX_HPP
#define PICO_DISPLAY_GFX_HPP

#include "st73xx_ui.hpp"       // 抽象 UI 基类
#include "st7305_driver.hpp"   // 底层驱动

namespace pico_gfx { // 使用新的命名空间以避免潜在冲突

class PicoDisplayGFX : public ST73XX_UI {
public:
    PicoDisplayGFX(st7305::ST7305Driver& driver, int16_t w, int16_t h);
    ~PicoDisplayGFX() override;

    // 实现 ST73XX_UI 的纯虚函数
    // ST73XX_UI::drawPixel 调用这些 writePoint
    // 这里的 x, y 已经是经过 ST73XX_UI 旋转逻辑处理后的坐标
    void writePoint(uint x, uint y, bool enabled) override;
    void writePoint(uint x, uint y, uint16_t color) override; // uint16_t color 用于兼容，对于单色屏会转换为 bool

private:
    st7305::ST7305Driver& driver_; // 底层驱动的引用
};

} // namespace pico_gfx
#endif // PICO_DISPLAY_GFX_HPP 