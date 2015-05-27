# PBtnToggle
Arduino toggle button library

## Description
Simple toggle button library for handling toggle buttons, debounsing and adds button long press support.<br />
Button press, long press and release events can be registered, but are not mandatory. Registered callback method must have 2 parameters:<br />
`int btn` - toggle button pin number<br />
`int status` - toggle button state: `HIGH` or `LOW`<br />
Longpress function `LongPressFunc callback` must return boolean:<br />
  If returned `false`, releasing button will trigger `onRelease` callback.<br />
  If returned `true`, next button release and after that next button press will not trigger `onRelease` and `onPress` events.<br />
  
## Demo
https://www.youtube.com/watch?v=McYEGBtuxOA

## Public methods
```c++
PBtnToggle(int btn_pin, int pressed_state=HIGH);
```
`int btn_pin` - pin where toggle button is wired<br />
`int pressed_state=HIGH` - toggle button pin state when button is pressed down. Default state is HIGH if not set.
Creates new toggle button object. Sets pin mode to INPUT.<br />

```c++
void onPress(ToggleFunc callback);
```
`ToggleFunc callback` - callback function that will be called on button press

```c++
void onRelease(ToggleFunc callback);
```
`ToggleFunc callback` - callback function that will be called on button release

```c++
void onLongPress(LongPressFunc callback);
```
`LongPressFunc callback` - callback function that will be called on button long press

```c++
void check();
```
This method must be called from sketch loop. Monitors toggle button state and calls callback functions.

```c++
int getPin();
```
Gets toggle button pin number.

## Usage
Create toggle button instance in global scope:
```c++
PBtnToggle* btn = new PBtnToggle(7, HIGH);
```
Register some callback functions:
```c++
void setup() {
  btn->onPress(onButtonPress);
  btn->onLongPress(onButtonLongPress);
  btn->onRelease(onButtonRelease);
}
```
Check() method must be called in loop(). Additionally, check() van also be called by external interrupts:
```c++
void loop() {
  btn->check();
}
```
