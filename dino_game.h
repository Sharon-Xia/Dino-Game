// Sharon Xia - CS 145 - Final Project
// structure that defines the game DinoGame -> holds all the game information
// includes:
//		DinoGame_Tick(DinoGame* d) -> the gear that runs the entire game
//		display methods -> displays game, lobby; each individual game component has its own display method


#ifndef DINO_GAME
#define DINO_GAME

#include <stdbool.h> // for booleans
#include <stdio.h> // for sprintf
#include "lcd_customcharacters.h"
#include "avr.h"
#include "lcd.h"
#include "random_generator.h" // random(int x, int y);


// custom chars
#define PLAYER_TOP 0
#define PLAYER_BOTTOM1 1
#define PLAYER_BOTTOM2 2
#define PLAYER_SQUAT 3
#define CACTUS1 4
#define CACTUS2 5
#define CACTUS3 6
#define CACTUS4 7
#define PTERODACTYL 'E' // tbh, it's a sideways cactus (changed to E)

// player statuses
#define Player_run 0
#define Player_squat 1
#define Player_jump 2

// obstacle types
#define CACTUS 0
#define PTERODACTYL_UP 1
#define PTERODACTYL_DOWN 2


// other settings
#define Player_POS 5 // 6th column
#define max_obstacles 8
#define STATUS_DURATION 5

// when obstacles appear
#define LEVEL_0 0 // needs to be 0 // cactii
#define LEVEL_1 100 // double cactii
#define LEVEL_2 400
#define LEVEL_3 800

// **** PLAYER OBJECT *********
// displayed in column 6 of lcd
typedef struct Player{
		int status;
		int timeLeftInStatus; // 0 for running
		// decide which running animation by even/odd time
	} Player;
	
// **** OBSTACLE OBJECT ******
// How time affects obstacle appearance:
// appears on screen at time = 16
// disappears from screen at time = 0
// cactus is updated at 2x the rate of pterodactyl
// hits player at time == 6 // 6th column
// substate: 0 = 2 half cactii, 1 = 1 full cactus
// substate for ptero: moves every 2 ticks
typedef struct Obstacle{
		int type;
		int required_player_status; // status needed to avoid death
		int time;
		int substate; // used for cactus with multiple frames
	} Obstacle;


// **** DINOGAME OBJECT *******
typedef struct DinoGame {
		int time; // just for even/odd; refreshes at 99
		int score; // max score of 9999
		Player player;
		Obstacle obstacles[max_obstacles]; // used as a queue of obstacles
		int numObstacles; // keep track of number of obstacles
		int obstacleIndex; // queue index; kept on last obstacle object
		int untilNextObstacle; // countdown until next obstacle appears
		int adjuster; // increases difficulty of game as it continues
		int adjusterLimit; // score at which adjuster increases
		bool gameover; // for emergency turn-off game
	} DinoGame;


// Constructor for DinoGame 'class'
DinoGame _DinoGame();

void gameover(DinoGame* d);


// **** GAME METHODS ******

// creates obstacles when untilNextObstacle = 0
// advances time
// checks if player hits obstacle -> loses life or pauses/restarts game
// end game if return false
void DinoGame_Tick(DinoGame* d);

// ** SUB-TICKS **
void obstacle_tick(DinoGame* d);

void player_tick(DinoGame* d);



// creates new obstacle, updates array and indices accordingly
// decides time until next obstacle
void create_obstacle(DinoGame* d);

void clear_obstacles(DinoGame* d);



//     **** GAME STATE METHODS ****

// updates player state from user input
// can only change states when the player is in running state
// e.g. can't squat when in jumping mode
void update_state(DinoGame* d, int state);


// checks for collisions with obstacles
bool is_collision(DinoGame* d);




//     **** GAME DISPLAY METHODS ****
// write display methods in a way that uses ' ' to clear
// certain positions instead of lcd_clr()

// displays entire game on lcd
void display_game(DinoGame* d);

void display_player(DinoGame* d);

void display_obstacles(DinoGame* d);

// # of lives???
// score in 4 digits
// %04d
void display_stats(DinoGame* d);


// what to display when the game isn't running
// takes in high score
// "High #: ###### 0"
// "Press 1 to start"
void display_lobby(int highscore);


// Other necessary methods:
// modulus, since % = remainder
int mod(int x, int y);

#endif
