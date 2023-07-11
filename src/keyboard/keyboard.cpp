//
// Created by Temur on 31/05/2023.
//
#include "keyboard.h"
#include "chords.h"
#include "midible/midioverble.h"
#include <SparkFunSX1509.h>               //Click here for the library: http://librarymanager/All#SparkFun_SX1509
#include "lights.h"

SX1509 io;                        // Create an SX1509 object for buttons

ArduinoQueue<queueItem> update_queue(4);
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

    Serial.println("SX1509 Buttons: OK.");
}

// scan keypad for pressed keys and update the queue
void scan_keypad(void * parameter){
    uint8_t val;
    uint8_t i, j = 0;

    while (i < NUM_BTN_COLUMNS) {
        io.digitalWrite(button_grounds[i], LOW); // select the column to read

        // Read the button inputs
        for ( j = 0; j < NUM_BTN_ROWS; j++) {
            val = io.digitalRead(button_rows[j]);
            uint8_t key_index = (i * NUM_BTN_ROWS) + j;

            if (val == LOW) {
                // active low: val is low when btn is pressed
                if (debounce_count[i][j] < MAX_DEBOUNCE) {
                    debounce_count[i][j]++;
                    if (debounce_count[i][j] == MAX_DEBOUNCE ) {
                        Serial.print("Key Down ");
                        Serial.println((i * NUM_BTN_ROWS) + j);
                        if (key_index < 12)
                            update_key_by_index(key_index, true);
                        else
                            change_chord_type((chord_type) (key_index - 12));
                    }
                }
            } else {
                // otherwise, button is released
                if (debounce_count[i][j] > 0) {
                    debounce_count[i][j]--;
                    if (debounce_count[i][j] == 0 ) {
                        Serial.print("Key Up ");
                        Serial.println((i * NUM_BTN_ROWS) + j);
                        if (key_index < 12)
                            update_key_by_index(key_index, false);
                    }
                }
            }
        }

        // deselect the column
        io.digitalWrite(button_grounds[i], HIGH);
        i++;
    }
}

void update_key_by_index(uint8_t key_index, bool is_pressed) {
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

void process_pressed_key(void * parameter) {
    if (!update_queue.isEmpty()) {
        queueItem item = update_queue.dequeue();
        chord_struct active_chord = chords[current_chord_type][item.key_index];

        uint8_t number_of_notes = 3;
        if ( current_chord_type == augmented )
            number_of_notes = 4;

        Serial.print("Playing Chord: ");
        Serial.print(active_chord.name);
        Serial.print(" ");
        Serial.print(type_to_string(current_chord_type));
        Serial.print(" with notes at: ");

        midi_chord midi_chord_item;
        midi_chord_item.is_pressed = item.is_pressed;
        midi_chord_item.number_of_notes = number_of_notes;

        for (int i = 0; i < number_of_notes; ++i) {
            uint8_t key_index = active_chord.keys[i];

            Serial.print(key_index);
            Serial.print(" ");

            midi_chord_item.notes[i] = keys[key_index].midi_note;
            pressed[key_index] = item.is_pressed;
            print_key(key_index, true);
            // change the color of light in array to red if pressed
            colors currentColor;
            if (item.is_pressed) {
                if (i == 0)
                    currentColor = RED;
                else
                    currentColor = GREEN;
            } else {
                currentColor = noLight;
            }
            change_led_color_by_index(key_index, currentColor);
        }

        midi_chords_queue.enqueue(midi_chord_item);
        Serial.println();
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