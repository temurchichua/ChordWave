#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#include "keyboard/key_bitmaps.h"

#endif

typedef struct {
    char* note;
    int x;
    int y;
    int width;
    int height;

    const unsigned char* key_bitmap;
    const unsigned char* key_pressed;
} key_struct;

const key_struct keys[12] = {
        {"C", 9, 20, 7, 40, left, left_pressed},
        {"C#", 13, 20, 6, 25, black, black_pressed},
        {"D", 17, 20, 7, 40, middle, middle_pressed},
        {"D#", 21, 20, 6, 25, black, black_pressed},
        {"E", 25, 20, 7, 40, right, right_pressed},
        {"F", 33, 20, 7, 40, left, left_pressed},
        {"F#", 37, 20, 6, 25, black, black_pressed},
        {"G", 41, 20, 7, 40, middle, middle_pressed},
        {"G#", 45, 20, 6, 25, black, black_pressed},
        {"A", 49, 20, 7, 40, middle, middle_pressed},
        {"A#", 53, 20, 6, 25, black, black_pressed},
        {"B", 57, 20, 7, 40, right, right_pressed},
};

int pressed[12]= {false, false, false, true, false, true, false, true};

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display


void setup(void) {
    pressed[11] = true; // fix this line

    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setBitmapMode(1);
    u8g2.setDrawColor(2);

    u8g2.drawHLine(0, 16, 128);

    for (uint8_t i = 0; i < 12; ++i) {
        const key_struct key = keys[i];
        const unsigned char* bmp = pressed[i] ? key.key_pressed : key.key_bitmap;
        u8g2.drawXBM(key.x, key.y, key.width, key.height, bmp);

    }
    u8g2.sendBuffer();
}

void loop(void) {

}