# PBtnToggle
Arduino library for handling push buttons. Supports push buttons wired to Arduino digital pib or thru PISO (parallel-in/serial out) chip like 74HC165.

## Description
Arduino library for handling push buttons. Supports push buttons wired to Arduino digital pin or thru PISO (parallel-in/serial out) chip like 74HC165 with support to daisy chain multiple chips. Automatic handling of debounsing. Buttons have push event, long push event and release event.

## Documentation
https://htmlpreview.github.io/?https://raw.githubusercontent.com/petslane/PBtnToggle/master/doc/html/annotated.html

## Demo
https://www.youtube.com/watch?v=McYEGBtuxOA

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
