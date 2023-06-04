//
// Created by Temur on 31/05/2023.
//
#pragma once
#include "keyboard.h"

ArduinoQueue<queueItem> update_queue(24);
bool pressed[12];

// Keyboard matrix
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns

char keyboard[ROWS][COLS] = {
        {'1','2','3', 'A'},
        {'4','5','6', 'B'},
        {'7','8','9', 'C'},
        {'*','0','#', 'D'}
};

byte colPins[COLS] = {D6, D7, D8, D9}; //connect to the column pinouts of the keypad
byte rowPins[ROWS] = {D0, D3, D4, D5}; //connect to the row pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keyboard), rowPins, colPins, ROWS, COLS);

// Print the full keyboard to the display
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

void update_key_by_index(uint8_t key_index, bool is_pressed) {
    // update the pressed array
    queueItem item = {key_index, is_pressed};
    update_queue.enqueue(item);
}

void print_key(uint8_t key_index, bool print = false) {
    const key_struct key = keys[key_index];
    // if pressed then draw the pressed bitmap else draw the normal bitmap
    const unsigned char* bmp = pressed[key_index] ? key.key_pressed : key.key_bitmap;
    // set the color to 1 if pressed else 0
    pressed[key_index] ? u8g2.setDrawColor(0) : u8g2.setDrawColor(1);
    // update the display buffer
    u8g2.drawXBM(key.x, key.y, key.width, key.height, bmp);
    // send the buffer to the display
    if (print) {
        u8g2.sendBuffer();
    }
}

void update_key_by_keypad(char keypad, bool is_pressed) {
    for (uint8_t i = 0; i < 12; ++i) {
        const key_struct key_struct = keys[i];
        if (key_struct.keypad == keypad) {
            update_key_by_index(i, is_pressed);
            break;
        }
    }
}

void check_and_display_key() {
    if (!update_queue.isEmpty()) {
        queueItem item = update_queue.dequeue();
        pressed[item.key_index] = item.is_pressed;
        print_key(item.key_index, true);
    }
}

const unsigned long interval = 100;  // Animation interval in milliseconds
unsigned long previousMillis = 0;
uint8_t currentKey = 0;

void animate_keyboard(){
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        if (currentKey == 12) {
            currentKey = 0;
        }

        update_key_by_index(currentKey, true);
        u8g2.sendBuffer();

        update_key_by_index(currentKey, false);
        currentKey++;
    }
}
