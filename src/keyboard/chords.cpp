//
// Created by Temur on 04/06/2023.
//
#include <Arduino.h>
#include "chords.h"
#include "lights.h"

// find the chord by the index of the key
chord_struct find_chord_by_key_index(uint8_t key_index, chord_type type) {
    return chords[type][key_index];
}

chord_type current_chord_type = major;

void change_chord_type(chord_type type) {
    Serial.print("Chord type changed to ");
    Serial.println(type_to_string(type));
    current_chord_type = type;
    // turn colors to all the other lights in that row off but the one that is pressed
    for (uint8_t i = 0; i < 4; ++i) {
        if (i != current_chord_type) {
            change_led_color_by_array(3, i, noLight);
        } else {
            change_led_color_by_array(3, i, BLUE);
        }
    }
}

char* type_to_string(chord_type type) {
    switch (type) {
        case major:
            return "Maj";
        case minor:
            return "Min";
        case diminished:
            return "Dim";
        case augmented:
            return "7th";
    }
}