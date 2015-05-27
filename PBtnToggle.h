#ifndef PBTNTOGGLE_H
#define PBTNTOGGLE_H

// the debounce time in ms.
#define PBTNTOGGLE_CLICK_DEBOUNCE_TIME 100
// long press time in ms
#define PBTNTOGGLE_LONGCLICK_TIME 800

#include "Arduino.h"

typedef void (*ToggleFunc)(int btn, int state);
typedef bool (*LongPressFunc)(int btn, int state);

class PBtnToggle {
    public:
        PBtnToggle(int btn_pin, int pressed_state=HIGH);
        void onPress(ToggleFunc callback);
        void onLongPress(LongPressFunc callback);
        void onRelease(ToggleFunc callback);
        void checkChange();
        void check();
        int getPin();
    private:
        int btn;
        long timer;
        byte state;
        ToggleFunc onPressCallback;
        LongPressFunc onLongPressCallback;
        ToggleFunc onReleaseCallback;
        void triggerEvents(bool btn_pressed);
};


#endif //PBTNTOGGLE_H
