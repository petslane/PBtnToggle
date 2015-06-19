#ifndef PBTNTOGGLE_H
#define PBTNTOGGLE_H

/**
 * the debounce time in ms.
 */
#define PBTNTOGGLE_CLICK_DEBOUNCE_TIME 100
/**
 * long press time in ms
 */
#define PBTNTOGGLE_LONGCLICK_TIME 800

#include "Arduino.h"

/**
 * A type definition for press/release event callback function
 */
typedef void (*ToggleFunc)(int btn, int state);
/**
 * A type definition for long press event callback function
 */
typedef bool (*LongPressFunc)(int btn, int state);

class PBtnToggle {
    public:
        PBtnToggle(int btn_pin, int pressed_state=HIGH);
        void onPress(ToggleFunc callback);
        void onLongPress(LongPressFunc callback);
        void onRelease(ToggleFunc callback);
        void check();
        int getPin();
    private:
        int btn_;
        long timer_;
        byte state_;
        ToggleFunc on_press_callback_;
        LongPressFunc on_long_press_callback_;
        ToggleFunc on_release_callback_;
        void trigger_events_(bool btn_pressed);
        bool is_btn_pressed_();
};


#endif //PBTNTOGGLE_H
