#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

typedef u8g2_uint_t u8g_uint_t;

void setup(void) {
    u8g2.begin();
}

void loop(void) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_HelvetiPixel_tr);
    u8g2.drawStr(0,16,"Patarav Menatrebi!");
    u8g2.drawStr(10,48,"Da Dzln Miyv!");
    u8g2.sendBuffer();
}