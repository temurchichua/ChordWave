#include <Arduino.h>

#include "keyboard/keyboard.h"


void setup(void) {
    u8g2.init(0x3C);
    init_keypad();
    print_keyboard();

    Serial.begin(115200);
//    keypad.addEventListener(keypadEvent);
}
void loop(void) {
    if (digitalRead(INT_PIN) == LOW){
        scan_keypad();
    }
    check_and_display_key();
}