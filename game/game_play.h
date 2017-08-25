/* ******************************
 * game_play.h
 * 
 * 
 * Author: Anna Branam (akbranam) + Ethan Lawton (elawton2)
 * Date Created: 04/21/2016
 * Last Modified by: Anna Branam (akbranam)
 * Date Last Modified: 
 * Assignment: 
 * Part of: C335 
 *
 ***********************/

#include <stm32f30x.h>

#define EXIT 2
#define WIN 1
#define LOSE -1
 
int GAMESTATE;
static int SELECT_X; 
static int SELECT_Y;
static int FLAGS;//number of tiels that have been flagged
static int REVEALED_TILES; //number of revealed tiles

int move_selection(int x, int y);
void selectTile(uint16_t color);
int toggle_flag(void);
int detonate_tile(void);
int chain_reveal(int x, int y, int start);
void start_game(void);

