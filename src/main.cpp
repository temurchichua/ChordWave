#include <Arduino.h>

void setup(void) {
    u8g2.begin();
}

void loop(void) {
    u8g2.clearBuffer();
    u8g2.drawBitmap(0, 0, 128/8, 64, epd_bitmap_base_gui);
    u8g2.sendBuffer();
}