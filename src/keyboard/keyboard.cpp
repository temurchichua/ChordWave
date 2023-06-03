//
// Created by Temur on 31/05/2023.
//

#include "keyboard.h"
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

bool pressed[12];

const unsigned long interval = 100;  // Animation interval in milliseconds
unsigned long previousMillis = 0;
uint8_t currentKey = 0;

void initial_setup() {
    u8g2.begin();

    u8g2.setBitmapMode(1);
    u8g2.setDrawColor(2);

    u8g2.clearBuffer();
}

void print_keyboard(){
    u8g2.clearBuffer();

    u8g2.drawHLine(0, 16, 128);

    for (uint8_t i = 0; i < 12; ++i) {
        const key_struct key = keys[i];
        const unsigned char* bmp = pressed[i] ? key.key_pressed : key.key_bitmap;
        u8g2.drawXBM(key.x, key.y, key.width, key.height, bmp);

    }
    u8g2.sendBuffer();
}

void update_key(uint8_t key_index, bool is_pressed, bool* _pressed) {
    // update the pressed array
    _pressed[key_index] = is_pressed;
    // update the display buffer
    const key_struct key = keys[key_index];
    // if pressed then draw the pressed bitmap else draw the normal bitmap
    const unsigned char* bmp = is_pressed ? key.key_pressed : key.key_bitmap;
    // set the color to 1 if pressed else 0
    is_pressed ? u8g2.setDrawColor(0) : u8g2.setDrawColor(1);
    u8g2.drawXBM(key.x, key.y, key.width, key.height, bmp);
}

void animate_keyboard(){
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        if (currentKey == 12) {
            currentKey = 0;
        }

        update_key(currentKey, true, pressed);
        u8g2.sendBuffer();

        update_key(currentKey, false, pressed);
        currentKey++;
    }
}
