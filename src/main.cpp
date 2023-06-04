#include <Arduino.h>

#include "keyboard/keyboard.h"


void setup(void) {
    u8g2.init();
    print_keyboard();

    Serial.begin(9600);
//    keypad.addEventListener(keypadEvent);
}
void loop(void) {
    if (keypad.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( keypad.key[i].stateChanged )   // Only find keys that have changed state.
            {
                switch (keypad.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                        update_key_by_keypad(keypad.key[i].kchar, true);
                        break;
                    case HOLD:
                        break;
                    case RELEASED:
                        update_key_by_keypad(keypad.key[i].kchar, false);
                        break;
                    case IDLE:
                        break;
                }
                Serial.print(" Key ");
                Serial.print(keypad.key[i].kcode);
            }
        }
    }
    check_and_display_key();
}