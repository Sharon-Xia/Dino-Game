/*
Uses analog voltage sampled from A0-A1 to generate random values
* Leave the probes out in the air for inconsistent sampling
* Creates an element of randomness for other use
* Idea from Piazza CS 145
* Only supports randomness up to 2^10
	* Less randomness, because air voltage usually around 2-4v

Created for the purpose of DinoGame
Code Snippets from voltage.h in Project 4

Sharon Xia
*/



#ifndef RANDOM_GEN
#define RANDOM_GEN

#include <avr/io.h>

// 'busy'
#define FINISH_FLAG GET_BIT(ADCSRA, 6)

// get sample voltage from input A0 - A1
// value between 0 and 1023
int get_sample();


// generates random value between x and y
// x and y must not be more than 1024 apart
// preferably smaller value for better randomness
int random(int x, int y);

#endif