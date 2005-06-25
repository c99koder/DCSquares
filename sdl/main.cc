#ifdef WIN32
#include <windows.h>
#ifdef DIRECTX
#include <dsutil.h>
#include <atlbase.h>
#endif
#endif
#ifdef MACOS
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#ifdef SDL
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#ifdef MACOS
#include <SDL_mixer/SDL_mixer.h>
#else
#include <SDL/SDL_mixer.h>
#endif
#endif
#ifdef UNIX
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#endif
#ifdef DREAMCAST
#include <kos.h>
#include <oggvorbis/sndoggvorbis.h>
#include <dc/sound/sfxmgr.h>

#include "libmenu.h"
int snakemain();
#endif
#ifndef WIN32
#include <sys/time.h>
#endif
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "texture.h"
#include "sys.h"
#include "input.h"
#include "font.h"
#include "level.h"

#include "text.h"
#include "squares.h"
#include "http.h"
#include "options.h"
#include "score.h"
#include "hud.h"
#include "game.h"
#include "theme.h"
#include "rand.h"

char debugtxt[256];

void debug(char *msg) {
  printf("%s\n",msg);
}

void status(char *msg) {
  printf("Status: %s\n",msg);
}

void MessageBox(char *title, char *msg) {
 printf("MessageBox: %s: %s\n",title,msg);
}

#ifndef DREAMCAST
void callback(char *msg) {
  printf("http: %s\n",msg);
}
#else
void callback(char *msg) {
}
#endif

extern gameoptions_t gameoptions;

#ifdef DREAMCAST
extern "C" {
void update_lcds();
}
#endif

extern int combo;
extern int score;
extern int squares;
extern int scoreval;
extern float tickval;
extern float power;
extern float speedval;
extern int powerup_mode;
extern int loading_tex;
int menu_select=0;
squarelist *player;
int exitflag=0;
int paused=0;
float game_gt;
extern int bg_tex;
extern int title_tex;
extern int menu_tex;
extern int effect_type;
extern float effect_timer;
extern int maxcombo;
extern struct themeinfo_t themeinfo;
extern char highcode[20];

#ifdef SDL
extern Mix_Music *bgm;
extern Mix_Music *title;
#endif

