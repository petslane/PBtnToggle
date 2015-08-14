#include "PBtnToggle.h"
#include "PBtnTogglePISO.h"
#include "PBtnTogglePISOController.h"

PBtnTogglePISOController* btnController = new PBtnTogglePISOController(2, 3, 2, 5);

// buttons wired to first PISO chip
PBtnTogglePISO* btn01 = new PBtnTogglePISO(0, 0);
PBtnTogglePISO* btn02 = new PBtnTogglePISO(0, 1);
PBtnTogglePISO* btn03 = new PBtnTogglePISO(0, 2);
PBtnTogglePISO* btn04 = new PBtnTogglePISO(0, 3);
PBtnTogglePISO* btn05 = new PBtnTogglePISO(0, 4);
PBtnTogglePISO* btn06 = new PBtnTogglePISO(0, 5);
PBtnTogglePISO* btn07 = new PBtnTogglePISO(0, 6);
PBtnTogglePISO* btn08 = new PBtnTogglePISO(0, 7);
// buttons wired to second PISO chip
PBtnTogglePISO* btn11 = new PBtnTogglePISO(1, 0);
PBtnTogglePISO* btn12 = new PBtnTogglePISO(1, 1);
PBtnTogglePISO* btn13 = new PBtnTogglePISO(1, 2);
PBtnTogglePISO* btn14 = new PBtnTogglePISO(1, 3);
PBtnTogglePISO* btn15 = new PBtnTogglePISO(1, 4);
PBtnTogglePISO* btn16 = new PBtnTogglePISO(1, 5);
PBtnTogglePISO* btn17 = new PBtnTogglePISO(1, 6);
PBtnTogglePISO* btn18 = new PBtnTogglePISO(1, 7);


void onButtonPress(int btn, int status) {
    Serial.print("Button pressed on pin ");
    Serial.println(btn);
}
bool onButtonLongPress(int btn, int status) {
    Serial.print("Long button press on pin ");
    Serial.println(btn);
    return false;
}
void onButtonRelease(int btn, int status) {
    Serial.print("Button released on pin ");
    Serial.println(btn);
}

void setup() {
    Serial.begin(9600);
    Serial.println("Setup");

    btnController->add(btn01);
    btnController->add(btn02);
    btnController->add(btn03);
    btnController->add(btn04);
    btnController->add(btn05);
    btnController->add(btn06);
    btnController->add(btn07);
    btnController->add(btn08);

    btnController->add(btn11);
    btnController->add(btn12);
    btnController->add(btn13);
    btnController->add(btn14);
    btnController->add(btn15);
    btnController->add(btn16);
    btnController->add(btn17);
    btnController->add(btn18);

    btn01->onPress(onButtonPress);
    btn01->onLongPress(onButtonLongPress);
    btn01->onRelease(onButtonRelease);
    btn02->onPress(onButtonPress);
    btn02->onLongPress(onButtonLongPress);
    btn02->onRelease(onButtonRelease);
    btn03->onPress(onButtonPress);
    btn03->onLongPress(onButtonLongPress);
    btn03->onRelease(onButtonRelease);
    btn04->onPress(onButtonPress);
    btn04->onLongPress(onButtonLongPress);
    btn04->onRelease(onButtonRelease);
    btn05->onPress(onButtonPress);
    btn05->onLongPress(onButtonLongPress);
    btn05->onRelease(onButtonRelease);
    btn06->onPress(onButtonPress);
    btn06->onLongPress(onButtonLongPress);
    btn06->onRelease(onButtonRelease);
    btn07->onPress(onButtonPress);
    btn07->onLongPress(onButtonLongPress);
    btn07->onRelease(onButtonRelease);
    btn08->onPress(onButtonPress);
    btn08->onLongPress(onButtonLongPress);
    btn08->onRelease(onButtonRelease);

    btn11->onPress(onButtonPress);
    btn11->onLongPress(onButtonLongPress);
    btn11->onRelease(onButtonRelease);
    btn12->onPress(onButtonPress);
    btn12->onLongPress(onButtonLongPress);
    btn12->onRelease(onButtonRelease);
    btn13->onPress(onButtonPress);
    btn13->onLongPress(onButtonLongPress);
    btn13->onRelease(onButtonRelease);
    btn14->onPress(onButtonPress);
    btn14->onLongPress(onButtonLongPress);
    btn14->onRelease(onButtonRelease);
    btn15->onPress(onButtonPress);
    btn15->onLongPress(onButtonLongPress);
    btn15->onRelease(onButtonRelease);
    btn16->onPress(onButtonPress);
    btn16->onLongPress(onButtonLongPress);
    btn16->onRelease(onButtonRelease);
    btn17->onPress(onButtonPress);
    btn17->onLongPress(onButtonLongPress);
    btn17->onRelease(onButtonRelease);
    btn18->onPress(onButtonPress);
    btn18->onLongPress(onButtonLongPress);
    btn18->onRelease(onButtonRelease);
}


void loop() {
    btnController->check();
}
