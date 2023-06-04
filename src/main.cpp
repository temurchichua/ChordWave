#include <Arduino.h>

#include "keyboard/keyboard.h"

#include <Keypad.h>
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns

char keyboard[ROWS][COLS] = {
        {'1','2','3', 'A'},
        {'4','5','6', 'B'},
        {'7','8','9', 'C'},
        {'*','0','#', 'D'}
};


byte rowPins[ROWS] = {28, 29, 26, 27}; //connect to the column pinouts of the keypad
byte colPins[COLS] = {24, 25, 22, 23}; //connect to the row pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keyboard), rowPins, colPins, ROWS, COLS );

// Taking care of some special events.
void keypadEvent(KeypadEvent key){
    String eventDescription;
    eventDescription += key;

    switch (keypad.getState()){
        case PRESSED:
            eventDescription += "Pressed: ";
            update_key_by_keypad(key, true);
            break;

        case RELEASED:
            eventDescription += "Released: ";
            update_key_by_keypad(key, false);
            break;

        case HOLD:
            eventDescription += "Hold: ";
            break;
    }
    Serial.println(eventDescription);
}

void setup(void) {
    initial_setup();
    Serial.begin(9600);
    keypad.addEventListener(keypadEvent);
}

void loop(void) {
    char key = keypad.getKey();
    check_and_display_key();
//
//    if (key){
//        Serial.println(key);
//    }

}