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
    state_pressed_on_high_(pressed_state==HIGH);
    PBtnToggleBase::on_press_callback_ = NULL;
    PBtnToggleBase::on_long_press_callback_ = NULL;
    PBtnToggleBase::on_release_callback_ = NULL;
    PBtnToggleBase::timer_ = millis();
}

/**
 * Set optional callback function to be called on button press event
 *
 * @param callback Callback function pointer
 */
void PBtnToggleBase::onPress(ToggleFunc callback) {
    PBtnToggleBase::on_press_callback_ = callback;
}

/**
 * Set optional callback function to be called on button long press event
 *
 * @param callback Callback function pointer
 */
void PBtnToggleBase::onLongPress(LongPressFunc callback) {
    PBtnToggleBase::on_long_press_callback_ = callback;
}

/**
 * Set optional callback function to be called on button release event
 *
 * @param callback Callback function pointer
 */
void PBtnToggleBase::onRelease(ToggleFunc callback) {
    PBtnToggleBase::on_release_callback_ = callback;
}

void PBtnToggleBase::trigger_events_(bool btn_pressed) {
    if (btn_pressed) {
        // button pressed, trigger press event if not triggered already
        if (!state_press_triggered_()) {
            // if last event was long press, then do not trigger this press event
            if (!state_longpress_triggered_() && PBtnToggleBase::on_press_callback_) {
                PBtnToggleBase::on_press_callback_(PBtnToggleBase::btn_, state_pressed_on_high_()?HIGH:LOW);
            }
            // if no long press event callback set, then stop press timer
            if (!PBtnToggleBase::on_long_press_callback_) {
                state_press_timer_started_(false);
            }

            state_press_triggered_(true);
            state_longpress_triggered_(false);
            state_release_triggered_(false);
            PBtnToggleBase::timer_ = millis();
        }
        // longpress event callback set but not triggered, button press period reached long press state
        if (PBtnToggleBase::on_long_press_callback_ && !state_longpress_triggered_() && PBtnToggleBase::timer_ + PBTNTOGGLEBASE_LONGCLICK_TIME < millis()) {
            bool noSkip = PBtnToggleBase::on_long_press_callback_(PBtnToggleBase::btn_, state_pressed_on_high_()?HIGH:LOW);
            // if long press callback returns true, then set longpress event status as triggered, this will make button release not to trigger release event
            if (noSkip) {
                state_press_triggered_(false);
                state_longpress_triggered_(true);
                state_release_triggered_(false);
            }
            state_press_timer_started_(false);
        }
    } else {
        // if no press or longpress triggered before, then stop timer
        if (!state_press_triggered_() && !state_longpress_triggered_()) {
            state_press_timer_started_(false);
            return;
        }
        // trigger release
        if (!state_release_triggered_() && !state_longpress_triggered_() && state_press_triggered_()) {
            if (PBtnToggleBase::on_release_callback_) {
                PBtnToggleBase::on_release_callback_(PBtnToggleBase::btn_, !state_pressed_on_high_()?HIGH:LOW);
            }
            state_press_timer_started_(false);

            state_press_triggered_(false);
            state_longpress_triggered_(false);
            state_release_triggered_(true);
        }
    }
}

/**
 * Check button state and trigger event callback functions. This method must be called in loop()
 */
void PBtnToggleBase::check() {
    if (state_is_running_()) {
        return;
    }
    state_is_running_(true);

    // get current button state
    bool btn_pressed = this->is_btn_pressed_();

    // has state changed during last check
    bool btn_state_changed = btn_pressed != state_was_button_pressed_();
    state_was_button_pressed_(btn_pressed);

    // trigger event if debounce time is passed
    if (!btn_state_changed && PBtnToggleBase::timer_ + PBTNTOGGLEBASE_CLICK_DEBOUNCE_TIME < millis() && state_press_timer_started_()) {
        PBtnToggleBase::trigger_events_(btn_pressed);
    }

    // reset timer if state has changed
    if (btn_state_changed) {
        PBtnToggleBase::timer_ = millis();
        state_press_timer_started_(true);
    }
    state_is_running_(false);
}

/**
 * Get pin number
 */
int PBtnToggleBase::getPin() {
    return this->btn_;
}




/**
 * Check if initial
 */
bool PBtnToggleBase::state_press_timer_started_() {
    return bitRead(PBtnToggleBase::state_, 0) == 1;
}
/**
 * Set current running state
 */
void PBtnToggleBase::state_press_timer_started_(bool set) {
    state_set_state_(set, 0);
}

/**
 * Check if press event triggered
 */
bool PBtnToggleBase::state_press_triggered_() {
    return bitRead(PBtnToggleBase::state_, 1) == 1;
}
/**
 * Set press event trigger status
 */
void PBtnToggleBase::state_press_triggered_(bool set) {
    state_set_state_(set, 1);
}
/**
 * Check if long press event triggered
 */
bool PBtnToggleBase::state_longpress_triggered_() {
    return bitRead(PBtnToggleBase::state_, 2) == 1;
}
/**
 * Set long press event trigger status
 */
void PBtnToggleBase::state_longpress_triggered_(bool set) {
    state_set_state_(set, 2);
}
/**
 * Check if release event triggered
 */
bool PBtnToggleBase::state_release_triggered_() {
    return bitRead(PBtnToggleBase::state_, 3) == 1;
}
/**
 * Set release event trigger status
 */
void PBtnToggleBase::state_release_triggered_(bool set) {
    state_set_state_(set, 3);
}

/**
 * Check if button check() is currently running
 */
bool PBtnToggleBase::state_is_running_() {
    return bitRead(PBtnToggleBase::state_, 4) == 1;
}
/**
 * Set current running state
 */
void PBtnToggleBase::state_is_running_(bool set) {
    state_set_state_(set, 4);
}

/**
 * Get pin state on button press
 */
bool PBtnToggleBase::state_pressed_on_high_() {
    return bitRead(PBtnToggleBase::state_, 5) == 1;
}
/**
 * Set pin state in button press
 */
void PBtnToggleBase::state_pressed_on_high_(bool set) {
    state_set_state_(set, 5);
}

/**
 * Get button pressed status on last check
 */
bool PBtnToggleBase::state_was_button_pressed_() {
    return bitRead(PBtnToggleBase::state_, 6) == 1;
}
/**
 * Set button pressed status for next check
 * Used to detect if button pressed status is changed
 */
void PBtnToggleBase::state_was_button_pressed_(bool set) {
    state_set_state_(set, 6);
}

/**
 * Common method for setting state bit value
 */
void PBtnToggleBase::state_set_state_(bool set, int bit) {
    if (set) {
        bitSet(PBtnToggleBase::state_, bit);
    } else {
        bitClear(PBtnToggleBase::state_, bit);
    }
}
