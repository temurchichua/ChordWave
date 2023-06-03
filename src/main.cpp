#include <Arduino.h>
#include "keyboard/keyboard.h"


void setup(void) {
    initial_setup();
    print_keyboard();
}

void loop(void) {
    animate_keyboard();

}