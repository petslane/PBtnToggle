#ifndef PBTNTOGGLEBASE_H
#define PBTNTOGGLEBASE_H

/**
 * the debounce time in ms.
 */
#define PBTNTOGGLEBASE_CLICK_DEBOUNCE_TIME 100
/**
 * long press time in ms
 */
#define PBTNTOGGLEBASE_LONGCLICK_TIME 800

#include "Arduino.h"

/**
 * A type definition for press/release event callback function
 */
typedef void (*ToggleFunc)(int btn, int state);
/**
 * A type definition for long press event callback function
 */
typedef bool (*LongPressFunc)(int btn, int state);

class PBtnToggleBase {
    friend class PBtnToggle;
    public:
        PBtnToggleBase(int btn_pin, int pressed_state=HIGH);
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
        virtual bool is_btn_pressed_() = 0;
        void state_set_state_(bool set, int bit);
        bool state_is_running_();
        void state_is_running_(bool set);
        bool state_was_button_pressed_();
        void state_was_button_pressed_(bool set);
        bool state_press_timer_started_();
        void state_press_timer_started_(bool set);
        bool state_press_triggered_();
        void state_press_triggered_(bool set);
        bool state_longpress_triggered_();
        void state_longpress_triggered_(bool set);
        bool state_release_triggered_();
        void state_release_triggered_(bool set);
        bool state_pressed_on_high_();
        void state_pressed_on_high_(bool set);
};


#endif //PBTNTOGGLEBASE_H
