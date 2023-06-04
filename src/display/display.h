//
// Created by Temur on 03/06/2023.
//
#ifndef CHORDWAVE_DISPLAY_H
#define CHORDWAVE_DISPLAY_H
#include <U8g2lib.h>

// U8G2_SSD1306_128X64_NONAME_F_HW_I2C
class display : public U8G2 {
public:
    display(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE) : U8G2() {
        u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, rotation, u8x8_byte_arduino_sw_i2c, u8x8_gpio_and_delay_arduino);
        u8x8_SetPin_SW_I2C(getU8x8(), clock,  data,  reset);

    }

    void init(){
        this->begin();

        this->setBitmapMode(1);
        this->setDrawColor(2);
    }
};


#endif //CHORDWAVE_DISPLAY_H
