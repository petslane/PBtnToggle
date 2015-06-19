#include "PBtnToggleBase.h"

// PBtnToggleBase::state_ bit values:
//   0 - pressed, timer started
//   1 - onPress triggered
//   2 - onLongPress triggered
//   3 - onRelease triggered
//   4 - check running
//   5 - pressed button state
//   6 - last button state
//   7 - last event was long press, ignore next release, then press ad clear on press

/**
 * Create button instance
 *
 * @param btn_pin Pin where button is wired
 * @param pressed_state Pin state when button is pressed. Value LOW or HIGH depends on wiring.
 */
PBtnToggleBase::PBtnToggleBase(int btn_pin, int pressed_state) {
    PBtnToggleBase::btn_ = btn_pin;
    PBtnToggleBase::state_ = B00000000;
    if (pressed_state==HIGH) {
        bitSet(PBtnToggleBase::state_, 5);
    }
    PBtnToggleBase::on_press_callback_ = NULL;
    PBtnToggleBase::on_long_press_callback_ = NULL;
    PBtnToggleBase::on_release_callback_ = NULL;
}

/**
 * Set callback function for on button press event
 *
 * @param callback Callback function pointer
 */
void PBtnToggleBase::onPress(ToggleFunc callback) {
    PBtnToggleBase::on_press_callback_ = callback;
}

/**
 * Set callback function for on long button press event
 *
 * @param callback Callback function pointer
 */
void PBtnToggleBase::onLongPress(LongPressFunc callback) {
    PBtnToggleBase::on_long_press_callback_ = callback;
}

/**
 * Set callback function for on button release event
 *
 * @param callback Callback function pointer
 */
void PBtnToggleBase::onRelease(ToggleFunc callback) {
    PBtnToggleBase::on_release_callback_ = callback;
}

void PBtnToggleBase::trigger_events_(bool btn_pressed) {
    if (btn_pressed) {
        if (!bitRead(PBtnToggleBase::state_, 1)) {
            if (!bitRead(PBtnToggleBase::state_, 2) && PBtnToggleBase::on_press_callback_) {
                PBtnToggleBase::on_press_callback_(PBtnToggleBase::btn_, bitRead(PBtnToggleBase::state_, 5)?HIGH:LOW);
            }
            if (!PBtnToggleBase::on_long_press_callback_) {
                bitClear(PBtnToggleBase::state_, 0);
            }

            bitSet(PBtnToggleBase::state_, 1);
            bitClear(PBtnToggleBase::state_, 2);
            bitClear(PBtnToggleBase::state_, 3);
            PBtnToggleBase::timer_ = millis();
        }
        if (PBtnToggleBase::on_long_press_callback_ && !bitRead(PBtnToggleBase::state_, 2) && PBtnToggleBase::timer_ + PBTNTOGGLEBASE_LONGCLICK_TIME < millis()) {
            bool noSkip = PBtnToggleBase::on_long_press_callback_(PBtnToggleBase::btn_, bitRead(PBtnToggleBase::state_, 5)?HIGH:LOW);
            if (noSkip) {
                bitClear(PBtnToggleBase::state_, 1);
                bitSet(PBtnToggleBase::state_, 2);
                bitClear(PBtnToggleBase::state_, 3);
            }
            bitClear(PBtnToggleBase::state_, 0);
        }
    } else {
        if (!bitRead(PBtnToggleBase::state_, 3) && !bitRead(PBtnToggleBase::state_, 2)) {
            if (PBtnToggleBase::on_release_callback_) {
                PBtnToggleBase::on_release_callback_(PBtnToggleBase::btn_, !bitRead(PBtnToggleBase::state_, 5)?HIGH:LOW);
            }
            bitClear(PBtnToggleBase::state_, 0);

            bitClear(PBtnToggleBase::state_, 1);
            bitClear(PBtnToggleBase::state_, 2);
            bitSet(PBtnToggleBase::state_, 3);
        }
    }
}

/**
 * Check button state and trigger events if needed
 */
void PBtnToggleBase::check() {
    if (bitRead(PBtnToggleBase::state_, 4)) {
        return;
    }
    bitSet(PBtnToggleBase::state_, 4);
    // get current button state
    bool btn_pressed = this->is_btn_pressed_();
    // has state changed during last check
    bool btn_state_changed = (bool) btn_pressed != (bool) bitRead(PBtnToggleBase::state_, 6);

    if (btn_pressed) {
        bitSet(PBtnToggleBase::state_, 6);
    } else {
        bitClear(PBtnToggleBase::state_, 6);
    }

    // trigger event if debounce time is passed
    if (!btn_state_changed && PBtnToggleBase::timer_ > 0 && PBtnToggleBase::timer_ + PBTNTOGGLEBASE_CLICK_DEBOUNCE_TIME < millis() && bitRead(PBtnToggleBase::state_, 0)) {
        PBtnToggleBase::trigger_events_(btn_pressed);
    }
    // reset timer if state has changed
    if (btn_state_changed) {
        PBtnToggleBase::timer_ = millis();
        bitSet(PBtnToggleBase::state_, 0);
    }
    bitClear(PBtnToggleBase::state_, 4);
}

/**
 * Get pin number
 */
int PBtnToggleBase::getPin() {
    return this->btn_;
}
