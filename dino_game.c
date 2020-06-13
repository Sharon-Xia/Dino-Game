#include "dino_game.h"

// **** Constructors ******
DinoGame _DinoGame(){
	Player p = {Player_run, 0};
	
	DinoGame d = {0, 0, p, {{0, 0, -1, 0}, {0, 0, -1, 0}, {0, 0, -1, 0}, {0, 0, -1, 0}}, 0, -1, -1, 0, 100, false};
	clear_obstacles(&d);
	create_obstacle(&d);
	return d;
}




// **** GAME METHODS ******

// creates obstacles when untilNextObstacle = 0
// advances time
// checks if player hits obstacle -> loses life or pauses/restarts game
// obstacles become more frequent as the game continues
// moves pterodactyl on evens
// moves/refreshes cactus every tick
// sets expired obstacles to time = -1
// ends game if returns false
void DinoGame_Tick(DinoGame* d){
	++d->time;
	--d->untilNextObstacle;
	
	if (d->score > d->adjusterLimit){ // inc obstacle adjuster
		d->adjusterLimit *= 1.5;
		++d->adjuster;
	}

	if (d->untilNextObstacle <= 0){ // countdown reaches 0
		d->untilNextObstacle = random(5, 19 - d->adjuster); // time until next
		create_obstacle(d);
	}

	if (d->time > 99){ d->time = 0;	} // prevent overflow
	d->score += 1;

	obstacle_tick(d);
	player_tick(d);

	// check gameover
	if (d->score > 9999){
		gameover(d);
		d->score = 9999; // only 4 digits for displaying score
	}
	if (is_collision(d)){
		gameover(d);
	}
	display_game(d);
}

// raises the flag to signal external that game is over
// the action of ending the game and going to lobby is handled elsewhere (main)
void gameover(DinoGame* d){
	d->gameover = true;
}



// ** SUB-TICKS **

// if time == 0, it's about to unexist
void obstacle_tick(DinoGame* d){
	// only check the obstacles that are valid
	for (int i = 0; i < d->numObstacles; ++i){ // existing obstacles
		int index = mod((d->obstacleIndex - i), max_obstacles);
		
		if (d->obstacles[index].time == 0 && d->obstacles[index].substate == 1){
			-- d->numObstacles; // unexistence
			d->obstacles[index].time = -2; // to be safe
			lcd_pos(0, 0);
			lcd_put(' ');
			lcd_pos(1, 0);
			lcd_put(' ');
		}

		if (d->obstacles[index].substate) // = 1
		{
			--d->obstacles[index].time; // tick down
		}
		// substate fluctuates between 1 and 0 every tick
		d->obstacles[index].substate = 1 - d->obstacles[index].substate;
	}
}

void player_tick(DinoGame* d){
	if (d->player.timeLeftInStatus){
		--d->player.timeLeftInStatus; // dec time left

		if (!d->player.timeLeftInStatus){ // if 0, run
			d->player.status = Player_run;
		}
	}
}

// sub-function for create_obstacle
void inc_numObstacles(DinoGame* d){
	++d->numObstacles;
	++d->obstacleIndex;
	if (d->obstacleIndex == max_obstacles){ // edge of queue
		d->obstacleIndex = 0;
	}
}


