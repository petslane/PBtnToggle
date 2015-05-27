#include "PBtnToggle.h"

PBtnToggle* btn1 = new PBtnToggle(7, HIGH);

void setup() {
    Serial.begin(9600);

    Serial.println("Setup");

    btn1->onPress(onButtonPress);
    btn1->onLongPress(onButtonLongPress);
    btn1->onRelease(onButtonRelease);
}

void onButtonPress(int btn, int status) {
    Serial.print("Button pressed on pin ");
    Serial.println(btn);
}
bool onButtonLongPress(int btn, int status) {
    Serial.print("Long button press on pin ");
    Serial.println(btn);
    return true;
}
void onButtonRelease(int btn, int status) {
    Serial.print("Button released on pin ");
    Serial.println(btn);
}

void loop() {
    btn1->check();
}
