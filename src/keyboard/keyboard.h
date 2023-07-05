//
// Created by Temur on 31/05/2023.
//
#pragma once
#ifndef CHORDWAVE_KEYBOARD_H
#define CHORDWAVE_KEYBOARD_H
#include <U8g2lib.h>
#include <ArduinoQueue.h>
#include <Wire.h>

#include "display/display.h"

// Queue for updating the pressed array
typedef struct {
    uint8_t key_index;
    bool is_pressed;
} queueItem;

#define NUM_BTN_COLUMNS (4)
#define NUM_BTN_ROWS (4)

#define MAX_DEBOUNCE (2)


// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_BUTTONS_ADDRESS = 0x3F; // SX1509 I2C address
static int8_t debounce_count[NUM_BTN_COLUMNS][NUM_BTN_ROWS]; // debounce counter for each button
// it is used to count the number of times a button is read as pressed

// functions
void init_keypad(); // Initialize the keypad
void scan_keypad(); // scan keypad for pressed keys and update the queue
void print_keyboard(); // Print the full keyboard to the display
void animate_keyboard(); // Animate the keyboard

void update_key_by_index(uint8_t key_index, bool is_pressed); // Update the pressed array by index
void print_key(uint8_t key_index, bool print ); // Print a single key to the display
void process_pressed_key(); // Check the queue and update the pressed array and display
#endif //CHORDWAVE_KEYBOARD_H