int render_menu(char menu[][20], int size, float gt) {
  static int x=320,y=240,ox=0,oy=0,lmb,c=-1;
  int w=0,i;
	char buf[200];
	static float tm=0;
	static float oldgt=gt;
	
	float mx=float(themeinfo.game_x + themeinfo.game_w) /2.0f;
	
	if(gt < oldgt) oldgt=gt;
	
	static int inside_menu=0;
  
  set_font_size(18);

  for(i=0;i<size;i++) {
    if(txt_width(menu[i])>w) w=txt_width(menu[i]);
  }
  w+=20;
  
	glLoadIdentity();
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glColor4f(0,0,0,0.6);
	glBegin(GL_QUADS);
	glVertex3f(mx-(w/2)-4,185-4,0.9);
	glVertex3f(mx-(w/2)+w+4,185-4,0.9);
	glVertex3f(mx-(w/2)+w+4,185+(size*24)+4,0.9);
	glVertex3f(mx-(w/2)-4,185+(size*24)+4,0.9);
	glEnd();
	glColor4f(0.8,0.8,0.8,0.4);
	glBegin(GL_QUADS);
	glVertex3f(mx-(w/2),185,0.91);
	glVertex3f(mx-(w/2)+w,185,0.91);
	glVertex3f(mx-(w/2)+w,185+(size*24),0.91);
	glVertex3f(mx-(w/2),185+(size*24),0.91);
	glEnd();
	
	if(detect_mouse()) {
		read_mouse(&x,&y,&lmb);
		
		if(x<0) x=0;
		if(x>640) x=640;
		if(y<0) y=0;
		if(y>480) y=480;
		//sprintf(buf,"x: %i, y: %i, (320-(w/2)): %i, (320-(w/2)+w): %i",x,y,(320-(w/2)),(320-(w/2)+w));
		//draw_txt(20,20,buf,0,0,0,1,16);
		
		if((ox!=x || oy != y) && x>(273-(w/2)) && x<(273-(w/2)+w) && y>185 && y< (185+(size*24))) {
			menu_select=(y-185)/24;
			inside_menu=1;
		} else {
			inside_menu=0;
		}
		
		ox=x;
		oy=y;
glLoadIdentity();
#ifdef DREAMCAST
  glDisable(GL_TEXTURE_2D);
  glColor4f(0,0,0,0.8);
  glBegin(GL_TRIANGLES);
  glVertex3f(x-2,y-4,0.99);
  glVertex3f(x+18,y+16,0.99);
  glVertex3f(x-2,y+24,0.99);
  glEnd();
  glColor3f(0,0.6,0.8);
  glBegin(GL_TRIANGLES);
  glVertex3f(x,y,1.0);
  glVertex3f(x+15,y+15,1.0);
  glVertex3f(x,y+21,1.0);
  glEnd();
#endif
	} else {
		x=320;
		y=240;
		lmb=0;
	}
	
		glColor4f(1,1,1,0.7);
		glBegin(GL_QUADS);
		glVertex3f(mx-(w/2),185+(menu_select*24),0.91);
		glVertex3f(mx-(w/2)+w,185+(menu_select*24),0.91);
		glVertex3f(mx-(w/2)+w,185+((menu_select+1)*24),0.91);
		glVertex3f(mx-(w/2),185+((menu_select+1)*24),0.91);
		glEnd();	
	glEnable(GL_TEXTURE_2D);

  for(i=0;i<size;i++) {
    draw_txt(mx-(w/2)+12,208+(i*24),menu[i],0,0,0,1,18);
    draw_txt(mx-(w/2)+10,206+(i*24),menu[i],0.8,0.8,0.8,1,18);
  }
	
	c=poll_game_device(0);
	if(c==0) {
		tm=0;
		oldgt=gt;
	}
	if(tm>0) {
		tm-=(gt-oldgt);
	} else {
		if(c!=0) {
			tm=0.25;
			//printf("c: %i\n",c);
			switch(c) {
				case MOVE_UP:
					menu_select--;
					if(menu_select<0) menu_select=0;
					break;
				case MOVE_DOWN:
					menu_select++;
					if(menu_select>=size) menu_select = size-1;
					break;
				case BUTTON_X:
#ifdef DREAMCAST
					snakemain();
#endif
					break;
			}
		}
	}
	oldgt=gt;
	if(lmb || c==FIRE_BTN || c==START_BTN) return menu_select; else return -1;
}

void delay(float seconds) {
#ifdef SDL
	float st = ((float)SDL_GetTicks() / 1000.0f)+seconds;
	
	while((float)SDL_GetTicks() / 1000.0f < st);
#endif
#ifdef DREAMCAST
	thd_sleep(seconds*1000);
#endif
}

