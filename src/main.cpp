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

    xTaskCreate(
            scan_keypad,          /* Task function. */
            "Scan Pressed Keys",    /* String with name of task. */
            10000,               /* Stack size in bytes. */
            NULL,                /* Parameter passed as input of the task */
            1,                      /* Priority of the task. */
            NULL                 /* Task handle. */
        );

//    xTaskCreate(
//            process_pressed_key, /* Task function. */
//            "Process Pressed Keys", /* String with name of task. */
//            10000,               /* Stack size in bytes. */
//            NULL,                /* Parameter passed as input of the task */
//            1,                   /* Priority of the task. */
//            NULL                /* Task handle. */
//        );
//    xTaskCreate(
//            midi_loop,               /* Task function. */
//            "Send MIDI Signals out", /* String with name of task. */
//            10000,                   /* Stack size in bytes. */
//            NULL,                    /* Parameter passed as input of the task */
//            1,                       /* Priority of the task. */
//            NULL                     /* Task handle. */
//        );
//    xTaskCreate(
//            print_leds,               /* Task function. */
//            "Print LEDs", /* String with name of task. */
//            10000,                   /* Stack size in bytes. */
//            NULL,                    /* Parameter passed as input of the task */
//            2,                       /* Priority of the task. */
//            NULL                     /* Task handle. */
//        );

}

void loop(void) {
}