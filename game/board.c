
/* *************************
 *  Authors: Ethan Lawton (elawton2) + Anna Branam (akbranam)
 *  Data Created: 4/15/16
 *  Last Modified On: 4/20/16
 *  Last Updated By: Ethan Lawton (elawton2) 
 *
 *
 *  Update Log:
 *           4/15/16: We are not getting proper random numbers within bomb_generator();
 *           4/20/16: Tiles were drawn with boardGen(). Coordinates of the tiles were fixed. Random numbers are still not working 
 *
 ***************************/


#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <f3d_lcd_sd.h>
#include <f3d_gyro.h>
#include "board.h"
#include "game_play.h"

float data[3];
int dataCount = 0;

//checks that coords are in bounds
int in_bounds(int x, int y){
  if ((x>-1 && x <   BOARD_SIZE) &&  (y >-1 && y<   BOARD_SIZE)) return 1;
  return 0;
}

//16X20 tiles w? 1 pix border
void drawBlank(int x, int y){
  int temp_x, temp_y;
  for (temp_x= x; temp_x<x+15; ++temp_x){
    for (temp_y = y; temp_y<y+19; ++temp_y){
      f3d_lcd_drawPixel(temp_x, temp_y, WHITE);
    }}}

void boardGen(void){
  int x, y;
  f3d_lcd_fillScreen(LightGrey);
  for(x = 0; x<BOARD_SIZE;x++){
    for (y = 0; y<BOARD_SIZE;y++){
      tile_t temp = BOARD.tiles[x][y];
      drawBlank(temp.x_pos, temp.y_pos);
    }}}

void board_init(void){
  int x, y, i = 0, new_x = 0, new_y = 0;
  printf("init\n"); 
   for(x = 0; x < BOARD_SIZE; x++){
     for(y = 0; y < BOARD_SIZE; y++){
       tile_t temp = BOARD.tiles[x][y];
       temp.mine_flag = 0;
       temp.count = 0;
       temp.flag = 0;
       temp.revealed = 0;
       temp.selected = 0;
       temp.x_pos = new_x;
       temp.y_pos = new_y;
       BOARD.tiles[x][y] = temp;
       new_y+= 20; 
     }
     new_y = 0;
     new_x+=16;
   }
   bomb_generator();
}

void bomb_generator(void){ 
  f3d_gyro_getdata(data);
  srand(data[(dataCount++)%2] * 12512412 * SystemCoreClock);//etch-a-sketch factor
  int count = 0, random_x, random_y;
  while(count < 10){
    random_x = rand() % BOARD_SIZE;
    random_y = rand() % BOARD_SIZE;
    tile_t temp = BOARD.tiles[random_x][random_y];
    if(!temp.mine_flag){
      temp.mine_flag = 1;
      count++;
      int r, c;
      for (r =-1; r<2; r++){
	for(c = -1; c<2; c++){
	  int ax = random_x+r, ay = random_y+c;
	  if ((ax>-1 && ax <BOARD_SIZE) &&  (ay>-1 && ay<BOARD_SIZE) && !((c==0)&&(r==0))){
	    tile_t adj = BOARD.tiles[ax][ay];
	    if(!adj.mine_flag){
	      adj.count +=1;
	      BOARD.tiles[ax][ay] = adj;
	    }
	  }
	}
      }
    }
    BOARD.tiles[random_x][random_y] = temp;
    temp = BOARD.tiles[random_x][random_y];
  }
}


//display individual tile (still working on this)
int display_tile(int x, int y){//add selected tile case
  tile_t temp = BOARD.tiles[x][y];
  if(!temp.revealed){
    if(temp.flag) {f3d_lcd_drawChar((temp.x_pos + 8), (temp.y_pos + 10), 'F', RED, WHITE); return 9;}//draw flagged tile
    drawBlank(temp.x_pos, temp.y_pos);//draw unrevealed tile
    return 10; //draw regular tile
  }
  if (!temp.mine_flag){
    f3d_lcd_drawChar((temp.x_pos + 8), (temp.y_pos + 10), temp.count+'0', BLACK, WHITE);//draw count on tile 
    return temp.count;
  }
  f3d_lcd_drawChar((temp.x_pos + 8), (temp.y_pos + 10), '*', BLACK, WHITE);//draw mined tile
  return -1;//mine
}
	
