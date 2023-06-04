//
// Created by Temur on 31/05/2023.
//
#include "keyboard.h"
#include "chords.h"

enum key_size{
    x4x4,
    x1x4
};

key_size pad_size = x1x4;

ArduinoQueue<queueItem> update_queue(24);
bool pressed[12];
byte rowPins[4];
byte colPins[4];
byte ROWS;
byte COLS;
char keyboard[4][4] = {
        {'1','2','3', 'A'},
        {'4','5','6', 'B'},
        {'7','8','9', 'C'},
        {'*','0','#', 'D'}
};

Keypad keypad = Keypad( makeKeymap(keyboard), rowPins, colPins, ROWS, COLS );

void init_keypad(){
    if (pad_size == x4x4){

        ROWS = 4;
        COLS = 4;

        byte rowPins[4] = {D0, D3, D4, D5}; //connect to the row pinouts of the keypad
        byte colPins[4] = {D6, D7, D8, D9}; //connect to the column pinouts of the keypad


        keypad = Keypad( makeKeymap(keyboard), rowPins, colPins, ROWS, COLS );


    } else {
        ROWS = 1;
        COLS = 4;

        rowPins[0] = D0;

        colPins[0] = D6;
        colPins[1] = D5;
        colPins[2] = D9;
        colPins[3] = D7;

        keyboard[0][0] = '1';
        keyboard[0][1] = '2';
        keyboard[0][2] = '3';
        keyboard[0][3] = '4';
    }

    keypad = Keypad(makeKeymap((char*)keyboard), rowPins, colPins, ROWS, COLS);

}

// scan keypad for pressed keys and update the queue
void scan_keypad(){
    if (keypad.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( keypad.key[i].stateChanged )   // Only find keys that have changed state.
            {
                chord_struct chord_keys = find_chord_by_key_index(keypad.key[i].kcode, chord_type::major);

                switch (keypad.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                        for (int j = 0; j < 3; j++) {
                            update_key_by_index(chord_keys.keys[j], true);
                        }
                        break;
                    case HOLD:
                        break;
                    case RELEASED:
                        for (int j = 0; j < 3; j++) {
                            update_key_by_index(chord_keys.keys[j], false);
                        }
                        break;
                    case IDLE:
                        break;
                }
            }
        }
    }
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
