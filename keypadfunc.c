#include "keypadfunc.h"

int is_pressed(int r, int c)
{
	//set PORTC to N/C
	DDRC = 0b00000000;
	PORTC = 0b00000000;
	// set 'r' pin to s0
	CLR_BIT(PORTC, r);
	SET_BIT(DDRC, r);
	// set 'c' pin to w1
	CLR_BIT(DDRC, c);
	SET_BIT(PORTC, c);
	
	avr_wait(1);
	// read pin @ 'c' *
	// if 0 return pressed
	// return not pressed
	if (GET_BIT(PINC, c) == 0)
	{
		return 1;
	}
	return 0;
}


// returns the key pressed
// 0 if no key pressed
// 1-16 if key pressed
int get_key(void)
{
	int r, c;
	for (r = 0; r < 4; ++r)
	{
		for (c = 0; c < 4; ++c)
		{
			if (is_pressed(r, c+4) == 1)
			{
				return (r * 4) + c + 1;
			}
		}
	}
	return 0;
}