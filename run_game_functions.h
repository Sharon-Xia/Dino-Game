#ifndef RUN_GAME_FUNCTIONS
#define RUN_GAME_FUNCTIONS

// functions necessary for running the game

#include <stdbool.h> // booleans
#include <avr/io.h>
#include <avr/eeprom.h> // for eeprom memory

#include "lcd_customcharacters.h"
#include "avr.h"
#include "lcd.h"
#include "keypadfunc.h"
#include "dino_game.h"

// ** USER INPUT **
// A for jump
// B for squat/duck
// 1 for start game
// * for stop game
// # for reset high score
// C for proceed to lobby screen

void set_high_score(int score);

int get_high_score();

void reset_high_score();


typedef struct game_task {
  //int state;                  // Task's current state
  unsigned long period;       // Task period
  unsigned long elapsedTime;  // Time elapsed since last task tick
  void (*TickFct)(DinoGame*);        // Task tick function
} game_task;


#define DEBOUNCE_DURATION 4

void debounce();

void debounce_tick();

bool debounce_ready();


// while user is in-game
// called every 50 ms
void game_input_tick(DinoGame* d);


// waits for C to go into lobby
// in lobby -> waits for 1 to start game, or # to reset high score
void pause_into_lobby();

void lobby(int highscore);


// running the tick methods
// exits function when the game is over
void run_game(DinoGame* d);

#endif