void high_scores() {
#ifdef DREAMCAST
  uint32 s,ms,tm;
  float st=0,gt=0,ot=0;
#endif
#ifdef SDL
  float st=0,gt=0,ot=0;
#endif
#ifdef DIRECTX
  DWORD delayend;
#endif
	squarelist *c;
	int max=0,size=6;

	char tmp[256];
	char tmp2[30];
	int seed,x,y,lmb;
  int oldx=-1,oldy=-1;
	
	tickval=1.0f; //1.0
	speedval=1.2f; //1.2
	scoreval=100; //100
	power=0;
	powerup_mode=-1;
	effect_type=-1;
	
#ifdef SDL
  st = (float)SDL_GetTicks() / 1000.0f;
	SDL_ShowCursor(SDL_DISABLE);
	Mix_VolumeMusic(MIX_MAX_VOLUME);
#endif
#ifdef DREAMCAST

	timer_ms_gettime(&s,&ms);
	st=s+((float)ms/1000.0f);
#endif
	destroy_list();
	init_genrand(st*1000);
	
	while(1) {
    if(poll_game_device(0)==QUIT_BTN) {
      exitflag=1;
      break;
    }
		read_mouse(&x,&y,&lmb);
    if(poll_game_device(0)==START_BTN || lmb!=0 || gt > 5) {
      break;
    }
	if(paused==0) {
#ifdef DREAMCAST
    timer_ms_gettime(&s,&ms);

	  update_lcds();
		gt=(s+((float)ms/1000.0f))-st;
#endif
#ifdef SDL
		gt=((float)SDL_GetTicks()/1000.0f)-st;
#endif
#ifdef DIRECTX
					while(ShowCursor(FALSE)>=0);
  DWORD delayend=timeGetTime()+30;
#endif
	
		add_squares(gt - ot);
		update_squares(gt - ot);

		if(sys_render_begin()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_bg_game(menu_tex,1);
#ifdef DREAMCAST
			glKosFinishList();
#else
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
			render_squares(0.6);
			/*glColor4f(0,0,0,0.4);
			glBegin(GL_QUADS);
			glVertex3f(24,40,0.11);
			glVertex3f(24+497,40,0.11);
			glVertex3f(24+497,40+401,0.11);
			glVertex3f(24,441,0.11);
			glEnd();*/

			if(gt>= 0.5) render_highscores();

			if(gt<0.5) {
#ifndef DREAMCAST
				glEnable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
#endif
				glDisable(GL_TEXTURE_2D);
				glColor4f(0.8,0.8,0.8,1-(gt/0.5));
				glBegin(GL_QUADS);
				glVertex3f(0,0,0.9);
				glVertex3f(640,0,0.9);
				glVertex3f(640,480,0.9);
				glVertex3f(0,480,0.9);
				glEnd();
			}
			sys_render_finish();
		}

   	ot = gt;

#ifdef DIRECTX
    while(timeGetTime() < delayend) {
			sys_render_begin();
		}
#endif
	}
	}
	st+=gt;
	gt=0;
	ot=0;

	while(gt <= 0.49f) {
#ifdef DREAMCAST
    timer_ms_gettime(&s,&ms);

		gt=(s+((float)ms/1000.0f))-st;
#endif
#ifdef SDL
		gt=((float)SDL_GetTicks()/1000.0f)-st;
#endif

		update_squares(gt-ot);

		if(sys_render_begin()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_bg_game(menu_tex,1);
#ifdef DREAMCAST
			glKosFinishList();
#else
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
			/*glColor4f(0,0,0,0.4);
			glBegin(GL_QUADS);
			glVertex3f(24,40,0.11);
			glVertex3f(24+497,40,0.11);
			glVertex3f(24+497,40+401,0.11);
			glVertex3f(24,441,0.11);
			glEnd();*/
						//render_squares(1.0f-((gt)/2.0f));
			//render_highscores();
#ifndef DREAMCAST
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
			glDisable(GL_TEXTURE_2D);
			glLoadIdentity();
			glColor4f(0.8,0.8,0.8,(gt/0.5));
			glBegin(GL_QUADS);
			glVertex3f(0,0,0.9);
			glVertex3f(640,0,0.9);
			glVertex3f(640,480,0.9);
			glVertex3f(0,480,0.9);
			glEnd();
			sys_render_finish();
		}

		ot=gt;
	}

	destroy_list();
}

