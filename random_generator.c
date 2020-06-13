#include "random_generator.h"
#include "avr.h"


// get sample voltage from input A0 - A1
// value between 0 and 1023
int get_sample(){
	ADMUX = 0b01000000; 
	SET_BIT(ADCSRA, 7); // turns on ADC
	SET_BIT(ADCSRA, 6); // flag
	
	while (FINISH_FLAG); // busy
	return ADC; // contains sample value
}

// generates random value between x and y
// y must be larger than x
// x and y must not be more than 1024 apart
// preferably smaller value for better randomness
int random(int x, int y)
{
	int s = get_sample();
	return s % (y - x + 1)	+ x;
}