//
// Created by Temur on 05/07/2023.
//

#ifndef CHORDWAVE_LIGHTS_H
#define CHORDWAVE_LIGHTS_H
//config variables
#include <Arduino.h>
#include <Wire.h>           // Include the I2C library (required)
#include "SparkFunSX1509.h"

#define LED_COLUMNS (4)
#define LED_ROWS (4)
#define NUM_COLORS (3)

enum colors {
    noLight,
    RED,
    GREEN,
    BLUE,
};

const byte SX1509_LEDS_ADDRESS = 0x3E;   // SX1509 I2C address

// functions
void init_leds(); // Initialize the leds
void print_leds(); // Print the full leds to the display
void animate_leds(); // Animate the leds
void change_led_color_by_index(uint8_t led_index, colors color); // Change the color of a single led
void change_led_color_by_array(uint8_t i, uint8_t j, colors color); // Change the color of a single led by array

#endif //CHORDWAVE_LIGHTS_H
