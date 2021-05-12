#ifndef KEYBOARD_H
#define KEYBOARD_H

extern void initKeyboard();
unsigned long keyboardOSReadKey();
byte keyboardGetLockState();

#endif
