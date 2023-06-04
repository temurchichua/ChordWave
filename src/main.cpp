#include <Arduino.h>

#include "keyboard/keyboard.h"


void setup(void) {
    u8g2.init();
    init_keypad();
    print_keyboard();

    Serial.begin(9600);
//    keypad.addEventListener(keypadEvent);
}
void loop(void) {
    scan_keypad();
    check_and_display_key();
}