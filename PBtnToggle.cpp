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
    PBtnToggle::btn_ = btn_pin;
    pinMode(PBtnToggle::btn_, INPUT);
    PBtnToggle::state_ = B00000000;
    if (pressed_state==HIGH) {
        bitSet(PBtnToggle::state_, 5);
    }
    PBtnToggle::on_press_callback_ = NULL;
    PBtnToggle::on_long_press_callback_ = NULL;
    PBtnToggle::on_release_callback_ = NULL;
}

/**
 * Set callback function for on button press event
 *
 * @param callback Callback function pointer
 */
void PBtnToggle::onPress(ToggleFunc callback) {
    PBtnToggle::on_press_callback_ = callback;
}

/**
 * Set callback function for on long button press event
 *
 * @param callback Callback function pointer
 */
void PBtnToggle::onLongPress(LongPressFunc callback) {
    PBtnToggle::on_long_press_callback_ = callback;
}

/**
 * Set callback function for on button release event
 *
 * @param callback Callback function pointer
 */
void PBtnToggle::onRelease(ToggleFunc callback) {
    PBtnToggle::on_release_callback_ = callback;
}

void PBtnToggle::triggerEvents(bool btn_pressed) {
    if (btn_pressed) {
        if (!bitRead(PBtnToggle::state_, 1)) {
            if (!bitRead(PBtnToggle::state_, 2) && PBtnToggle::on_press_callback_) {
                PBtnToggle::on_press_callback_(PBtnToggle::btn_, bitRead(PBtnToggle::state_, 5)?HIGH:LOW);
            }
            if (!PBtnToggle::on_long_press_callback_) {
                bitClear(PBtnToggle::state_, 0);
            }

            bitSet(PBtnToggle::state_, 1);
            bitClear(PBtnToggle::state_, 2);
            bitClear(PBtnToggle::state_, 3);
            PBtnToggle::timer_ = millis();
        }
        if (PBtnToggle::on_long_press_callback_ && !bitRead(PBtnToggle::state_, 2) && PBtnToggle::timer_ + PBTNTOGGLE_LONGCLICK_TIME < millis()) {
            bool noSkip = PBtnToggle::on_long_press_callback_(PBtnToggle::btn_, bitRead(PBtnToggle::state_, 5)?HIGH:LOW);
            if (noSkip) {
                bitClear(PBtnToggle::state_, 1);
                bitSet(PBtnToggle::state_, 2);
                bitClear(PBtnToggle::state_, 3);
            }
            bitClear(PBtnToggle::state_, 0);
        }
    } else {
        if (!bitRead(PBtnToggle::state_, 3) && !bitRead(PBtnToggle::state_, 2)) {
            if (PBtnToggle::on_release_callback_) {
                PBtnToggle::on_release_callback_(PBtnToggle::btn_, !bitRead(PBtnToggle::state_, 5)?HIGH:LOW);
            }
            bitClear(PBtnToggle::state_, 0);

            bitClear(PBtnToggle::state_, 1);
            bitClear(PBtnToggle::state_, 2);
            bitSet(PBtnToggle::state_, 3);
        }
    }
}

/**
 * Read button state and return true if button is pressed down.
 */
bool PBtnToggle::is_btn_pressed_() {
    return (digitalRead(PBtnToggle::btn_) == HIGH) == (bitRead(PBtnToggle::state_, 5) == 1);
}

/**
 * Check button state and trigger events if needed
 */
void PBtnToggle::check() {
    if (bitRead(PBtnToggle::state_, 4)) {
        return;
    }
    bitSet(PBtnToggle::state_, 4);
    // get current button state
    bool btn_pressed = PBtnToggle::is_btn_pressed_();
    // has state changed during last check
    bool btn_state_changed = (bool) btn_pressed != (bool) bitRead(PBtnToggle::state_, 6);

    if (btn_pressed) {
        bitSet(PBtnToggle::state_, 6);
    } else {
        bitClear(PBtnToggle::state_, 6);
    }

    // trigger event if debounce time is passed
    if (!btn_state_changed && PBtnToggle::timer_ > 0 && PBtnToggle::timer_ + PBTNTOGGLE_CLICK_DEBOUNCE_TIME < millis() && bitRead(PBtnToggle::state_, 0)) {
        PBtnToggle::triggerEvents(btn_pressed);
    }
    // reset timer if state has changed
    if (btn_state_changed) {
        PBtnToggle::timer_ = millis();
        bitSet(PBtnToggle::state_, 0);
    }
    bitClear(PBtnToggle::state_, 4);
}

/**
 * Get pin number
 */
int PBtnToggle::getPin() {
    return this->btn_;
}
