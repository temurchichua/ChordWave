//
// Created by Temur on 05/07/2023.
//

#include "lights.h"

SX1509 leds_io;
// pins are conected to the grounds (left side of 4x4 matrix)
static const uint8_t rgb_grounds[LED_COLUMNS]         = {0, 1, 2, 3};

// array represent rows of rgb lights with [RED, GREEN, BLUE] pin numbers
static const uint8_t rgb_rows[LED_ROWS][NUM_COLORS]   = {{4, 5, 6},
                                                         {7, 8, 9},
                                                         {10, 11, 12},
                                                         {13, 14, 15}};
// array to store the color state of each led
static colors led_colors[LED_COLUMNS][LED_ROWS]; // array of colors for each led

// Initialize the leds
void init_leds(){

    // LED columns
    // Call io.begin(<address>) to initialize the SX1509. If it
    // successfully communicates, it'll return 1.
    bool io_success;
    io_success = leds_io.begin(SX1509_LEDS_ADDRESS);
    if (!io_success) {
        Serial.print("Failed to communicate with Buttons Extender at 0x");
        Serial.print(SX1509_LEDS_ADDRESS, HEX);
        Serial.println(".");
        while (1)
            ; // If we fail to communicate, loop forever.
    };

    for (uint8_t i = 0; i < LED_COLUMNS; i++) {
        leds_io.pinMode(rgb_grounds[i], OUTPUT);
        // with nothing selected by default
        leds_io.digitalWrite(rgb_grounds[i], HIGH);
    }
    // LED drive lines
    for (uint8_t i = 0; i < LED_ROWS; i++) {
        for (uint8_t j = 0; j < NUM_COLORS; j++) {
            leds_io.pinMode(rgb_rows[i][j], OUTPUT); // set the pin to output
            leds_io.digitalWrite(rgb_rows[i][j], LOW); // set the pin to low (off) default
        }
    }
    Serial.println("SX1509 LEDs: OK.");

}

void print_leds(void * parameter){
    // scan over the matrix and turn on the leds according to the led_colors array
    // make sure to only turn the color on that is specified in the led colors array
    // iterate over columns
    for (uint8_t i = 0; i < LED_COLUMNS; i++) {
        // array with values of pins that will be pulled high
        uint8_t high_pins[LED_ROWS] = {99, 99, 99, 99};
        // turn the ground pin low to turn on the leds
        leds_io.digitalWrite(rgb_grounds[i], LOW);
        // iterate over rows
        for (uint8_t j = 0; j < LED_ROWS; j++) {
            // check the color of the led and turn it on if it is specified in the led_colors array
            uint8_t pin;
            switch (led_colors[i][j]) {
                case RED:
                    pin = rgb_rows[j][0];
                    break;
                case GREEN:
                    pin = rgb_rows[j][1];
                    break;
                case BLUE:
                    pin = rgb_rows[j][2];
                    break;
                default:
                    pin = 99;
                    break;
            }
            if (pin != 99) {
                leds_io.digitalWrite(pin, HIGH);
                high_pins[j] = pin;
            } else {
                leds_io.digitalWrite(rgb_rows[j][0], LOW);
                leds_io.digitalWrite(rgb_rows[j][1], LOW);
                leds_io.digitalWrite(rgb_rows[j][2], LOW);
            }

        }
        delay(1);
        // turn the ground pin high to turn off the leds
        leds_io.digitalWrite(rgb_grounds[i], HIGH);
        // turn off the rows that were turned on
        for (uint8_t j = 0; j < LED_ROWS; j++) {
            if (high_pins[j] != 99) {
                leds_io.digitalWrite(high_pins[j], LOW);
            }
        }
    }

}

void change_led_color_by_index(uint8_t led_index, colors color){
    // turn the ground pin low to turn on the leds
    uint8_t i = led_index / LED_ROWS;
    uint8_t j = led_index % LED_ROWS;
    led_colors[i][j] = color;
}

void change_led_color_by_array(uint8_t i, uint8_t j, colors color){
    // turn the ground pin low to turn on the leds
    led_colors[i][j] = color;
}