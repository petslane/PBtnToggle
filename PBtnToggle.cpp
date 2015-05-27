#include "PBtnToggle.h"

// PBtnToggle::state bit values:
//   0 - pressed, timer started
//   1 - onPress triggered
//   2 - onLongPress triggered
//   3 - onRelease triggered
//   4 - check running
//   5 - pressed button state
//   6 - last button state
//   7 - last event was long press, ignore next release, then press ad clear on press

PBtnToggle::PBtnToggle(int btn_pin, int pressed_state) {
    PBtnToggle::btn = btn_pin;
    pinMode(PBtnToggle::btn, INPUT);
    PBtnToggle::state = B00000000;
    if (pressed_state==HIGH) {
        bitSet(PBtnToggle::state, 5);
    }
    PBtnToggle::onPressCallback = NULL;
    PBtnToggle::onLongPressCallback = NULL;
    PBtnToggle::onReleaseCallback = NULL;
}

void PBtnToggle::onPress(ToggleFunc callback) {
    PBtnToggle::onPressCallback = callback;
}

void PBtnToggle::onLongPress(LongPressFunc callback) {
    PBtnToggle::onLongPressCallback = callback;
}

void PBtnToggle::onRelease(ToggleFunc callback) {
    PBtnToggle::onReleaseCallback = callback;
}

void PBtnToggle::checkChange() {
    bool btn_pressed = (digitalRead(PBtnToggle::btn) == HIGH) == (bool) bitRead(PBtnToggle::state, 5);
    PBtnToggle::timer = millis();
}

void PBtnToggle::triggerEvents(bool btn_pressed) {
    if (btn_pressed) {
        if (!bitRead(PBtnToggle::state, 1)) {
            if (!bitRead(PBtnToggle::state, 2)) {
                PBtnToggle::onPressCallback(PBtnToggle::btn, HIGH);
            }
            if (!PBtnToggle::onLongPressCallback) {
                bitClear(PBtnToggle::state, 0);
            }

            bitSet(PBtnToggle::state, 1);
            bitClear(PBtnToggle::state, 2);
            bitClear(PBtnToggle::state, 3);
            PBtnToggle::timer = millis();
        }
        if (PBtnToggle::onLongPressCallback && !bitRead(PBtnToggle::state, 2) && PBtnToggle::timer + PBTNTOGGLE_LONGCLICK_TIME < millis()) {
            bool noSkip = PBtnToggle::onLongPressCallback(PBtnToggle::btn, HIGH);
            if (noSkip) {
                bitClear(PBtnToggle::state, 1);
                bitSet(PBtnToggle::state, 2);
                bitClear(PBtnToggle::state, 3);
            }
            bitClear(PBtnToggle::state, 0);
        }
    } else {
        if (!bitRead(PBtnToggle::state, 3) && !bitRead(PBtnToggle::state, 2)) {
            PBtnToggle::onReleaseCallback(PBtnToggle::btn, HIGH);
            bitClear(PBtnToggle::state, 0);

            bitClear(PBtnToggle::state, 1);
            bitClear(PBtnToggle::state, 2);
            bitSet(PBtnToggle::state, 3);
        }
    }
}

void PBtnToggle::check() {
    if (bitRead(PBtnToggle::state, 4)) {
        return;
    }
    bitSet(PBtnToggle::state, 4);
    // get current button state
    bool btn_pressed = (digitalRead(PBtnToggle::btn) == HIGH) == (bitRead(PBtnToggle::state, 5) == 1);
    // has state changed during last check
    bool btn_state_changed = (bool) btn_pressed != (bool) bitRead(PBtnToggle::state, 6);

    if (btn_pressed) {
        bitSet(PBtnToggle::state, 6);
    } else {
        bitClear(PBtnToggle::state, 6);
    }

    if (!btn_state_changed && PBtnToggle::timer > 0 && PBtnToggle::timer + PBTNTOGGLE_CLICK_DEBOUNCE_TIME < millis() && bitRead(PBtnToggle::state, 0)) {
        PBtnToggle::triggerEvents(btn_pressed);
    }
    if (btn_state_changed) {
        PBtnToggle::timer = millis();
        bitSet(PBtnToggle::state, 0);
    }
    bitClear(PBtnToggle::state, 4);
}

int PBtnToggle::getPin() {
    return this->btn;
}
