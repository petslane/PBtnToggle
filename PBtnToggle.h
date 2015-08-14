#ifndef PETSLANE_PBTNTOGGLE_PBTNTOGGLE_H
#define PETSLANE_PBTNTOGGLE_PBTNTOGGLE_H

#include "Arduino.h"
#include "PBtnToggleBase.h"

/** Single button connected directly to pin.
 * This is the simplest way to handle buttons. Wire push button to any digital pin with pull-up or pull-down resistor.
 */
class PBtnToggle : public PBtnToggleBase {
    public:
        PBtnToggle(int btn_pin, int pressed_state=HIGH);
    private:
        bool is_btn_pressed_();
};


#endif //PETSLANE_PBTNTOGGLE_PBTNTOGGLE_H
