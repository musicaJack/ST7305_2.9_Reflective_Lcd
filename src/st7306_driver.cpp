#include "st7306_driver.hpp"
#include <cstring>
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "st73xx_font.hpp"
#include "gfx_colors.hpp"

namespace st7306 {

ST7306Driver::ST7306Driver(uint dc_pin, uint res_pin, uint cs_pin, uint sclk_pin, uint sdin_pin) :
    dc_pin_(dc_pin),
    res_pin_(res_pin),
    cs_pin_(cs_pin),
    sclk_pin_(sclk_pin),
    sdin_pin_(sdin_pin),
    display_buffer_(new uint8_t[DISPLAY_BUFFER_LENGTH]),
    font_layout_(FontLayout::Vertical)
{
    // 初始化GPIO
    gpio_init(dc_pin_);
    gpio_init(res_pin_);
    gpio_init(cs_pin_);
    gpio_init(sclk_pin_);
    gpio_init(sdin_pin_);

    gpio_set_dir(dc_pin_, GPIO_OUT);
    gpio_set_dir(res_pin_, GPIO_OUT);
    gpio_set_dir(cs_pin_, GPIO_OUT);
    gpio_set_dir(sclk_pin_, GPIO_OUT);
    gpio_set_dir(sdin_pin_, GPIO_OUT);

    // 初始化SPI
    spi_init(spi0, 40000000); // 40MHz
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(sclk_pin_, GPIO_FUNC_SPI);
    gpio_set_function(sdin_pin_, GPIO_FUNC_SPI);
}

ST7306Driver::~ST7306Driver() {
    delete[] display_buffer_;
}

void ST7306Driver::initialize() {
    // 初始化引脚
    gpio_set_dir(dc_pin_, GPIO_OUT);
    gpio_set_dir(res_pin_, GPIO_OUT);
    gpio_set_dir(cs_pin_, GPIO_OUT);
    gpio_set_dir(sclk_pin_, GPIO_OUT);
    gpio_set_dir(sdin_pin_, GPIO_OUT);

    // 复位时序
    gpio_put(res_pin_, 1);
    sleep_ms(10);
    gpio_put(res_pin_, 0);
    sleep_ms(10);
    gpio_put(res_pin_, 1);
    sleep_ms(10);

    // 初始化SPI
    spi_init(spi0, 40000000); // 40MHz
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(sclk_pin_, GPIO_FUNC_SPI);
    gpio_set_function(sdin_pin_, GPIO_FUNC_SPI);

    initST7306();
}

void ST7306Driver::initST7306() {
    writeCommand(0xD6); // NVM Load Control
    writeData(0x17);
    writeData(0x02);

    writeCommand(0xD1); // Booster Enable
    writeData(0x01);

    writeCommand(0xC0); // Gate Voltage Setting
    writeData(0x12); // VGH 17V
    writeData(0x0A); // VGL -10V

    writeCommand(0xC1); // VSHP Setting
    writeData(115);    // VSHP1
    writeData(0x3E);   // VSHP2
    writeData(0x3C);   // VSHP3
    writeData(0x3C);   // VSHP4

    writeCommand(0xC2); // VSLP Setting
    writeData(0);      // VSLP1
    writeData(0x21);   // VSLP2
    writeData(0x23);   // VSLP3
    writeData(0x23);   // VSLP4

    writeCommand(0xC4); // VSHN Setting
    writeData(50);     // VSHN1
    writeData(0x5C);   // VSHN2
    writeData(0x5A);   // VSHN3
    writeData(0x5A);   // VSHN4

    writeCommand(0xC5); // VSLN Setting
    writeData(50);     // VSLN1
    writeData(0x35);   // VSLN2
    writeData(0x37);   // VSLN3
    writeData(0x37);   // VSLN4

    writeCommand(0xD8); // OSC Setting
    writeData(0xA6);   // Enable OSC
    writeData(0xE9);

    writeCommand(0xB2); // Frame Rate Control
    writeData(0x12);   // HPM=32hz ; LPM=1hz

    writeCommand(0xB3); // Update Period Gate EQ Control in HPM
    writeData(0xE5);
    writeData(0xF6);
    writeData(0x17);
    writeData(0x77);
    writeData(0x77);
    writeData(0x77);
    writeData(0x77);
    writeData(0x77);
    writeData(0x77);
    writeData(0x71);

    writeCommand(0xB4); // Update Period Gate EQ Control in LPM
    writeData(0x05);
    writeData(0x46);
    writeData(0x77);
    writeData(0x77);
    writeData(0x77);
    writeData(0x77);
    writeData(0x76);
    writeData(0x45);

    writeCommand(0x62); // Gate Timing Control
    writeData(0x32);
    writeData(0x03);
    writeData(0x1F);

    writeCommand(0xB7); // Source EQ Enable
    writeData(0x13);

    writeCommand(0xB0); // Gate Line Setting
    writeData(0x64);   // 400 line = 100*4

    writeCommand(0x11); // Sleep out
    sleep_ms(120);

    writeCommand(0xC9); // Source Voltage Select
    writeData(0x00);   // VSHP1; VSLP1 ; VSHN1 ; VSLN1

    writeCommand(0x36); // Memory Data Access Control
    writeData(0x48);   // MX=1 ; DO=1

    writeCommand(0x3A); // Data Format Select
    writeData(0x11);   // 3write for 24bit

    writeCommand(0xB9); // Gamma Mode Setting
    writeData(0x20);   // Mono

    writeCommand(0xB8); // Panel Setting
    writeData(0x29);   // 1-Dot inversion, Frame inversion, One Line Interlace

    writeCommand(0x2A); // Column Address Setting
    writeData(0x05);
    writeData(0x36);

    writeCommand(0x2B); // Row Address Setting
    writeData(0x00);
    writeData(0xC7);

    writeCommand(0x35); // TE
    writeData(0x00);

    writeCommand(0xD0); // Auto power down
    writeData(0xFF);   // Auto power down ON

    writeCommand(0x38); // HPM:high Power Mode ON

    writeCommand(0x29); // Display ON

    writeCommand(0x20); // Display Inversion Off

    writeCommand(0xBB); // Enable Clear RAM
    writeData(0x4F);   // CLR=0 ; Enable Clear RAM,clear RAM to 0

    hpm_mode_ = true;
    lpm_mode_ = false;
}

void ST7306Driver::writeCommand(uint8_t cmd) {
    gpio_put(dc_pin_, 0);
    gpio_put(cs_pin_, 0);
    spi_write_blocking(spi0, &cmd, 1);
    gpio_put(cs_pin_, 1);
}

void ST7306Driver::writeData(uint8_t data) {
    gpio_put(dc_pin_, 1);
    gpio_put(cs_pin_, 0);
    spi_write_blocking(spi0, &data, 1);
    gpio_put(cs_pin_, 1);
}

void ST7306Driver::writeData(const uint8_t* data, size_t len) {
    gpio_put(dc_pin_, 1);
    gpio_put(cs_pin_, 0);
    spi_write_blocking(spi0, data, len);
    gpio_put(cs_pin_, 1);
}

void ST7306Driver::clear() {
    memset(display_buffer_, 0x00, DISPLAY_BUFFER_LENGTH);
}

void ST7306Driver::fill(uint8_t data) {
    memset(display_buffer_, data, DISPLAY_BUFFER_LENGTH);
}

void ST7306Driver::writePoint(uint16_t x, uint16_t y, bool enabled) {
    if(x >= LCD_WIDTH || y >= LCD_HEIGHT) return;
    
    uint real_x = x/2; // 0->0, 3->0, 4->1, 7->1
    uint real_y = y/2; // 0->0, 1->0, 2->1, 3->1
    uint write_byte_index = real_y*LCD_DATA_WIDTH+real_x;
    uint one_two = (y % 2 == 0)?0:1;
    uint line_bit_1 = (x % 2)*4;
    uint line_bit_0 = (x % 2)*4 + 2;
    uint8_t write_bit_1 = 7-(line_bit_1+one_two);
    uint8_t write_bit_0 = 7-(line_bit_0+one_two);

    if (enabled) {
        display_buffer_[write_byte_index] |= (1 << write_bit_1);
        display_buffer_[write_byte_index] |= (1 << write_bit_0);
    } else {
        display_buffer_[write_byte_index] &= ~(1 << write_bit_1);
        display_buffer_[write_byte_index] &= ~(1 << write_bit_0);
    }
}

void ST7306Driver::display() {
    setAddress();
    writeData(display_buffer_, DISPLAY_BUFFER_LENGTH);
}

void ST7306Driver::setAddress() {
    writeCommand(0x2A); // Column Address Setting
    writeData(0x05);
    writeData(0x36);

    writeCommand(0x2B); // Row Address Setting
    writeData(0x00);
    writeData(0xC7);

    writeCommand(0x2C); // Write image data
}

void ST7306Driver::drawPixel(uint16_t x, uint16_t y, bool color) {
    uint16_t tx = x, ty = y;
    switch (rotation_) {
        case 1:
            tx = LCD_WIDTH - 1 - y;
            ty = x;
            break;
        case 2:
            tx = LCD_WIDTH - 1 - x;
            ty = LCD_HEIGHT - 1 - y;
            break;
        case 3:
            tx = y;
            ty = LCD_HEIGHT - 1 - x;
            break;
        default:
            break;
    }
    plotPixelRaw(tx, ty, color);
}

void ST7306Driver::plotPixelRaw(uint16_t x, uint16_t y, bool color) {
    writePoint(x, y, color);
}

void ST7306Driver::displayOn(bool enabled) {
    writeCommand(enabled ? 0x29 : 0x28);
}

void ST7306Driver::displaySleep(bool enabled) {
    if (enabled) {
        if (lpm_mode_) {
            writeCommand(0x38); // HPM:high Power Mode ON
            sleep_ms(300);
        }
        writeCommand(0x10); // Sleep IN
        sleep_ms(100);
    } else {
        writeCommand(0x11); // Sleep OUT
        sleep_ms(100);
    }
}

void ST7306Driver::displayInversion(bool enabled) {
    writeCommand(enabled ? 0x21 : 0x20);
}

void ST7306Driver::lowPowerMode() {
    if (!lpm_mode_) {
        writeCommand(0x39); // LPM:Low Power Mode ON
        hpm_mode_ = false;
        lpm_mode_ = true;
    }
}

void ST7306Driver::highPowerMode() {
    if (!hpm_mode_) {
        writeCommand(0x38); // HPM:high Power Mode ON
        hpm_mode_ = true;
        lpm_mode_ = false;
    }
}

void ST7306Driver::clearDisplay() {
    clear();
}

void ST7306Driver::setRotation(int r) {
    rotation_ = r & 0x03;
}

int ST7306Driver::getRotation() const {
    return rotation_;
}

void ST7306Driver::display_on(bool enabled) {
    displayOn(enabled);
}

void ST7306Driver::display_sleep(bool enabled) {
    displaySleep(enabled);
}

void ST7306Driver::display_Inversion(bool enabled) {
    displayInversion(enabled);
}

void ST7306Driver::Low_Power_Mode() {
    lowPowerMode();
}

void ST7306Driver::High_Power_Mode() {
    highPowerMode();
}

void ST7306Driver::setFontLayout(FontLayout layout) {
    font_layout_ = layout;
}

uint8_t ST7306Driver::getCurrentFontWidth() const {
    return font::FONT_WIDTH;
}

void ST7306Driver::drawString(uint16_t x, uint16_t y, std::string_view str, bool color) {
    drawString(x, y, str.data(), color);
}

} // namespace st7306 