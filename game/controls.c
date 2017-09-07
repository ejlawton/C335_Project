
/* controls.c --- 
 * 
 * Filename: controls.c
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



#include <stdio.h>
#include <f3d_nunchuk.h>
#include "game_play.h"
#include <f3d_lcd_sd.h>
#include "start_screen.h"
#include "controls.h"
#include <ff.h>
#include <queue.h>
#include <diskio.h>
#include <math.h>



#define TIMER 20000
#define AUDIOBUFSIZE 128

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;

FATFS Fatfs;		/* File system object */
FIL fid;		/* File object */
BYTE Buff[512];		/* File read buffer */
int ret;

struct ckhd {
  uint32_t ckID;
  uint32_t cksize;
};

struct fmtck {
  uint16_t wFormatTag;      
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};

nunchuk_t Ndata;
int soundState;

char *sOption[3] = {"bell.wav", "thermo.wav", "thermo.wav"};
int s_op = 0;


void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}



void play_game(void){


 

   f3d_nunchuk_read(&Ndata);
    if (user_btn_read()){
      while(user_btn_read());
      GAMESTATE = 2; 
    }
    if(Ndata.c){
      detonate_tile();
      while(Ndata.c) f3d_nunchuk_read(&Ndata);
    }
    if(Ndata.z){
      if(toggle_flag() == 1) soundState = 1;
      while(Ndata.z) f3d_nunchuk_read(&Ndata);
    }
    if(Ndata.jx >= 250) {
      move_selection(1,0);
      while(Ndata.jx >= 250) f3d_nunchuk_read(&Ndata);
    }
    if(Ndata.jx <= 5){
      move_selection(-1,0);
       while(Ndata.jx <= 5) f3d_nunchuk_read(&Ndata);
    }
    if(Ndata.jy >= 250){
      move_selection(0,-1);
      while(Ndata.jy >= 250) f3d_nunchuk_read(&Ndata);
    }
    if(Ndata.jy <= 5){
      move_selection(0, 1);
      while(Ndata.jy <= 5) f3d_nunchuk_read(&Ndata);
    }
}

void exit_game(int state){
  if(state == LOSE)lose_screen();
  if(state == WIN)win_screen();
  if (state == EXIT)setStartScreen();
}

void spam(void){

  
  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;
  int bytesread;
  

  
  

  
printf("Reset\n");
  
 f_mount(0, &Fatfs);/* Register volume work area */
 
 int playSound(int op){
   printf("\nOpen bell.wav\n");
   if (op == 0) rc = f_open(&fid, "bell.wav", FA_READ);
   if (op == 1) rc = f_open(&fid, "beep.wav", FA_READ);
   if (op == 2) rc = f_open(&fid, "cheering.wav", FA_READ);
   if (op == 3) rc = f_open(&fid, "boo.wav", FA_READ);
  
  if (!rc) {
    struct ckhd hd;
    uint32_t  waveid;
    struct fmtck fck;
    
    readckhd(&fid, &hd, 'FFIR');
    
    f_read(&fid, &waveid, sizeof(waveid), &ret);
    if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
      return -1;
    
    readckhd(&fid, &hd, ' tmf');
    
    f_read(&fid, &fck, sizeof(fck), &ret);
    
    // skip over extra info
    
    if (hd.cksize != 16) {
      printf("extra header info %d\n", hd.cksize - 16);
      f_lseek(&fid, hd.cksize - 16);
    }
    
    printf("audio format 0x%x\n", fck.wFormatTag);
    printf("channels %d\n", fck.nChannels);
    printf("sample rate %d\n", fck.nSamplesPerSec);
    printf("data rate %d\n", fck.nAvgBytesPerSec);
    printf("block alignment %d\n", fck.nBlockAlign);
    printf("bits per sample %d\n", fck.wBitsPerSample);
    
    // now skip all non-data chunks !
    
    while(1){
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
	break;
      f_lseek(&fid, hd.cksize);
    }
    
    printf("Samples %d\n", hd.cksize);
    
    // Play it !
    
    //      audioplayerInit(fck.nSamplesPerSec);
    
    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
    while (hd.cksize) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(Audiobuf, AUDIOBUFSIZE/2);
	f_read(&fid, Audiobuf, next, &ret);
	hd.cksize -= ret;
	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
	hd.cksize -= ret;
	audioplayerWhole = 0;
      }
    }
    audioplayerStop();
  }
  
  printf("\nClose the file.\n"); 
  rc = f_close(&fid);
  
  if (rc) die(rc);
 }  

  f3d_nunchuk_read(&Ndata);
    if(Ndata.jy >= 250){
      if (choice == 0) choice = 4;
      choice --;
      setStartScreen();
      while(Ndata.jy >= 250) f3d_nunchuk_read(&Ndata);
    }
    if(Ndata.jy <= 5){
      choice = (choice + 1) % 4;
      setStartScreen();
      while(Ndata.jy <= 5) f3d_nunchuk_read(&Ndata);
    }
    
    if(Ndata.c){
      while(Ndata.c) f3d_nunchuk_read(&Ndata);
      switch(choice)
	{
	case 0: 
          playSound(0);
	  start_game();
	  while(1){
	    if(soundState == 1) playSound(soundState--);
	    if (GAMESTATE != 0){
	      exit_game(GAMESTATE);
	      switch(GAMESTATE)
		{
		case 1: 
		  playSound(2);
		case -1: 
		  playSound(3);
		}
	      GAMESTATE = 0;
	      break;}
	    play_game();}
	  break;
	case 1:
	  howToPlay();
	  break;
	case 2:
	  setInstructionScreen();
	  break;
	}
    }

  /* if(Ndata.c){ */
  /*   //printf ("%d\n",  CSCREEN); */
  /*   while(Ndata.c == 1) f3d_nunchuk_read(&Ndata); */
  /*   if (CSCREEN == 1){ */
  /*     CSCREEN --; */
  /*     setStartScreen();} */
  /*   if (CSCREEN == 0){ */
  /*     CSCREEN ++; */
  /*     setInstructionScreen();} */
  /* } */
  /* if(Ndata.z && (CSCREEN == 0)){ */
  /*   while(Ndata.z) f3d_nunchuk_read(&Ndata); */
  /*   start_game(); */
  /*   while(1){ */
  /*     if (GAMESTATE != 0){ */
  /* 	exit_game(GAMESTATE); */
  /* 	GAMESTATE = 0; */
  /* 	//	while(!Ndata.c) f3d_nunchuk_read(&Ndata); */
  /* 	break;} */
  /*     play_game();} */
  /* } */
    }
