//
// Created by Temur on 05/07/2023.
//

#ifndef CHORDWAVE_MIDIOVERBLE_H
#define CHORDWAVE_MIDIOVERBLE_H
#include <Arduino.h>
#include <BLEMidi.h>
#include <ArduinoQueue.h>

typedef struct {
    uint8_t number_of_notes;
    uint8_t notes[4];
    bool is_pressed;
} midi_chord;

extern ArduinoQueue<midi_chord> midi_chords_queue;

void setup_midi_over_ble();
void midi_loop(void * parameter);
#endif //CHORDWAVE_MIDIOVERBLE_H
