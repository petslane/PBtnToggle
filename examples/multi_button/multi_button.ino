#include "PBtnToggle.h"

PBtnToggle* btnLeft = new PBtnToggle(7, HIGH);
PBtnToggle* btnRight = new PBtnToggle(8, HIGH);

void setup() {
    Serial.begin(9600);
    Serial.println("Setup");

    btnLeft->onPress(onButtonPress);
    btnLeft->onLongPress(onButtonLongPress);
    btnLeft->onRelease(onButtonRelease);

    btnRight->onPress(onButtonPress);
    btnRight->onLongPress(onButtonLongPress);
    btnRight->onRelease(onButtonRelease);
}

void onButtonPress(int btn, int status) {
    if (btnLeft->getPin() == btn) {
        Serial.println("Left button pressed");
    } else if (btnRight->getPin() == btn) {
        Serial.println("Right button pressed");
    }
}
bool onButtonLongPress(int btn, int status) {
    if (btnLeft->getPin() == btn) {
        Serial.println("Going left until released");
    } else if (btnRight->getPin() == btn) {
        Serial.println("Going right until released");
    }
    return false;
}
void onButtonRelease(int btn, int status) {
    if (btnLeft->getPin() == btn) {
        Serial.println("Left button released");
    } else if (btnRight->getPin() == btn) {
        Serial.println("Right button released");
    }
}

void loop() {
    btnLeft->check();
    btnRight->check();
}
