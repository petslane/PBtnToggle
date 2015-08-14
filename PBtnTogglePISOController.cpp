#include "PBtnTogglePISOController.h"

/**
 * Initialize PISO chip controller.
 *
 * @param chips Number of chips daisy chained
 * @param data_pin pin number wired to chip Q7 (serial out)
 * @param clock_pin pin number wired to chip CP (clock input)
 * @param latch_pin pin number wired to *PL* /active low/ (parallel load input)
 */
PBtnTogglePISOController::PBtnTogglePISOController(int chips, int data_pin, int clock_pin, int latch_pin) {
    chips_ = chips;
    data_pin_ = data_pin;
    clock_pin_ = clock_pin;
    latch_pin_ = latch_pin;
    button_ = 0;

    pinMode(data_pin, INPUT);
    pinMode(clock_pin, OUTPUT);
    pinMode(latch_pin, OUTPUT);
}

/**
 * Add PBtnTogglePISO button to controller.
 *
 * @param *btn PBtnTogglePISO pointer
 */
void PBtnTogglePISOController::add(PBtnTogglePISO *btn) {
    // next slot empty, add it
    if (!button_) {
        button_ = btn;
        return;
    }
    // next button nr is lower than *btn button nr, append after next button
    if (button_->btn_ < btn->btn_) {
        button_->append_button(btn);
        return;
    }
    // switch next and *btn
    btn->append_button(button_);
    button_ = btn;
}

/**
 * Check buttons state. This method must be called in loop()
 */
void PBtnTogglePISOController::check() {
    // Load all Dn values into register
    digitalWrite (latch_pin_, LOW);
    delayMicroseconds(10);
    digitalWrite (latch_pin_, HIGH);

    // read all chip values
    byte states[chips_];
    for (int i = 0; i < chips_; i++) {
        states[i] = shiftIn165();
    }

    // check buttons states
    if (button_) {
        button_->check(states);
    }
}

/*
 * Here we define an alternate shiftIn function. The only difference
 * between this shift in function and the built-in shiftIn function
 * is that the clock must got LOW before shifting and HIGH after
 * shifting.  This is the opposite of the built in Arduino shiftIn.
 */
byte PBtnTogglePISOController::shiftIn165() {
    byte value = 0;
    for (int i = 0; i < 8; ++i) {
        digitalWrite(clock_pin_, LOW);
        value |= digitalRead(data_pin_) << (7 - i);
        digitalWrite(clock_pin_, HIGH);
    }
    return value;
}