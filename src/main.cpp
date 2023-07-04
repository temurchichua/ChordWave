#include <Arduino.h>

#include "keyboard/keyboard.h"


void setup(void) {
    Serial.begin(115200);

    u8g2.init(0x3C);
    init_keypad();
    print_keyboard();

    Serial.println("Setup complete");
//    keypad.addEventListener(keypadEvent);
}
void loop(void) {
    scan_keypad();
    check_and_display_key();
}