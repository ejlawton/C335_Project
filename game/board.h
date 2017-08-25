/* ******************************
 * board.h
 * 
 * 
 * Author: Ethan Lawton (elawton2) + Anna Branam (akbranam)
 * Date Created: 02/05/2016
 * Last Modified by: Ethan Lawton (elawton2)
 * Date Last Modified: 
 * Assignment: 
 * Part of: C335 
 *
 ***********************/

#include <stm32f30x.h>

#define BOARD_SIZE 8

typedef struct tile {
  int mine_flag;
  int count;
  int flag;
  int revealed;
  int selected;
  int x_pos;
  int y_pos;
}tile_t;

typedef struct board {
  int mine_count;
  tile_t tiles[BOARD_SIZE][BOARD_SIZE];
}board_t;

board_t BOARD; 

void board_init(void);
void boardGen(void);
int in_bounds(int x, int y);
void bomb_generator(void);
unsigned newrand(void);
