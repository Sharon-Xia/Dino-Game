
#ifndef LCD_CUSTOMCHAR
#define LCD_CUSTOMCHAR

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"



// create the custom characters
// call before you can use the custom characters
void lcd_build();


// creates individual char character
void Custom_Char (unsigned char loc, unsigned char *c);



// used in Custom_Char
void lcd_command(unsigned char cmnd);


// displays the custom char character
void lcd_char (unsigned char char_data);  /* LCD data write function */


#endif