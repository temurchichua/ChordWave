//
// Created by Temur on 05/07/2023.
//

#include "lights.h"

SX1509 leds_io;
static const uint8_t rgb_grounds[LED_COLUMNS]         = {0, 1, 2, 3};

static const uint8_t rgb_rows[LED_ROWS][NUM_COLORS]   = {{4, 5, 6}, 
                                                         {7, 8, 9}, 
                                                         {10, 11, 12}, 
                                                         {13, 14, 15}};

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
    // turn the first led on
    leds_io.digitalWrite(rgb_rows[0][0], HIGH);
    leds_io.digitalWrite(rgb_grounds[0], LOW);

}

// Animate the leds using millis()
unsigned long prevMillis = 0;
const long interval = 200; // interval at which to animate (milliseconds)
uint8_t i, j = 0;

void animate_leds(){
    unsigned long currentMillis = millis();
    if (currentMillis - prevMillis >= interval) {
        prevMillis = currentMillis;
        led_colors[i][j] = static_cast<colors>((led_colors[i][j] + 1) % NUM_COLORS);
        Serial.print("Updated led_colors[");
        Serial.print(i);
        Serial.print("][");
        Serial.print(j);
        Serial.print("] to ");
        Serial.println(led_colors[i][j]);

        change_led_color_by_array(i, j, led_colors[i][j]);

        j++;
        if (j >= NUM_COLORS) {
            j = 0;
            i++;
            if (i >= LED_ROWS) {
                // pull the ground pin high to turn off the leds
                i = 0;
            }
        }
    }
}

void change_led_color_by_array(uint8_t i, uint8_t j, colors color){
    // turn the ground pin low to turn on the leds
    leds_io.digitalWrite(rgb_grounds[i], LOW);
    // turn all leds off but the one specified in color
    for (uint8_t k = 0; k < NUM_COLORS; k++) {
        if (k != color) {
            leds_io.digitalWrite(rgb_rows[i][k], LOW);
        } else {
            leds_io.digitalWrite(rgb_rows[i][k], HIGH);
        }
    }
}