//
// Created by Temur on 31/05/2023.
//
#include "keyboard.h"
#include "chords.h"
#include <SparkFunSX1509.h> //Click here for the library: http://librarymanager/All#SparkFun_SX1509

SX1509 io_buttons;                        // Create an SX1509 object for buttons
SX1509 io_leds;                           // Create an SX1509 object for leds

ArduinoQueue<queueItem> update_queue(24);
bool pressed[12];

char keyMap[BTN_COLUMNS][BTN_ROWS] = {
        {'1','2','3', 'A'},
        {'4','5','6', 'B'},
        {'7','8','9', 'C'},
        {'*','0','#', 'D'}
};

void init_keypad(){
    // Call io.begin(<address>) to initialize the SX1509. If it
    // successfully communicates, it'll return 1.
    bool io_success;
    io_success = io_buttons.begin(SX1509_BUTTONS_ADDRESS);
    if (!io_success) {
        Serial.print("Failed to communicate with Buttons Extender at 0x");
        Serial.print(SX1509_BUTTONS_ADDRESS, HEX);
        Serial.println(".");
        while (1)
            ; // If we fail to communicate, loop forever.
    };
    io_buttons.keypad(BTN_ROWS, BTN_COLUMNS,
                      SLEEP_TIME, SCAN_TIME, DEBOUNCE_TIME);

    // Set up the Arduino interrupt pin as an input w/
    // internal pull-up. (The SX1509 interrupt is active-low.)
    pinMode(INT_PIN, INPUT_PULLUP);

    Serial.println("SX1509 Buttons: OK.");
}

// Compared to the keypad in keypad.ino, this keypad example
// is a bit more advanced. We'll use these varaibles to check
// if a key is being held down, or has been released. Then we
// can kind of emulate the operation of a computer keyboard.
unsigned int previousKeyData = 0;         // Stores last key pressed
unsigned int holdCount, releaseCount = 0; // Count durations
const unsigned int holdCountMax = 15;     // Key hold limit
const unsigned int releaseCountMax = 100; // Release limit

// scan keypad for pressed keys and update the queue
void scan_keypad(){
    // Use io.readKeypad() to get the raw keypad row/column
    unsigned int keyData = io_buttons.readKeypad();

    // Then use io.getRow() and io.getCol() to parse that
    // data into row and column values.
    byte row = io_buttons.getRow(keyData);
    byte col = io_buttons.getCol(keyData);
    // Then plug row and column into keyMap to get which
    // key was pressed.
    char key = keyMap[row][col];

    // If it's a new key pressed
    if (keyData != previousKeyData) {
        holdCount = 0;               // Reset hold-down count
        Serial.print("Key pressed: "); // Print the key
        Serial.println(key);
    } else {
        // If the button's being held down:
        holdCount++;                  // Increment holdCount
        if (holdCount > holdCountMax) // If it exceeds threshold
            Serial.print("Holding: "); // Print "Holding: "
            Serial.println(key);          // Print the key
    }
    releaseCount = 0;          // Clear the releaseCount variable
    previousKeyData = keyData; // Update previousKeyData
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
