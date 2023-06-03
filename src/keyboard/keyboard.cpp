//
// Created by Temur on 31/05/2023.
//

#include "keyboard.h"
#include "display/display.h"


bool pressed[12];

const unsigned long interval = 100;  // Animation interval in milliseconds
unsigned long previousMillis = 0;
uint8_t currentKey = 0;
display u8g2 = display(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void initial_setup() {
    u8g2.init();
    print_keyboard();
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
    const key_struct key = keys[key_index];
    // update the pressed array
    _pressed[key_index] = is_pressed;
    // if pressed then draw the pressed bitmap else draw the normal bitmap
    const unsigned char* bmp = is_pressed ? key.key_pressed : key.key_bitmap;
    // set the color to 1 if pressed else 0
    is_pressed ? u8g2.setDrawColor(0) : u8g2.setDrawColor(1);
    // update the display buffer
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