void level_stats() {
#ifdef DREAMCAST
  uint32 s,ms,tm;
  float st=0,gt=0,ot=0;
#endif
#ifdef SDL
  float st=0,gt=0,ot=0;
#endif
#ifdef DIRECTX
  DWORD delayend;
#endif
	squarelist *c;
	int max=0,size=6;

	char tmp[256];
	char tmp2[30];
	int seed,x,y,lmb;
  int oldx=-1,oldy=-1;
	
	tickval=1.0f; //1.0
	speedval=1.2f; //1.2
	scoreval=100; //100
	power=0;
	powerup_mode=-1;
	effect_type=-1;

#ifdef SDL
  st = (float)SDL_GetTicks() / 1000.0f;
	SDL_ShowCursor(SDL_DISABLE);
	Mix_VolumeMusic(MIX_MAX_VOLUME);
#endif
#ifdef DREAMCAST

	timer_ms_gettime(&s,&ms);
	st=s+((float)ms/1000.0f);
#endif
	destroy_list();
	init_genrand(st*1000);
	
	while(1) {
    if(poll_game_device(0)==QUIT_BTN) {
      exitflag=1;
      break;
    }
		read_mouse(&x,&y,&lmb);
    if(poll_game_device(0)==START_BTN || lmb!=0) {
      break;
    }
	if(paused==0) {
#ifdef DREAMCAST
    timer_ms_gettime(&s,&ms);

	  update_lcds();
		gt=(s+((float)ms/1000.0f))-st;
#endif
#ifdef SDL
		gt=((float)SDL_GetTicks()/1000.0f)-st;
#endif
#ifdef DIRECTX
					while(ShowCursor(FALSE)>=0);
  DWORD delayend=timeGetTime()+30;
#endif
	
		add_squares(gt - ot);
		update_squares(gt - ot);

		if(sys_render_begin()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_bg_game(menu_tex,1.0);
#ifdef DREAMCAST
			glKosFinishList();
#else
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
			render_squares(0.6);
/*			glColor4f(0,0,0,0.4);
			glBegin(GL_QUADS);
			glVertex3f(24,40,0.11);
			glVertex3f(24+497,40,0.11);
			glVertex3f(24+497,40+401,0.11);
			glVertex3f(24,441,0.11);
			glEnd();*/

			if(gt>= 0.5) render_win(game_gt,(gt-ot));

			if(gt<0.5) {
#ifndef DREAMCAST
				glEnable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
#endif
				glDisable(GL_TEXTURE_2D);
				glColor4f(0.8,0.8,0.8,1-(gt/0.5));
				glBegin(GL_QUADS);
				glVertex3f(0,0,0.9);
				glVertex3f(640,0,0.9);
				glVertex3f(640,480,0.9);
				glVertex3f(0,480,0.9);
				glEnd();
			}
			sys_render_finish();
		}

   	ot = gt;

#ifdef DIRECTX
    while(timeGetTime() < delayend) {
			sys_render_begin();
		}
#endif
	}
	}
	st+=gt;
	gt=0;
	ot=0;

	while(gt <= 0.49f) {
#ifdef DREAMCAST
    timer_ms_gettime(&s,&ms);

		gt=(s+((float)ms/1000.0f))-st;
#endif
#ifdef SDL
		gt=((float)SDL_GetTicks()/1000.0f)-st;
#endif

		update_squares(gt-ot);

		if(sys_render_begin()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_bg_game(title_tex,1);
#ifdef DREAMCAST
			glKosFinishList();
#else
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
			glColor4f(0,0,0,0.4);
			glBegin(GL_QUADS);
			glVertex3f(24,40,0.11);
			glVertex3f(24+497,40,0.11);
			glVertex3f(24+497,40+401,0.11);
			glVertex3f(24,441,0.11);
			glEnd();
						//render_squares(1.0f-((gt)/2.0f));
			//render_highscores();
#ifndef DREAMCAST
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
			glDisable(GL_TEXTURE_2D);
			glLoadIdentity();
			glColor4f(0.8,0.8,0.8,(gt/0.5));
			glBegin(GL_QUADS);
			glVertex3f(0,0,0.9);
			glVertex3f(640,0,0.9);
			glVertex3f(640,480,0.9);
			glVertex3f(0,480,0.9);
			glEnd();
			sys_render_finish();
		}

		ot=gt;
	}

	destroy_list();
}

