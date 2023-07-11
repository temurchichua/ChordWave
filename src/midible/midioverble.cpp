//
// Created by Temur on 05/07/2023.
//

#include "midioverble.h"

ArduinoQueue<midi_chord> midi_chords_queue(4);

void setup_midi_over_ble() {
    Serial.println("Setting up MIDI over BLE");
    BLEMidiServer.begin("ChordWave");

    Serial.println("Waiting for connections...");
    BLEMidiServer.enableDebugging();  // Uncomment if you want to see some debugging output from the library (not much for the server class...)
}

// use millis() to control the note duration
unsigned long note_start_time = 0;
unsigned long note_duration = 0;


void midi_loop(void * parameter){
    if(BLEMidiServer.isConnected() && !midi_chords_queue.isEmpty()) {
        midi_chord item = midi_chords_queue.dequeue();
        if (item.is_pressed) {
            for (int i = 0; i < item.number_of_notes; ++i) {
                BLEMidiServer.noteOn(0, item.notes[i], 127);
            }
        } else {
            for (int i = 0; i < item.number_of_notes; ++i) {
                BLEMidiServer.noteOff(0, item.notes[i], 127);
            }
        }
    }
}