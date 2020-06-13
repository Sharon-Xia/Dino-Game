
#include "lcd_customcharacters.h"

// create the custom characters
void lcd_build(){

	// *** PLAYER
	unsigned char Player_Top[8] = { 0b00000,
		0b00000,
		0b00000,
		0b01110,
		0b00000,
		0b01010,
		0b10001,
		0b11111 };
	unsigned char Player_Bottom1[8] = { 0b10001,
		0b10001,
		0b10001,
		0b10001,
		0b01110,
		0b00011,
		0b11011,
		0b11000 };
	unsigned char Player_Bottom2[8] = { 0b10001,
		0b10001,
		0b10001,
		0b10001,
		0b01110,
		0b11000,
		0b11011,
		0b00011 };
	unsigned char Player_Squat[8] = {	0b01110,
		0b00000,
		0b01010,
		0b10001,
		0b11111,
		0b00000,
		0b11011,
		0b11011 };
	
	// **** CACTII
	unsigned char Cactus1[8] = {	0b00000,
		0b00000,
		0b00010,
		0b00010,
		0b00010,
		0b00011,
		0b00000,
		0b00000 };
	// full cactus
	unsigned char Cactus2[8] = {	0b00100,
		0b00101,
		0b10101,
		0b10101,
		0b10101,
		0b11111,
		0b00100,
		0b00100 };
	
	unsigned char Cactus3[8] = { 0b10000,
		0b10100,
		0b10100,
		0b10100,
		0b10100,
		0b11100,
		0b10000,
		0b10000 };
	
	unsigned char Cactus4[8] = {	0b10100,
		0b10110,
		0b10110,
		0b10110,
		0b10110,
		0b11111,
		0b10000,
		0b10000 };
		
		Custom_Char(0, Player_Top);
		Custom_Char(0, Player_Top); // 2nd time to prevent error idk why
		Custom_Char(1, Player_Bottom1);
		Custom_Char(2, Player_Bottom2);
		Custom_Char(3, Player_Squat);
		Custom_Char(4, Cactus1);
		Custom_Char(5, Cactus2);
		Custom_Char(6, Cactus3);
		Custom_Char(7, Cactus4);
}



void Custom_Char (unsigned char loc, unsigned char *c){
	unsigned char j;
	
	if (loc < 8)
	{
		lcd_command(0x40 + (loc*8));  /* Command 0x40 and onwards forces the device to point CGRAM address */
		for(j=0;j<8;j++)	/* Write 8 byte for generation of 1 character */
		lcd_char(c[j]);
	}
}


// pulled from https://www.electronicwings.com/avr-atmega/lcd-custom-character-display-using-atmega-16-32-

#define LCD_Data_Dir DDRD	/* Define LCD data port direction */
#define LCD_Command_Dir DDRB	/* Define LCD command port direction register */
#define LCD_Data_Port PORTD	/* Define LCD data port */
#define LCD_Command_Port PORTB	/* Define LCD data port */
#define RS PC0			/* Define Register Select signal pin */
#define RW PC1			/* Define Read/Write signal pin */
#define EN PC2			/* Define Enable signal pin */


void lcd_command(unsigned char cmnd)
{
	LCD_Data_Port= cmnd;
	LCD_Command_Port &= ~(1<<RS);	/* RS=0 command reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 Write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable pulse */
	avr_wait(1);
	LCD_Command_Port &= ~(1<<EN);
	avr_wait(2);
}


void lcd_char (unsigned char char_data)  /* LCD data write function */
{
	LCD_Data_Port= char_data;
	LCD_Command_Port |= (1<<RS);	/* RS=1 Data reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable Pulse */
	avr_wait(1);
	LCD_Command_Port &= ~(1<<EN);
	avr_wait(2);			/* Data write delay */
}