void title_screen() {
#ifdef DREAMCAST
  uint32 s,ms,tm;
  float st=0,gt=0,ot=0;
#endif
#ifdef SDL
  float st=0,gt=0,ot=0;
#endif
#ifdef DIRECTX
  DWORD delayend;
#endif
	squarelist *c;
	int max=0,size=6;

	char menu[][20] = { "New Game","Highscores","Options","Quit" };

	char tmp[256];
	char tmp2[30];
	int seed,x,y,lmb;
  int oldx=-1,oldy=-1;
	
	tickval=1.0f; //1.0
	speedval=1.2f; //1.2
	scoreval=100; //100
	power=0;
	powerup_mode=-1;
	effect_type=-1;
	menu_select=0;
	
	if(gameoptions.bgm) {
#ifdef DREAMCAST
    printf("Loading bgdim_loop.ogg\n");
		sndoggvorbis_stop();
		sndoggvorbis_start(theme_dir("bgdim_loop.ogg"),1);
#endif
#ifdef SDL
	  Mix_PlayMusic(title,-1);
#endif
#ifdef DIRECTX
    stop_bgm();
    play_bgm(A2W(theme_dir("bgdim_loop.ogg")));
#endif
	}
#ifdef SDL
	while(SDL_ShowCursor(SDL_ENABLE)<1);
#endif
	
#ifdef SDL
  st = (float)SDL_GetTicks() / 1000.0f;
	Mix_VolumeMusic(MIX_MAX_VOLUME);
#endif
#ifdef DREAMCAST

	timer_ms_gettime(&s,&ms);
	st=s+((float)ms/1000.0f);
#endif
	destroy_list();
	init_genrand(st*1000);
	
	//player=create_square((640/2)-4,(480/2)-4,8,PLAYER1);
	//netplay_init();

	while(1) {
		if(paused==0) {
	#ifdef DREAMCAST
			timer_ms_gettime(&s,&ms);

			update_lcds();
			gt=(s+((float)ms/1000.0f))-st;
	#endif
	#ifdef SDL
			gt=((float)SDL_GetTicks()/1000.0f)-st;
	#endif
			if((gt-ot) >= (1.0f / 120.0f)) {
				add_squares(gt - ot);
				update_squares(gt - ot);

				if(sys_render_begin()) {
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					render_bg_game(title_tex,1);
		#ifdef DREAMCAST
					glKosFinishList();
		#else
					glEnable(GL_BLEND);
					glDisable(GL_DEPTH_TEST);
		#endif
					render_squares(0.6);
					render_title(gt - ot);
					if(render_menu(menu,4,gt)!=-1) break;
					if(gt<0.5) {
		#ifndef DREAMCAST
						glEnable(GL_BLEND);
						glDisable(GL_DEPTH_TEST);
		#endif
						glDisable(GL_TEXTURE_2D);
						glColor4f(0.8,0.8,0.8,1-(gt/0.5));
						glBegin(GL_QUADS);
						glVertex3f(0,0,0.9);
						glVertex3f(640,0,0.9);
						glVertex3f(640,480,0.9);
						glVertex3f(0,480,0.9);
						glEnd();
					}
					sys_render_finish();
				}

				ot = gt;
			}
		} else {
			//status("Paused");
			//net_update();
		}
	}
	st+=gt;
	gt=0;
	ot=0;
	//x=player->size;
	while(gt <= 0.49f) {
#ifdef DREAMCAST
    timer_ms_gettime(&s,&ms);

		gt=(s+((float)ms/1000.0f))-st;
#endif
#ifdef SDL
		gt=((float)SDL_GetTicks()/1000.0f)-st;
#endif

		update_squares(gt-ot);
		//player->size=x+(200.0f * gt);

		if(sys_render_begin()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_bg_game(title_tex,1);
#ifdef DREAMCAST
			glKosFinishList();
#else
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			//if(debugtxt[0]!='\0') center(100,debugtxt,20,0);
#endif
			render_squares(1.0f-((gt)/2.0f));
#ifndef DREAMCAST
//			glEnable(GL_BLEND);
//			glDisable(GL_DEPTH_TEST);
#endif
//glDisable(GL_TEXTURE_2D);
			glLoadIdentity();
			glColor4f(0.8,0.8,0.8,(gt/0.5));
			glBegin(GL_QUADS);
			glVertex3f(0,0,0.9);
			glVertex3f(640,0,0.9);
			glVertex3f(640,480,0.9);
			glVertex3f(0,480,0.9);
			glEnd();
			sys_render_finish();
		}

		ot=gt;
	}

	destroy_list();
}


