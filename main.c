/*
 * CS145_Project5.c
 * Replication of the Dino Game that chrome displays when the wifi dies
 *
 * Created: 6/10/2020 3:30:45 AM
 * Author : Sharon Xia
 */ 

#include <stdbool.h>
#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include "keypadfunc.h"
#include "dino_game.h"
#include "run_game_functions.h"



// **** INCLUDE IN THE MAIN FUNCTION *****
// * create the game
// * uhh figure out the timer
// * user input in the timer
// * keep track of the high score -> put in EEPROM??
// * updates high score from DinoGame when gameover

// ** USER INPUT **
// A for jump
// B for squat/duck
// 1 for start game
// * for stop game
// # for reset high score
// C for proceed to lobby screen




int main(void)
{
    lcd_init();
	lcd_build();
	
	lcd_clr();
	
	int highscore = get_high_score();
	
	lobby(highscore);

	while (1){
		DinoGame game = _DinoGame();
		//update_game(&game);
		display_game(&game);

		run_game(&game); // returns score
		
		highscore = get_high_score();
		
		if (game.score > highscore){
			highscore = game.score;
			set_high_score(highscore);
		}
		
		pause_into_lobby();
		lobby(highscore);
	}
}

