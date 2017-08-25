/* start_screen.c --- 
 * 
 * Filename: start_screen.c
 * Description: 
 * Author: Ethan Lawton (elawton2) & Anna Branam (akbranam)
 * Maintainer: ^^^^^^^^^^^
 * Created: Fri Apr 15 2016
 * Last-Updated: 4/15/16
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 */




#include <f3d_nunchuk.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>
#include "start_screen.h"


char *option[] = {"Start", "How to Play", "Controls", "Exit"};

void setStartScreen(){
  f3d_lcd_fillScreen(BLACK);
  f3d_lcd_drawString(30, 20, "Minesweeper!", WHITE, BLACK);
  f3d_lcd_drawString(40, 60, option[0], WHITE, BLACK);
  f3d_lcd_drawString(40, 70, option[1], WHITE, BLACK);
  f3d_lcd_drawString(40, 80, option[2], WHITE, BLACK);
  f3d_lcd_drawString(40, 90, option[3], WHITE, BLACK);
  f3d_lcd_drawString(40, 60+(choice*10), option[choice], RED, BLACK);
}


/* void setStartScreen(void){ */
/*   f3d_lcd_fillScreen(BLACK); */
/*   f3d_lcd_drawString(25, 30, "MINE SWEEPER", BLUE, BLACK); */
/*   f3d_lcd_drawString(15, 50, "Press Z to Start", WHITE, BLACK); */
/*   f3d_lcd_drawString(25, 70, "Press C for", WHITE, BLACK); */
/*   f3d_lcd_drawString(20, 85, "Instructions", WHITE, BLACK); */
/* } */

void setInstructionScreen(void){
  f3d_lcd_fillScreen(BLACK); 

  f3d_lcd_drawString(40, 20, "Controls: ", WHITE, BLACK);
  f3d_lcd_drawString(5, 40, "Left:  Move left", WHITE, BLACK);
  f3d_lcd_drawString(5, 50, "Right: Move right", WHITE, BLACK);
  f3d_lcd_drawString(5, 60, "Up:    Move up", WHITE, BLACK); 
  f3d_lcd_drawString(5, 70, "Down:  Move down", WHITE, BLACK);
  f3d_lcd_drawString(5, 80, "C:     Reveal tile", WHITE, BLACK);
  f3d_lcd_drawString(5, 90, "Z:     Flag tile", WHITE, BLACK);
}


/* void setInstructionScreen(void){ */
/*   f3d_lcd_fillScreen(BLACK); */
/*   f3d_lcd_drawString(15, 10,"Instructions", BLUE, BLACK); */
/*   f3d_lcd_drawString(15, 30,"Joystick: Move", WHITE, BLACK); */
/*   f3d_lcd_drawString(15, 50,"Z: Toggle Flag", WHITE, BLACK); */
/*   f3d_lcd_drawString(15, 70,"C: Detonate!", WHITE, BLACK); */
/*   f3d_lcd_drawString(15, 90,"User Button: Quit", WHITE, BLACK); */
/*   f3d_lcd_drawString(15, 110,"Don't Lose", WHITE, BLACK); */
/*   f3d_lcd_drawString(15, 130,"Back : C", WHITE, BLACK); */
/* } */

void howToPlay(void){
  
  f3d_lcd_fillScreen(BLACK);
  
 
  f3d_lcd_drawString(30, 20, "How to Play:", WHITE, BLACK);
  f3d_lcd_drawString(3, 40, "Flag all 10 mines.", WHITE, BLACK);
  f3d_lcd_drawString(3, 50, "Hitting a mine ", WHITE, BLACK); 
  f3d_lcd_drawString(3, 60, "loses you the game.", WHITE, BLACK);
  f3d_lcd_drawString(3, 80, "The number represents", WHITE, BLACK);
  f3d_lcd_drawString(3, 90, "the number of mines ", WHITE, BLACK);
  f3d_lcd_drawString(3, 100, "around that tile.", WHITE, BLACK);
  f3d_lcd_drawString(30, 130, "Good luck!", WHITE, BLACK); 
}

void win_screen(void){
  f3d_lcd_fillScreen(WHITE);
  f3d_lcd_drawString(15, 50,"YOU WIN!", BLUE, WHITE);
  f3d_lcd_drawString(15, 90,"Press C", BLACK, WHITE);
}

void lose_screen(void){
  f3d_lcd_fillScreen(BLACK);
  f3d_lcd_drawString(15, 50,"YOU LOSE!", RED, BLACK);
  f3d_lcd_drawString(15, 90,"Press C", WHITE, BLACK);
}
