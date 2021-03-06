/* main.c --- 
 * 
 * Filename: main.c
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

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <f3d_user_btn.h>
#include <ff.h>
#include <ffconf.h> 
#include <integer.h>

#include <queue.h>
#include <f3d_dac.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "board.h"
#include "game_play.h"
#include "start_screen.h"
#include "controls.h"

int main(void)
{
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);  

  f3d_uart_init();
  f3d_user_btn_init();
  f3d_led_init();
  f3d_i2c1_init(); 
  f3d_timer2_init();
  f3d_dac_init();
  f3d_delay_init();
  f3d_rtc_init();
  f3d_gyro_init();
  f3d_systick_init();
  f3d_lcd_init();  

  setStartScreen();
  while (1){
    spam();
  }}




#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif
