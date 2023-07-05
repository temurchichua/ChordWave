#include <Arduino.h>

#include "keyboard/keyboard.h"
#include "midible/midioverble.h"
#include "keyboard/lights.h"


void setup(void) {
    Serial.begin(115200);

    u8g2.init(0x3C);
    init_keypad();
    init_leds();

    print_keyboard();

//    while (true) {
//        animate_leds();
//    }
    setup_midi_over_ble();
    Serial.println("Setup complete");
}
void loop(void) {
    scan_keypad();
    process_pressed_key();
    midi_loop();
}