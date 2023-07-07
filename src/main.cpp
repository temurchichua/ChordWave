#include <Arduino.h>

#include "keyboard/keyboard.h"
#include "midible/midioverble.h"
#include "keyboard/lights.h"
#include "keyboard/chords.h"


void setup(void) {
    Serial.begin(115200);

    u8g2.init(0x3C);
    print_keyboard();
    Serial.println("Display Connected: Ok");

    init_keypad();
    init_leds();


//    while (true) {
//        animate_leds();
//    }
    setup_midi_over_ble();
    // set the chord type to major
    change_chord_type(major);
    Serial.println("Setup complete");
}
void loop(void) {
    scan_keypad();
    process_pressed_key();
    midi_loop();
    print_leds();
}