void play_game() {
	int frames=0;
  float st=0,gt=0,ot=0,ft=0,fps=0;
#ifdef DREAMCAST
  uint32 s,ms,tm;
#endif
	squarelist *c;
	int max=0,size=6;

	char tmp[256];
	char tmp2[30];
	int seed,x;
  int oldx=-1,oldy=-1;
	
	tickval=1.0f; //1.0
	speedval=1.2f; //1.2
	scoreval=100; //100
	score=0;
	combo=0;
	squares=0;
	maxcombo=0;
	power=0;
	powerup_mode=-1;

	if(gameoptions.bgm) {
#ifdef DREAMCAST
    printf("Loading bg_loop.ogg\n");
		sndoggvorbis_stop();
		sndoggvorbis_start(theme_dir("bg_loop.ogg"),1);
#endif
#ifdef SDL
	  Mix_PlayMusic(bgm,-1);
#endif
#ifdef DIRECTX
    stop_bgm();
    play_bgm(A2W(theme_dir("bg_loop.ogg")));
#endif
	}
#ifdef SDL
	while(SDL_ShowCursor(SDL_DISABLE)>0);
#endif


#ifdef SDL
  st = (float)SDL_GetTicks() / 1000.0f;
	SDL_ShowCursor(SDL_DISABLE);
#endif
#ifdef DREAMCAST
	timer_ms_gettime(&s,&ms);
	st=s+((float)ms/1000.0f);
#endif
	destroy_list();
	
	player=create_square((640/2)-4,(480/2)-4,6,PLAYER1);
	//netplay_init();

	while(1) {
	if(paused==0) {
#ifdef DREAMCAST
		update_lcds();
    timer_ms_gettime(&s,&ms);

		gt=(s+((float)ms/1000.0f))-st;
#endif
#ifdef SDL
		gt=((float)SDL_GetTicks()/1000.0f)-st;
#endif
#ifdef DIRECTX
					while(ShowCursor(FALSE)>=0);
  DWORD delayend=timeGetTime()+30;
#endif
	
	if((gt-ot)>(1.0f / 120.0f)) {
		if(gt>1.0f) add_squares(gt - ot);
		update_squares(gt - ot);

		/*if((tick%2==1) && (oldx!=player->x || oldy!=player->y)) {
		  //sprintf(tmp,"2:p,%i,%i,%i",player->x,player->y,player->size);
		  //net_sendpacket(tmp);
			oldx=player->x;
			oldy=player->y;
		}*/
		
		if(sys_render_begin()) {
			frames++;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_bg_game(bg_tex,1);
#ifdef DREAMCAST
			glKosFinishList();
#else
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			//if(debugtxt[0]!='\0') center(100,debugtxt,20,0);
#endif
			render_squares(gt<1?(gt):1.0);
			if(gt>0.4) render_score(gt);

			c=check_collide(player);
			if(c!=NULL && ((c->type == ENEMY && powerup_mode!=INVINC) || (c->type == SCORE && powerup_mode==EVIL))) {
				break;
			}
			if(check_win(gt) != 0) break;
			/*if(c!=NULL&&c->type>PLAYER_NET) {
				c->deleted=1;
				if(c->type==SCORE && powerup_mode!=EVIL) {
				  c->tm=gt;
				  //sprintf(tmp,"2:g,%i,%i",c->id,c->tm);
					//net_sendpacket(tmp);
					give_points(PLAYER1);
				}
			}*/
			//sprintf(tmp,"Tickval: %.2f Scoreval: %i Speedval: %.2f FPS: %.2f gt: %.2f",tickval,scoreval,speedval, fps,gt);
			//draw_txt(0,480,tmp,0,0,0,1,16);
			ft+=(gt-ot);
			if(ft>=1) {
				fps=float(frames+fps)/2.0f;
				ft=0;
				frames=0;
			}
			
			if(gt<0.5) {
#ifndef DREAMCAST
				glEnable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
#endif
				glDisable(GL_TEXTURE_2D);
				glColor4f(0.8,0.8,0.8,1-(gt/0.5));
				glBegin(GL_QUADS);
				glVertex3f(0,0,0.9);
				glVertex3f(640,0,0.9);
				glVertex3f(640,480,0.9);
				glVertex3f(0,480,0.9);
				glEnd();
			}
			sys_render_finish();
		}
		ot = gt;
	}
  } else {
	  //status("Paused");
		//net_update();
	}
	}
	if(combo>maxcombo) maxcombo=combo;
	game_gt=gt;
	
	st+=gt;
	gt=0;
	ot=0;
	x=player->size;
	oldx=player->x;
	oldy=player->y;
	while(gt <= 0.9f) {
#ifdef DREAMCAST
    timer_ms_gettime(&s,&ms);

		gt=(s+((float)ms/1000.0f))-st;
#endif
#ifdef SDL
		gt=((float)SDL_GetTicks()/1000.0f)-st;
#endif

		update_squares(gt-ot);
		player->size=x+(200.0f * gt);
		player->x=oldx;
		player->y=oldy;

		if(sys_render_begin()) {
			frames++;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_bg_game(bg_tex,1);
#ifdef DREAMCAST
			glKosFinishList();
#else
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			//if(debugtxt[0]!='\0') center(100,debugtxt,20,0);
#endif
			render_squares(1.0f-((gt)/2.0f));
			//render_score(gt);
#ifndef DREAMCAST
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
glDisable(GL_TEXTURE_2D);
			glColor4f(0.8,0.8,0.8,gt);
			glBegin(GL_QUADS);
			glVertex3f(0,0,0.9);
			glVertex3f(640,0,0.9);
			glVertex3f(640,480,0.9);
			glVertex3f(0,480,0.9);
			glEnd();
			sys_render_finish();
		}

		ot=gt;
	}

	destroy_list();
}

