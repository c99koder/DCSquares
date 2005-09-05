//#include <windows.h>
#ifdef MACOS
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#ifdef DREAMCAST
#include <kos.h>
#endif
#include "sys.h"
#include "stream.h"
#include "font.h"
#include "screen.h"
#include "input.h"
#include "squares.h"
#include "theme.h"
#include <time.h>
#include <stdlib.h>

#define BOARD_X 77
#define BOARD_Y 19

int board[BOARD_X][BOARD_Y];

int sx=3,sy=3,sdx=1,sdy=0,ssize=3;
int sscore=0,slives=3;
#ifdef DREAMCAST
extern sfxhnd_t powerup;
extern sfxhnd_t powerdown;
extern sfxhnd_t collect;  
extern sfxhnd_t gameover;
#endif

#ifdef WIN32
void delay(float seconds) {
	clock_t delayend=clock()+(seconds * CLOCKS_PER_SEC);
  while(clock() < delayend) sys_render_begin(); //pump the win32 message queue so Windows can still "multitask"... *sigh*
}
#endif

int snakemain() {/*WINAPI WinMain(	HINSTANCE	hInstance, HINSTANCE	hPrevInstance, LPSTR lpCmdLine,int nCmdShow) {*/
  int x,y;
  bool go=1;
  bool dead=0;

  srand(time(NULL));
	
	sx=rand()%BOARD_X;
	sy=rand()%BOARD_Y;
	
	if(sx<(BOARD_X/2)) sdx = 1; else sdx = -1;
	sdy = 0;
	sscore=0;
	slives=3;

  //initialize the screen
  //fs_chdir("/rd");
  //sys_init("SnakeyGame");
  init_screen();
  //c99_do_render=0; //we handle calling render_screen() ourselves, don't do it on every cout
  set_border(0); //black
  color(15,0);    //white on black
  clear_screen();
  set_do_wrap(0); //don't wrap lines
  set_do_scroll(0); //don't scroll the screen
  //initialize the board
  for(x=0;x<BOARD_X;x++) {
    for(y=0;y<BOARD_Y;y++) {
      board[x][y]=0;
    }
  }

  //place an item
  board[rand()%BOARD_X][rand()%BOARD_Y]=999;

  do {
    //move head
    if(!dead) {
      sx+=sdx;
      sy+=sdy;
    }
    //check bounds
    if((sx>=BOARD_X || sx<0 || sy>=BOARD_Y || sy<0) && !dead) { 
      slives--; 
#ifdef DREAMCAST
			snd_sfx_play(gameover,250,128);
#endif
      ssize=3; 
      for(x=0;x<BOARD_X;x++) {
        for(y=0;y<BOARD_Y;y++) {
          board[x][y]=0;
        }
      }
      sx=rand()%BOARD_X;
      sy=rand()%BOARD_Y;
      board[rand()%BOARD_X][rand()%BOARD_Y]=999;
    }

    //check collisions
    if(board[sx][sy]>0 && !dead) {
      if(board[sx][sy]==999) { //item
				board[sx][sy]=0;
        ssize+=2; //increment snake size
        sscore++; //and score
				#ifdef DREAMCAST
				snd_sfx_play(collect,250,128);
				#endif
        do { //place a new item in an empty spot
          x=rand()%BOARD_X; y=rand()%BOARD_Y;
        } while(board[x][y]!=0);
        board[x][y]=999;
        //make the stretch work correctly
        for(x=0;x<BOARD_X;x++) {
          for(y=0;y<BOARD_Y;y++) {
            if(board[x][y]>0&&board[x][y]!=999) board[x][y]+=2;
          }
        }
      } else { //snake
        slives--; 
#ifdef DREAMCAST
				snd_sfx_play(gameover,250,128);
#endif
        ssize=3; 
        for(x=0;x<BOARD_X;x++) {
          for(y=0;y<BOARD_Y;y++) {
            board[x][y]=0;
          }
        }
        sx=rand()%BOARD_X;
        sy=rand()%BOARD_Y;
      }
      //board[rand()%BOARD_X][rand()%BOARD_Y]=999;
    }
    if(slives<=0) dead=1;
    board[sx][sy]=ssize+1;

    //decrement the array (simulates motion)
    for(x=0;x<BOARD_X;x++) {
      for(y=0;y<BOARD_Y;y++) {
        if(board[x][y]>0&&board[x][y]!=999) board[x][y]--;
      }
    }

    //render the screen
    locate(0,0);
    color(15,0);
    cout << "Score: " << sscore << endl;
    locate(70,0);
		cout << "Lives: " << (dead==0?slives:0) << endl; 
    color(14,0); //yellow on black
    cout << (char)218;
    for(x=0;x<BOARD_X;x++)
      cout << (char)196;
    cout << (char)191 << endl;
    for(y=0;y<BOARD_Y;y++) {
      color(14,0); //yellow on black
      cout << (char)179;
      for(x=0;x<BOARD_X;x++) {
        color(15,0); //white on black
        if(board[x][y]==999 && dead==0) { //draw an item
          color(13,0);
          cout << (char)4;
        } else if(board[x][y]>0&&dead==0) cout << "*"; //draw a snake piece
        else cout << " "; //draw a space
      }
      color(14,0); //yellow on black
      cout << (char)179 << endl;
    }
    cout << (char)192;
    for(x=0;x<BOARD_X;x++)
      cout << (char)196;
    cout << (char)217<<endl;
    color(15,0);
    cout << "SnakeGame v0.02                                            (C) 2003 Sam Steele";
    if(dead) {
      color(11,0);
      locate(36,11); cout << "You Died!";
      locate(36,12); cout << " Press B";
    }
    if(sys_render_begin()) { //refresh the screen
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
			glLoadIdentity();
      render_screen();
      sys_render_finish();
    }
    for(x=0;x<6;x++) { //we do this 6 times so that we can poll the keyboard 6 times more than we move the snake on screen
      delay(0.01);
      switch(poll_game_device(0)) {
      case MOVE_UP:
        if(sdy==0) {
          sdx=0;
          sdy=-1;
        }
        break;
      case MOVE_DOWN:
        if(sdy==0) {
          sdx=0;
          sdy=1;
        }
        break;
      case MOVE_LEFT:
        if(sdx==0) {
          sdx=-1;
          sdy=0;
        }
        break;
      case MOVE_RIGHT:
        if(sdx==0) {
          sdx=1;
          sdy=0;
        }
        break;
      case QUIT_BTN:
        go=0;
				while(poll_game_device(0) == QUIT_BTN);
        break;
      }
    }
  } while(go);
  return 0;
}  
