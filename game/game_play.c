/* ******************************
 * game_play.c
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
#include <f3d_user_btn.h>
#include "game_play.h"
#include <f3d_lcd_sd.h>
#include "board.h"
#include <stdio.h>
#include <glcdfont.h>
#include <f3d_nunchuk.h>



void selectTile(uint16_t color){
  tile_t temp = BOARD.tiles[SELECT_X][SELECT_Y];
  int x_temp = temp.x_pos +6;
  int y_temp = temp.y_pos +8;
  int a, b;
  for(a = 0; a < 4; a++){
    for(b = 0; b < 4; b++){
      f3d_lcd_drawPixel(x_temp+a, y_temp+b, color);
    }
  }
} 


//moves the selected tile to a new tile if in bounds
int move_selection(int x, int y){
  int new_x = SELECT_X+x;
  int new_y = SELECT_Y+y;
  if (in_bounds (new_x, new_y)){
    selectTile(WHITE);
    display_tile(SELECT_X, SELECT_Y);//draw tile
    SELECT_X = new_x;
    SELECT_Y = new_y;
    selectTile(BLACK);
   
    return 1;
  }
  return 0;
}


int toggle_flag(void){
  tile_t temp = BOARD.tiles[SELECT_X][SELECT_Y];
  if (temp.revealed) return 0;
  if (temp.flag) {temp.flag = 0; FLAGS++;}
  else{temp.flag = 1; FLAGS--;}
  BOARD.tiles[SELECT_X][SELECT_Y] = temp;
  display_tile(SELECT_X, SELECT_Y);//draw tile
  return 1;
}

int detonate_tile(void){
  tile_t temp = BOARD.tiles[SELECT_X][SELECT_Y];
  if (temp.flag || temp.revealed) return 0; //do nothing, can't detonate flagged tile or revealed tile
  if (temp.mine_flag)  {
    temp.revealed = 1;  
    display_tile(SELECT_X, SELECT_Y);
    GAMESTATE = LOSE;
    return -1;} //exploded mine, call end of game
  temp.revealed = 1;
  display_tile(SELECT_X, SELECT_Y);
  chain_reveal(SELECT_X, SELECT_Y, 1);
  BOARD.tiles[SELECT_X][SELECT_Y] = temp;
  if (REVEALED_TILES == 54) GAMESTATE = WIN;
  return 1;  
}

int chain_reveal(int x, int y, int start){
  tile_t temp = BOARD.tiles[x][y];
  if (temp.flag || (temp.revealed && !start) || temp.mine_flag) return 0;
  REVEALED_TILES++;
  temp.revealed = 1;
  BOARD.tiles[x][y] = temp;
  display_tile(x, y);//draw tile
  if (!temp.count){//if the count is 0, recur on all adjacent tiles
    int r, c;
    for (r =-1; r<2; r++){
      for(c = -1; c<2; c++){
	if (in_bounds(x+r, y+c) && !(!r && !c)) chain_reveal(x+r, y+c, 0);
      }
    }
  }
  return 1;
}

void init_globals(void){
  FLAGS = 0;
  REVEALED_TILES = 0;
  SELECT_X = 0;
  SELECT_Y = 0;
  GAMESTATE = 0;
}

void start_game(void){
  board_init();// set/reset board
  boardGen();
  init_globals();
  selectTile(BLACK);
}

