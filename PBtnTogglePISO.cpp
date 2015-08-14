#include "PBtnTogglePISO.h"

/**
 * Initialize button handler by providing PISO chip data input (Dn) number, chip number and pin state on button press.
 * Data pin number value from 0 to 7. Referred as Dn in PISO data sheet. PISO chips can be daisy chained. First chip
 * (with value 0) is one with Q7 (serial output from the last stage - pin 9) wired to Arduino.
 *
 * @param chip Chip number starting from 0.
 * @param btn_pin Dn number in PISO data sheet. Value from 0 to 7.
 * @param pressed_state Dn pin state on button press. Value: HIGH or LOW
 */
PBtnTogglePISO::PBtnTogglePISO(int chip, int btn_pin, int pressed_state): PBtnToggleBase(btn_pin, pressed_state) {
    PBtnTogglePISO::next_piso_button_ = 0;
    PBtnTogglePISO::btn_ = chip * 8 + btn_pin;
}

/**
 * Read button state and return true if button is pressed down.
 */
bool PBtnTogglePISO::is_btn_pressed_() {
    int btn = btn_ % 8;
    return (bitRead(chip_states_, btn) == 1) == (bitRead(state_, 5) == 1);
}


void PBtnTogglePISO::append_button(PBtnTogglePISO *btn) {
    // next slot empty, add it
    if (!next_piso_button_) {
        next_piso_button_ = btn;
        return;
    }
    // next button nr is lower than *btn button nr, append after next button
    if (next_piso_button_->btn_ < btn->btn_) {
        next_piso_button_->append_button(btn);
        return;
    }
    // switch next and *btn
    btn->append_button(next_piso_button_);
    next_piso_button_ = btn;
}

#ifndef ONLY_FOR_DOXYGEN
void PBtnTogglePISO::check(byte *states) {
    int chip = floor(btn_ / 8);
    chip_states_ = states[chip];

    PBtnToggleBase::check();
    if (next_piso_button_) {
        next_piso_button_->check(states);
    }
}
#else
/**
 * Not used. PISO buttons must be added to PISO controller and use PBtnTogglePISOController::check() in loop.
 */
void PBtnTogglePISO::check() {}
#endif
