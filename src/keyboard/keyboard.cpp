//
// Created by Temur on 31/05/2023.
//
#include "keyboard.h"
#include "chords.h"
#include <SparkFunSX1509.h>               //Click here for the library: http://librarymanager/All#SparkFun_SX1509

SX1509 io;                        // Create an SX1509 object for buttons
SX1509 io_leds;                           // Create an SX1509 object for leds

ArduinoQueue<queueItem> update_queue(24);
bool pressed[16]; // array of pressed keys
static const uint8_t button_grounds[NUM_BTN_COLUMNS] = {0, 1, 2, 3};
static const uint8_t button_rows[NUM_BTN_ROWS] = {8, 9, 10, 11};

void init_keypad(){
    // Call io.begin(<address>) to initialize the SX1509. If it
    // successfully communicates, it'll return 1.
    bool io_success;
    io_success = io.begin(SX1509_BUTTONS_ADDRESS);
    if (!io_success) {
        Serial.print("Failed to communicate with Buttons Extender at 0x");
        Serial.print(SX1509_BUTTONS_ADDRESS, HEX);
        Serial.println(".");
        while (1)
            ; // If we fail to communicate, loop forever.
    };
    // button columns
    for (uint8_t i = 0; i < NUM_BTN_COLUMNS; i++) {
        io.pinMode(button_grounds[i], OUTPUT);
        // with nothing selected by default
        io.digitalWrite(button_grounds[i], HIGH);
    }

    // button row input lines
    for (uint8_t i = 0; i < NUM_BTN_ROWS; i++) {
        io.pinMode(button_rows[i], INPUT_PULLUP);
    }

    // Initialize the debounce counter array
    for (uint8_t i = 0; i < NUM_BTN_COLUMNS; i++) {
        for (uint8_t j = 0; j < NUM_BTN_ROWS; j++) {
            debounce_count[i][j] = 0;
        }
    }

    // Set up the Arduino interrupt pin as an input w/
    // internal pull-up. (The SX1509 interrupt is active-low.)
    pinMode(INT_PIN, INPUT_PULLUP);

    Serial.println("SX1509 Buttons: OK.");
}

// scan keypad for pressed keys and update the queue
void scan_keypad(){
    //TODO: https://forum.pjrc.com/threads/42064-fix-behavior-of-keyPad-scanning-with-SX1509-port-expander
    static uint8_t current = 0;
    uint8_t val;
    uint8_t j;

    io.digitalWrite(button_grounds[current], LOW);

    // Read the button inputs
    for ( j = 0; j < NUM_BTN_ROWS; j++) {
        val = io.digitalRead(button_rows[j]);
        uint8_t key_index = (current * NUM_BTN_ROWS) + j;

        if (val == LOW) {
            // active low: val is low when btn is pressed
            if ( debounce_count[current][j] < MAX_DEBOUNCE) {
                debounce_count[current][j]++;
                if ( debounce_count[current][j] == MAX_DEBOUNCE ) {
                    Serial.print("Key Down ");
                    Serial.println((current * NUM_BTN_ROWS) + j);
                    update_key_by_index(key_index, true);
                }
            }
        } else {
            // otherwise, button is released
            if ( debounce_count[current][j] > 0) {
                debounce_count[current][j]--;
                if ( debounce_count[current][j] == 0 ) {
                    Serial.print("Key Up ");
                    Serial.println((current * NUM_BTN_ROWS) + j);
                    update_key_by_index(key_index, false);
                }
            }
        }
    }
    current++;
    if (current >= NUM_BTN_COLUMNS) {
        current = 0;
    }
}

void update_key_by_index(uint8_t key_index, bool is_pressed) {
    // update the pressed array
    pressed[key_index] = is_pressed;
    // add the key to the queue
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

void check_and_display_key() {
    if (!update_queue.isEmpty()) {
        queueItem item = update_queue.dequeue();
        pressed[item.key_index] = item.is_pressed;
        print_key(item.key_index, true);
    }
}

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