// creates new obstacle at the time given
// cactii : appear at score = 0
// upper pterodactyl: appear at score = 800
// lower pterodactyl: appear at score = 1200
// 	 updates array and indices accordingly
// decides time until next obstacle
void create_obstacle(DinoGame* d){
	if (d->numObstacles >= max_obstacles){
		return;
	}

	inc_numObstacles(d);

	int type; // obstacle type
	int req_player_status = Player_jump; // default, will be overwritten
	bool double_cactus = false;
	
	if (d->score < LEVEL_1){ // only cactii appear before 800
		type = CACTUS;
	}
	else if (d->score < LEVEL_2){ // double cactii
		type = random(0, 1);
		if (type == 1){ type = 3;}
	}
	else if (d->score < LEVEL_3){ // cactii + pterodactylUp
		type = random(0, 2);
		if (type == 2){ type = 3;}
	}
	else{ // any obstacle can appear
		type = random(0, 3); 
	}
	
	// check if can put in 2 cactii
	if (type == 3){
		if (d->numObstacles < max_obstacles){ // inc num obstacles later
			double_cactus = true;
		}
		type = CACTUS; // still type cactus
	}

	switch(type){
		case CACTUS:
			req_player_status = Player_jump;
			break;
		case PTERODACTYL_UP:
			req_player_status = Player_squat;
			break;
		case PTERODACTYL_DOWN:
			req_player_status = Player_jump;
			break;
	}
	// create obstacle object  = {type, req_player_status, 17, 0};
	d->obstacles[d->obstacleIndex].type = type;
	d->obstacles[d->obstacleIndex].required_player_status = req_player_status;
	d->obstacles[d->obstacleIndex].time = 17;
	d->obstacles[d->obstacleIndex].substate = 0;
	
	if (double_cactus){ // create second cactus
		inc_numObstacles(d);
		d->obstacles[d->obstacleIndex].type = type;
		d->obstacles[d->obstacleIndex].required_player_status = req_player_status;
		d->obstacles[d->obstacleIndex].time = 18;
		d->obstacles[d->obstacleIndex].substate = 0;
	}
}


void clear_obstacles(DinoGame* d){
	for (int i = 0; i < max_obstacles; ++i){
		d->obstacles[i].time = -3; // unexist
	}
	d->numObstacles = 0;
}


//     **** GAME STATE METHODS ****

// updates player state from user input
// can only change states when the player is in running state
// e.g. can't squat when in jumping mode and vice versa
// can double jump/squat, but cannot triple
// double squat/jump doesn't add onto time left in status; just resets it
// debouncing handled in run_game_functions game_input method
void update_state(DinoGame* d, int status){
	bool injump = d->player.status == Player_jump && d->player.timeLeftInStatus;
	bool insquat = d->player.status == Player_squat && d->player.timeLeftInStatus;

	static bool doublejumped; // don't allow triple jumps
	static bool doublesquat; // don't allow triple squats

	if (!injump){ // not already in jump
		doublejumped = false;
	}
	if (!insquat){ // not already in squat
		doublesquat = false;
	}


	switch(status){
		case Player_jump:
			if (d->player.status == Player_squat){ // cannot jump from squat
				return;
			}
			else if (injump && !doublejumped){ // double jump
				d->player.status = status;
				d->player.timeLeftInStatus = STATUS_DURATION;
				doublejumped = true;
			}
			else if (!injump){ // not already jumping
				d->player.status = status;
				d->player.timeLeftInStatus = STATUS_DURATION;
			}
			break;
		case Player_squat:
			if (d->player.status == Player_jump){ // can't squat from jump
				return;
			}
			else if (insquat && !doublesquat){ // not already squatting
				d->player.status = status;
				d->player.timeLeftInStatus = STATUS_DURATION;
				doublesquat = true;
			}
			else if (!insquat){
				d->player.status = status;
				d->player.timeLeftInStatus = STATUS_DURATION;
			}
			break;
	}
}


// checks for collisions with obstacles
// if player dodges obstacle, inc the score by more points
bool is_collision(DinoGame* d)
{
	// only check the obstacles that are valid
	for (int i = 0; i < d->numObstacles; ++i){ // existing obstacles
		int index = mod((d->obstacleIndex - i), max_obstacles);
		if (d->obstacles[index].time == Player_POS)
		{// obstacle encountered
			if (d->obstacles[index].required_player_status == d->player.status){// dodges
				d->score += 10;
				return false;
			}
			else{ // obstacle not dodged
				return true;
			}
			// can only encounter one obstacle at a time
		}
	}
	return false;
}



//     **** GAME DISPLAY METHODS ****
// write display methods in a way that uses ' ' to clear
// certain positions instead of lcd_clr()

// displays entire game on lcd
void display_game(DinoGame* d){
	display_player(d);
	display_obstacles(d);
	display_stats(d);
}

