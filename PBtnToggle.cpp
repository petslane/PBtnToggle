#include "PBtnToggle.h"

PBtnToggle::PBtnToggle(int btn_pin, int pressed_state): PBtnToggleBase(btn_pin, pressed_state) {
    pinMode(PBtnToggle::btn_, INPUT);
}

/**
 * Read button state and return true if button is pressed down.
 */
bool PBtnToggle::is_btn_pressed_() {
    return (digitalRead(PBtnToggle::btn_) == HIGH) == state_pressed_on_high_();
}
