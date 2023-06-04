//
// Created by Temur on 31/05/2023.
//
#ifndef CHORDWAVE_KEYBOARD_H
#define CHORDWAVE_KEYBOARD_H
#include <U8g2lib.h>
#include <ArduinoQueue.h>
#include <Keypad.h>

#include "display/display.h"


extern Keypad keypad;

// Queue for updating the pressed array
typedef struct {
    uint8_t key_index;
    bool is_pressed;
} queueItem;


// functions
void init_keypad(); // Initialize the keypad
void scan_keypad(); // scan keypad for pressed keys and update the queue
void print_keyboard(); // Print the full keyboard to the display
void animate_keyboard(); // Animate the keyboard

void update_key_by_index(uint8_t key_index, bool is_pressed); // Update the pressed array by index
void print_key(uint8_t key_index, bool print ); // Print a single key to the display
void check_and_display_key(); // Check the queue and update the pressed array and display
#endif //CHORDWAVE_KEYBOARD_H