void display_player(DinoGame* d){
	int player_status = d->player.status;
	switch(player_status){
		case Player_run:
			lcd_pos(0, Player_POS);
			lcd_char(PLAYER_TOP);
			lcd_pos(1, Player_POS);
			// which bottom to use for run
			if (d->time % 2 == 0){
				lcd_char(PLAYER_BOTTOM1);
			}
			else{
				lcd_char(PLAYER_BOTTOM2);
			}
			break;
		case Player_jump:
			lcd_pos(0, Player_POS);
			lcd_char(PLAYER_SQUAT);
			lcd_pos(1, Player_POS);
			lcd_put(' ');
			break;
		case Player_squat:
			lcd_pos(0, Player_POS);
			lcd_put(' ');
			lcd_pos(1, Player_POS);
			lcd_char(PLAYER_SQUAT);
			break;
	}
	
};

// there should only be adjacent obstacles in the case of a double cactus
void display_obstacles(DinoGame* d){
	// only check for obstacles that exist/are on the screen
	// visible between time = 0 and 15
	// displays from right to left
	for (int i = 0; i < d->numObstacles; ++i){ // existing obstacles i from right to left
		int index = mod((d->obstacleIndex - i), max_obstacles);
		int time = d->obstacles[index].time;
		
		// check for double cactus (checking for the left cactus because later animation)
		// confirm left cactus by checking if there's a cactus right next to it
		// only affects animation when they're in substate 0 (half cactii)
		bool double_cactus = false;
		if (i > 0 && d->obstacles[index].type == CACTUS){// obsctacle is cactus && there are obstacles with greater time (to the right)
			int index2 = mod((d->obstacleIndex - i + 1), max_obstacles); // index of obstacle to the right
			// if obstacle to the right is also cactus and times are adjacent, there is double cactus
			double_cactus = d->obstacles[index2].type == CACTUS && d->obstacles[index].time + 1 == d->obstacles[index2].time;
		}
		

		if (time >= 0 && time < 16){ // visible obstacles
			switch(d->obstacles[index].type){
				case CACTUS:
					if (d->obstacles[index].substate == 1){ // 1 full cactus
						lcd_pos(1, time);
						lcd_char(CACTUS2);
						if (time != 15 && !double_cactus){ // off the screen
							lcd_put(' ');
						}
					}
					else{ // 2 half cactii (happens before 1 full cactus)
						lcd_pos(1, time);
						lcd_char(CACTUS1);
						if (time != 15){ // if not off the screen
							if (double_cactus){
								lcd_char(CACTUS4);
							}
							else{
								lcd_char(CACTUS3);
							}
						}
					}
					break;
				case PTERODACTYL_UP:
					lcd_pos(0, time);
					lcd_put(PTERODACTYL);
					if (time != 15 && time != Player_POS - 1){ // off the screen & don't overwrite player
							lcd_put(' ');
						}
					break;
				case PTERODACTYL_DOWN:
					lcd_pos(1, time);
					lcd_put(PTERODACTYL); 
					if (time != 15 && time != Player_POS - 1){ // off the screen
							lcd_put(' ');
						}
					break;
			}
		}
	}
}

// # of lives???
// score in 4 digits
// %04d
void display_stats(DinoGame* d){
	char buf[4];
	sprintf(buf, "%04d", d->score);
	lcd_pos(0, 0);
	lcd_puts(buf);
}


// what to display when the game isn't running
// takes in high score
// blueprint:
// "High #: ###### 0"
// "Press 1 to start"
void display_lobby(int highscore){
	lcd_clr();

	char buf[17];
	sprintf(buf, "High #: %04d ", highscore);
	lcd_pos(0, 0);
	lcd_puts(buf);
	lcd_char(PLAYER_SQUAT);

	sprintf(buf, "Press 1 to start");
	lcd_pos(1, 0);
	lcd_puts(buf);
};


// Other necessary methods:
// modulus, since % = remainder
// returns x mod y
int mod(int x, int y){
	if (x < 0){ // neg x
		return x % y + y;
	}
	else{ // pos x
		return x % y;
	}
}