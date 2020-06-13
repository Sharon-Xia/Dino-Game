// Sharon Xia
// CS 145
// shxia


#ifndef KEYPADFUNC_H
#define KEYPADFUNC_H

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"


// checks if keypad is pressed at position (r, c)
int is_pressed(int r, int c);


// returns the key pressed
// 0 if no key pressed
// 1-16 if key pressed
int get_key(void);


#endif