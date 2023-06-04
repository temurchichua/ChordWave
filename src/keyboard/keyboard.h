//
// Created by Temur on 31/05/2023.
//

#ifndef CHORDWAVE_KEYBOARD_H
#define CHORDWAVE_KEYBOARD_H
#include "keyboard/key_bitmaps.h"
#include <U8g2lib.h>

typedef struct {
    const char* note;
    const char keypad;
    const int x;
    const int y;
    const int width;
    const int height;

    const unsigned char* key_bitmap;
    const unsigned char* key_pressed;
} key_struct;

const key_struct keys[12] = {
        {"C", '1', 9, 20, 7, 40, left, left_pressed},
        {"C#", '2',13, 20, 6, 25, black, black_pressed},
        {"D", '3',17, 20, 7, 40, middle, middle_pressed},
        {"D#", 'A',21, 20, 6, 25, black, black_pressed},
        {"E", '4',25, 20, 7, 40, right, right_pressed},
        {"F", '5',33, 20, 7, 40, left, left_pressed},
        {"F#", '6',37, 20, 6, 25, black, black_pressed},
        {"G", 'B',41, 20, 7, 40, middle, middle_pressed},
        {"G#", '7',45, 20, 6, 25, black, black_pressed},
        {"A", '8',49, 20, 7, 40, middle, middle_pressed},
        {"A#", '9',53, 20, 6, 25, black, black_pressed},
        {"B", 'C',57, 20, 7, 40, right, right_pressed},
};

// functions
void initial_setup();
void print_key(uint8_t);
void print_keyboard();
void animate_keyboard();

void update_key_by_keypad(char keypad, bool is_pressed);
void update_key_by_index(uint8_t key_index, bool is_pressed);
#endif //CHORDWAVE_KEYBOARD_H
