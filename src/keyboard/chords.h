//
// Created by Temur on 04/06/2023.
//

#ifndef CHORDWAVE_CHORDS_H
#define CHORDWAVE_CHORDS_H
#include "key_bitmaps.h"

// Piano Keys
typedef struct {
    const char* note;
    const char keypad;
    const int x;
    const int y;
    const int width;
    const int height;

    const unsigned char* key_bitmap;
    const unsigned char* key_pressed;
} key_struct;

const key_struct keys[12] = {
        {"C", '1', 9, 20, 7, 40, left, left_pressed},
        {"C#", '2',13, 20, 6, 25, black, black_pressed},
        {"D", '3',17, 20, 7, 40, middle, middle_pressed},
        {"D#", 'A',21, 20, 6, 25, black, black_pressed},
        {"E", '4',25, 20, 7, 40, right, right_pressed},
        {"F", '5',33, 20, 7, 40, left, left_pressed},
        {"F#", '6',37, 20, 6, 25, black, black_pressed},
        {"G", 'B',41, 20, 7, 40, middle, middle_pressed},
        {"G#", '7',45, 20, 6, 25, black, black_pressed},
        {"A", '8',49, 20, 7, 40, middle, middle_pressed},
        {"A#", '9',53, 20, 6, 25, black, black_pressed},
        {"B", 'C',57, 20, 7, 40, right, right_pressed},
};
// Define the chord types
enum chord_type {
    major,
    minor,
    diminished,
    augmented
};

// Define the chord structure
typedef struct {
    const char* name;
    const int keys[4]; // indexes to keys that create the chord
} chord_struct;

// Define the chord data
const chord_struct chords[4][12] = {
    // Major chords
    {
        {"C Major", {0, 4, 7}},
        {"C# Major", {1, 5, 8}},
        {"D Major", {2, 6, 9}},
        {"D# Major", {3, 7, 10}},
        {"E Major", {4, 8, 11}},
        {"F Major", {5, 9, 0}},
        {"F# Major", {6, 10, 1}},
        {"G Major", {7, 11, 2}},
        {"G# Major", {8, 0, 3}},
        {"A Major", {9, 1, 4}},
        {"A# Major", {10, 2, 5}},
        {"B Major", {11, 3, 6}}
    },

    // Minor chords
    {
        {"C Minor", {0, 3, 7}},
        {"C# Minor", {1, 4, 8}},
        {"D Minor", {2, 5, 9}},
        {"D# Minor", {3, 6, 10}},
        {"E Minor", {4, 7, 11}},
        {"F Minor", {5, 8, 0}},
        {"F# Minor", {6, 9, 1}},
        {"G Minor", {7, 10, 2}},
        {"G# Minor", {8, 11, 3}},
        {"A Minor", {9, 0, 4}},
        {"A# Minor", {10, 1, 5}},
        {"B Minor", {11, 2, 6}}
    },

    // Diminished chords
    {
        {"C Diminished", {0, 3, 6}},
        {"C# Diminished", {1, 4, 7}},
        {"D Diminished", {2, 5, 8}},
        {"D# Diminished", {3, 6, 9}},
        {"E Diminished", {4, 7, 10}},
        {"F Diminished", {5, 8, 11}},
        {"F# Diminished", {6, 9, 0}},
        {"G Diminished", {7, 10, 1}},
        {"G# Diminished", {8, 11, 2}},
        {"A Diminished", {9, 0, 3}},
        {"A# Diminished", {10, 1, 4}},
        {"B Diminished", {11, 2, 5}}
    },

    // 7th chords
    {
        {"C 7th", {0, 4, 7, 10}},
        {"C# 7th", {1, 5, 8, 11}},
        {"D 7th", {2, 6, 9, 0}},
        {"D# 7th", {3, 7, 10, 1}},
        {"E 7th", {4, 8, 11, 2}},
        {"F 7th", {5, 9, 0, 3}},
        {"F# 7th", {6, 10, 1, 4}},
        {"G 7th", {7, 11, 2, 5}},
        {"G# 7th", {8, 0, 3, 6}},
        {"A 7th", {9, 1, 4, 7}},
        {"A# 7th", {10, 2, 5, 8}},
        {"B 7th", {11, 3, 6, 9}}
    }
};

chord_struct find_chord_by_key_index(uint8_t key_index, chord_type type);

#endif //CHORDWAVE_CHORDS_H