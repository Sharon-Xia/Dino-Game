#include "run_game_functions.h"

// ** USER INPUT **
// A for jump
// B for squat/duck
// 1 for start game
// * for stop game
// # for reset high score
// C for proceed to lobby screen



// ***** HIGH SCORE - eeprom ******
void set_high_score(int score){
	eeprom_update_word((uint16_t*)1, score);
}


int get_high_score(){
	return eeprom_read_word((uint16_t*)1); // 1 is address, store like 4bytes over for another 4byte word
}


void reset_high_score(){
	eeprom_update_word((uint16_t*)1, 0);
}


// ***** DEBOUNCING ****
static int DEBOUNCE = 0;

void debounce(){
	DEBOUNCE = DEBOUNCE_DURATION;
}

void debounce_tick(){
	if (DEBOUNCE > 0){
		--DEBOUNCE;
	}
}

bool debounce_ready(){
	return DEBOUNCE == 0;
}


// while user is in-game
// called every 50 ms
void game_input_tick(DinoGame* d){
	if (!debounce_ready()){
		return;
	}

	int k = get_key(); // in keyboard func

	switch(k){
		case 4: // A - jump
			update_state(d, Player_jump);
			display_player(d);
			debounce();
			break;
		case 8: // B - squat
			update_state(d, Player_squat);
			display_player(d);
			debounce();
			break;
		case 13: // * stop game
			gameover(d);
			debounce();
			// pause & display lobby
			break;
	}
}



void pause_into_lobby(){
	// end-game
	// wait for 'C'
	int k = -1;
	while (k != 12){
		if (debounce_ready()){
			k = get_key();
			debounce();
		}
		avr_wait(50);
		debounce_tick();
	}
}


void lobby(int highscore){
	// in lobby
	display_lobby(highscore);
	int k = -1;
	while (k != 1){
		if (debounce_ready()){
			k = get_key();

			switch(k){
				case 1:
					debounce();
					return;
					break;
				case 15:
					debounce();
					reset_high_score();
					display_lobby(get_high_score());
					break;
			}
		}

		debounce_tick();
		avr_wait(50);
	};
	// start the game
}


// running the tick methods
// exits function when the game is over
void run_game(DinoGame* d){
	static const int PERIOD = 50;
	static const int DINO_PERIOD = 100;
	static const int INPUT_PERIOD = 50;

	game_task tasks[2] = {{DINO_PERIOD, DINO_PERIOD, DinoGame_Tick}, {INPUT_PERIOD, INPUT_PERIOD, game_input_tick}};
		
	lcd_clr();

	while (!d->gameover){//(!d->gameover){

		for (int i = 0; i < 2; ++i){
			tasks[i].elapsedTime += PERIOD;
			if (tasks[i].elapsedTime >= tasks[i].period){
				tasks[i].elapsedTime = 0;
				tasks[i].TickFct(d);
			}
		}
		debounce_tick();
		avr_wait(PERIOD);
	}
	clear_obstacles(d);
};


/*				sketchier way of doing it
				// use if functions are incompatible with each other
				switch(i){
					case 0: // DINO TASK
						DinoGame_Tick(d);
						break;
					case 1: // INPUT TASK
						game_input_tick(d);
						break;
				}
				*/