#ifdef DREAMCAST
extern "C" {
void dcb_vmu_init();
}

int main()
#endif

#ifdef MACOS
extern "C" {
int SDL_main(int argc, char **argv);
}

void check_updates();

int SDL_main(int argc, char **argv)
#endif

#ifdef LINUX
int main(int argc, char **argv)
#endif

#ifdef WIN32
int /*main*/WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
#endif
{
	char text[200];
	char text2[200];
	int len,cnt=0;
	c99_sys_init("DCSquares");
#ifdef SDL
	SDL_ShowCursor(SDL_ENABLE);
#endif
#ifdef DIRECTX
  USES_CONVERSION;
  audio_init();
#endif
#ifdef MACOS
	//chdir("DCSquares-2.0.app/Contents/Resources");
#endif
#ifdef LINUX
	if(readlink(argv[0],text,100)==-1) strcpy(text,argv[0]);

	if(text[0]!='/' && text[0]!='.') {
		char *val;
	  struct stat s;
		
		val=strtok(getenv("PATH"),":");
		do {
		  sprintf(text2,"%s/%s",val,argv[0]);
			//printf("Searching: %s\n",text2);
			if(lstat(text2,&s)!=-1) {
				//printf("Found it!\n");
				if(S_ISLNK(s.st_mode)) {
					readlink(text2,text,100);
				} else {
				  strcpy(text,text2);
				}
				break;
			}
		  val=strtok(NULL,":");
		} while(val!=NULL);
	}

	for(len=strlen(text);len>=0;len--) {
	  if(text[len]=='/') {
	    text[len]='\0';
	    break;
	  }
	}
	printf("%s\n",text);
	chdir(text);
#endif
#ifdef DREAMCAST
  //arch_set_exit_path(ARCH_EXIT_MENU);
  fs_chdir("/rd");
	//printf("Anti-crash!\n");
	debugtxt[0]='\0';

	dcb_vmu_init();
	sndoggvorbis_init();
	goat_init();
	if(goat_50hz()) vid_set_mode(DM_640x480_PAL_IL,PM_RGB565);
#endif
	//set_status_callback(callback);
	c99_mouse_init();
	texture_init();
	levels_init();
#ifdef DREAMCAST
	text_init("helvetica_dc.txf",40);
#else
	text_init("Helvetica-Bold.txf",40);
#endif
	load_options();
  write_options();
	score_list_init();
	load_theme(gameoptions.theme,0);
#ifdef DREAMCAST
  set_show_cursor(0);
	set_echo(0);
	set_keyblock(0);
	text_set_z(1);
#endif
	glDisable(GL_TEXTURE_2D);
	srand(time(0));
#ifdef DREAMCAST
	text_set_z(2);
#endif
#ifdef SDL
	SDL_ShowCursor(SDL_ENABLE);
#endif

#ifdef MACOS
	//status("Checking for updates");
	//check_updates();
#endif
#ifdef DREAMCAST
  load_font("hybrid.fnt");
#endif

//name_entry();

while(exitflag==0) {
	title_screen();
	if(exitflag==0) {
		switch(menu_select)
		{
			case 0:
				current_level = free_play;
				play_game();
				if(score>1000) {
					cnt=0;
					do {
						encrypt(genrand_int32()%26,(unsigned char *)build_code(score,squares,maxcombo,0),(unsigned char *)highcode);
						cnt++;
						if(cnt>20) {
							highcode[0]='\0';
							break;
						}
						//printf("%s\n",highcode);
					} while(invalid_code(highcode));		
					if(highcode[0]!='\0' && gameoptions.net && gameoptions.username[0]!='\0' && gameoptions.password[0]!='\0') {
						submit_code(highcode,gameoptions.username,gameoptions.password);
						highcode[0]='\0';
					}
				}
				level_stats();
				//name_entry(game_gt*1000);
				break;
			/*case 1:
				current_level = level_list_head;
				while(current_level!=NULL) {
					play_game();
					level_stats();
					if(check_win(game_gt) == 1) {
						current_level=current_level->next;
					} else {
						current_level=NULL;
					}	
				}
				current_level=free_play;
				break;*/
			case 1:
				high_scores();
				break;
			case 2:
				select_options();
				break;
			case 3:
				exitflag=1;
				break;
		}
	}
}
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
	
	for(float x=0; x<= 1; x+=0.01) {
		if(sys_render_begin()) {
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			glColor3f(1-x,1-x,1-x);
			glBegin(GL_QUADS);
			glVertex3f(0,0,0.9);
			glVertex3f(640,0,0.9);
			glVertex3f(640,480,0.9);
			glVertex3f(0,480,0.9);
			glEnd();
			sys_render_finish();
		}
		delay(0.01);
	}
	save_scores();
#ifdef DIRECTX
  stop_bgm();
#endif
#ifdef SDL
  SDL_Quit();
#endif
#ifdef DREAMCAST
	goat_exit();
#endif
	//c99_sys_shutdown();
	return 0;
}
