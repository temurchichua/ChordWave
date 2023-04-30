#include "Arduino.h"

#include "display/display.h"

void setup()
{
    Serial.begin(115200);
    display_setup();
    testscrolltext("es mushaobs");
}

void loop()
{
}

