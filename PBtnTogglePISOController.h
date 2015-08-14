/** Controller class for PISO buttons.
 * All PBtnTogglePISO buttons will be add()-ed to this controller.
 * PISO chips can be daisy chained. *PL* /active low/, CP, Q7 pins of first chip are wired to Arduino digital pins. All
 * chips have common *PL* /active low/, CP, Vcc, *CE* /active low/, GND pins. Pin DS will be wired with pin Q7 of next
 * chip.
 */

#ifndef PBTNTOGGLE_PBTNTOGGLEPISOCONTROLLER_H
#define PBTNTOGGLE_PBTNTOGGLEPISOCONTROLLER_H

#include "Arduino.h"
#include "PBtnTogglePISO.h"

class PBtnTogglePISOController {
public:
    PBtnTogglePISOController(int chips, int data_pin, int clock_pin, int latch_pin_);
    void add(PBtnTogglePISO *btn);
    void check();
private:
    int chips_;
    int data_pin_;
    int clock_pin_;
    int latch_pin_;
    int bit_order_;
    PBtnTogglePISO *button_;
    byte shiftIn165();
};


#endif //PBTNTOGGLE_PBTNTOGGLEPISOCONTROLLER_H
