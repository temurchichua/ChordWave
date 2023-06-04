//
// Created by Temur on 04/06/2023.
//
#include <Arduino.h>
#include "chords.h"

// find the chord by the index of the key
chord_struct find_chord_by_key_index(uint8_t key_index, chord_type type) {
    return chords[type][key_index];
}