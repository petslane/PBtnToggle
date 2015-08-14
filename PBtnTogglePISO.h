/** Single button connected to PISO chip.
 * This is button instance that is wire to PISO (parallel-in/serial out) shift register such as 74HC165.
 * Wire push button to PISO chip input pin with pull-up or pull-down resistor.
 */

#ifndef PBTNTOGGLE_PBTNTOGGLEPISO_H
#define PBTNTOGGLE_PBTNTOGGLEPISO_H

#include "Arduino.h"
#include "PBtnToggleBase.h"

class PBtnTogglePISO : public PBtnToggleBase {
    friend class PBtnTogglePISOController;
    public:
        PBtnTogglePISO(int chip, int btn_pin, int pressed_state=HIGH);
#ifdef ONLY_FOR_DOXYGEN
        void check();
#endif
    private:
        PBtnTogglePISO *next_piso_button_;
        void append_button(PBtnTogglePISO *btn);
        bool is_btn_pressed_();
//        byte state_;
        byte chip_states_;
#ifndef ONLY_FOR_DOXYGEN
        void check(byte *states);
#endif
};


#endif //PBTNTOGGLE_PBTNTOGGLEPISO_H
