#ifdef WIN32
#include <windows.h>
#endif
#ifdef DREAMCAST
#include <kos.h>
#endif
#ifdef MACOS
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#ifdef SDL
#include <SDL/SDL.h>
#ifdef MACOS
#include <SDL_mixer/SDL_mixer.h>
#else
#include <SDL/SDL_mixer.h>
#endif
#endif
#include <stdio.h>
#include <math.h>
#include "text.h"
#include "theme.h"
#include "squares.h"
#include "game.h"
#include "score.h"
#if defined(SDL) || defined(DREAMCAST)
#include "sys.h"
#include "input.h"
#endif

extern themeinfo_t themeinfo;
extern char highcode[20];
extern int highscore;
int combo=0;
int score=0;
int squares=0;
int scoreval=20;
int maxcombo=0;
float tickval=0.5;
float power=0;
float speedval=1.0;
extern int powerup_mode;
extern int bg_tex;

#ifdef SDL
extern Mix_Chunk *powerup;
extern Mix_Chunk *powerdown;
extern Mix_Chunk *collect;
extern Mix_Chunk *gameover;
extern Mix_Music *bgm;
extern Mix_Music *title;
#endif

#ifdef DREAMCAST
extern sfxhnd_t powerup;
extern sfxhnd_t powerdown;
extern sfxhnd_t collect;  
extern sfxhnd_t gameover;
#endif

float effect_timer=0;
float effect_mode=0;
int effect_type=-1;

void give_points(squarelist *player) {
#ifdef DREAMCAST
	snd_sfx_play(collect,250,128);
#endif
#ifdef SDL
	Mix_PlayChannel(-1,collect,0);
#endif
#ifdef WIN32
	//collect->Play(0,0);
#endif
	squares++;

	if(squares%5==0) {
		scoreval+=20+int(60.0f*(1.0f-tickval));
		if(scoreval > 1000) scoreval = 1000;
		tickval-=0.025;
		if(tickval<0.2) tickval=0.2;
		speedval+=0.05;
		if(speedval>6) speedval=6;
		player->size++;
		if(player->size>10) player->size=10;
	}
	if(player->type==PLAYER1) {
		combo++;
		score+=(scoreval);
		if(combo>4) score+=(2*combo);
	}
}

void render_bar(float power, float max) {
	float w=(power/max) * 300.0f;
	char msg[256];
	float center_alpha;

    glDisable(GL_TEXTURE_2D);
	  glLoadIdentity();

	if((max-power)/max < 1.0f/8.0f) {
		center_alpha=(max-power)/(max/8.0f);
		if(effect_mode==0) {
			effect_mode=1;
			effect_timer=0;
			effect_type=powerup_mode;
		}
	} else if((max-power)/max >= 7.0f/8.0f) {
		center_alpha=(power/max) * 8.0f;
		if(effect_mode==0) {
			effect_mode=-1;
			effect_timer=1;
		}
	} else {
		center_alpha=1.0;
	}
	
	
	//printf("%f (%f/%f)\n",f,power,max);
	switch(powerup_mode) {
		case INVINC:
			sprintf(msg,"Invincibility");
			glColor4f(themeinfo.good_r,themeinfo.good_g,themeinfo.good_b,0.6f);
			break;
		case SLOWMO:
			sprintf(msg,"Slow Motion");
			glColor4f(themeinfo.good_r,themeinfo.good_g,themeinfo.good_b,0.6f);
			break;
		case MINISQUARE:
			sprintf(msg,"Mini Square");
			glColor4f(themeinfo.good_r,themeinfo.good_g,themeinfo.good_b,0.4f);
			break;
		case PLUS1000:
			sprintf(msg,"+1000 Points");
			glColor4f(themeinfo.good_r,themeinfo.good_g,themeinfo.good_b,0.4f);
			break;
		case EVIL:
			sprintf(msg,"Evil Squares");
			glColor4f(themeinfo.evil_r,themeinfo.evil_g,themeinfo.evil_b,0.6f);
			break;
		case SPEED:
			sprintf(msg,"Speed Up");
			glColor4f(themeinfo.evil_r,themeinfo.evil_g,themeinfo.evil_b,0.6f);
			break;
		case BIGSQUARE:
			sprintf(msg,"Big Square");
			glColor4f(themeinfo.evil_r,themeinfo.evil_g,themeinfo.evil_b,0.4f);
			break;
		case MINUS1000:
			sprintf(msg,"-1000 Points");
			glColor4f(themeinfo.evil_r,themeinfo.evil_g,themeinfo.evil_b,0.4f);
			break;
		}
  if(powerup_mode <MINISQUARE || 
    (powerup_mode>PLUS1000 && powerup_mode<MINUS1000)) {
	  glBegin(GL_QUADS);
	  glVertex3f(123,100,0.2);
	  glVertex3f(123+w,100,0.2);
	  glVertex3f(123+w,110,0.2);
	  glVertex3f(123,110,0.2);
	  glEnd();
  }
	center_shad_rgb(115,msg,16,center_alpha,1,1,1);
}

void render_score(float gt) {
	static float oldgt=gt;
	static float oldpt=gt;
	static float oldet=gt;
	
	char tmp[100];
	
	if(gt<oldgt) oldgt=gt;
	if(gt<oldpt) oldpt=gt;
	if(gt<oldet) oldet=gt;
	
	if(effect_mode!=0) {
		if(effect_mode>0 && effect_timer < 1) {
			effect_timer += (gt-oldet)*1.5f;
			if(effect_timer >= 1) {
				effect_timer=1;
				effect_mode=0;
			}
		}
		if(effect_mode<0 && effect_timer > 0) {
			effect_timer -= (gt-oldet)*1.5f;
			if(effect_timer <= 0) {
				effect_timer=0;
				effect_mode=0;
			}
		}
	}
	
	oldet=gt;
	
	if(power>0) {
		render_bar(power,5);
		power-=(gt-oldpt);
		//printf("%f %f\n",power,(gt-oldpt));
		if(power<=0) {
		 if(powerup_mode==SLOWMO) {
#ifdef SDL
			//Mix_PlayMusic(bgm,-1);
			Mix_VolumeMusic(MIX_MAX_VOLUME);
#endif
		}
		}
	} else {
		powerup_mode=-1;
		effect_type=-1;
	}
	oldpt=gt;
	sprintf(tmp,"%i",score);
	center(themeinfo.score_x,themeinfo.score_y,tmp,themeinfo.score_size,1.0);
	sprintf(tmp,"%i%s",squares,themeinfo.squares_caption);
	center(themeinfo.squares_x,themeinfo.squares_y,tmp,themeinfo.squares_size,1.0);
	if(combo>4) {
		sprintf(tmp,"%i%s",combo,themeinfo.combo_caption);
		center(themeinfo.combo_x,themeinfo.combo_y,tmp,themeinfo.combo_size,1.0);
	}

	if(gt-oldgt>tickval) {
		score++;
		oldgt=gt;
	}
}

void render_title(float gt) {
	char tmp[100];
	static float alpha=1.0f;
	static float bstep=-0.01f;
	static float st=0;

	if(highcode[0]!='\0') {
		sprintf(tmp,"%i",highscore);
		center_shad(100,tmp,20,1);
		sprintf(tmp,"%s",highcode);
		center_shad(120,tmp,16,1);
	}
#if !defined(SDL) && !defined(DREAMCAST)
	center_shad_rgb(310,"Click to Begin",20,alpha,1,1,1);
#endif
	set_font_size(12);
	draw_txt(640-txt_width("Version 1.8"),480,"Version 1.8",0,0,0,1,12);
	set_font_size(16);
	center_shad(355,"Programming: Sam Steele",18,1);
	sprintf(tmp,"Artwork: %s",themeinfo.bg_auth);
	center_shad(375,tmp,18,1);
	sprintf(tmp,"Music: %s",themeinfo.music_auth);
	center_shad(395,tmp,18,1);
	sprintf(tmp,"Sound effects: %s",themeinfo.sfx_auth);
	center_shad(415,tmp,18,1);
	center_shad(440,"http://dcsquares.c99.org",18,1);
	st+=gt;
	if(st>0.015) {
		alpha+=bstep;
		if(alpha>=1 || alpha <= 0.6) bstep *= -1;
		st=0;
	}
}

int rank=-1;

void render_highscores() {
	int x;
	struct score_list_node *current=score_list_head;
	char tmp[100];
	
	center_shad_rgb(100,"High Scores",28,1,1,1,1);

	set_font_size(20);	
	
	draw_txt(85,180-28,"Name",1,1,1,1,20);
	draw_txt(280-(txt_width("999999")/2)-(txt_width("Score")/2),180-28,"Score",1,1,1,1,20);
	draw_txt(360-(txt_width("99")/2)-(txt_width("Combo")/2),180-28,"Combo",1,1,1,1,20);
	draw_txt(490-(txt_width("999.99")/2)-(txt_width("Time")/2),180-28,"Time",1,1,1,1,20);
	
	for(x=0; x<10; x++) {
		sprintf(tmp,"%i.",x+1);
#ifdef DREAMCAST
		draw_txt(80-txt_width(tmp),x*22+180,tmp,1,1,1,1,20);
#else
		draw_txt(85-txt_width(tmp),x*22+180,tmp,1,1,1,1,20);
#endif
		if(current!=NULL) {
			draw_txt(80,x*22+180,current->name,(rank==x+1)?1:0.03,(rank==x+1)?1:0.58,1,1,20);
			sprintf(tmp,"%i",current->score);
			draw_txt(280-txt_width(tmp),x*22+180,tmp,(rank==x+1)?1:0.03,(rank==x+1)?1:0.58,1,1,20);
			sprintf(tmp,"%i",current->combo);
			draw_txt(360-txt_width(tmp),x*22+180,tmp,(rank==x+1)?1:0.03,(rank==x+1)?1:0.58,1,1,20);
			sprintf(tmp,"%.2f",current->time/1000.0f);
			draw_txt(490-txt_width(tmp),x*22+180,tmp,(rank==x+1)?1:0.03,(rank==x+1)?1:0.58,1,1,20);
			current=current->next;
		} else {
			draw_txt(80,x*22+180,"---",0.03,0.58,1,1,20);
			draw_txt(280-txt_width("0"),x*22+180,"0",0.03,0.58,1,1,20);
			draw_txt(360-txt_width("0"),x*22+180,"0",0.03,0.58,1,1,20);
			draw_txt(490-txt_width("0.00"),x*22+180,"0.00",0.03,0.58,1,1,20);
		}
	}

	center_shad_rgb(420,"http://dcsquares.c99.org/",16,1,1,1,1);
}

void high_scores();

void name_entry(unsigned long time) {
#if 0
	char L1[2]="_";
	char L2[2]="_";
	char L3[2]="_";
	static char name[4]="   ";
	int pos=0,loop=1,x,ox;
	float r,g,b;
	char buf[100];
	
	power=0;
	
	rank=score_list_rank(score);
	if(rank>10) return;
	
	while(loop==1) {
		x=poll_game_device(0);
		switch(x) {
			case MOVE_UP:
				name[pos]++;
				if(name[pos]==' '+1) name[pos]='A';
				if(name[pos]>'Z') name[pos]=' ';
				break;
			case MOVE_DOWN:
				name[pos]--;
				if(name[pos]==' '-1) name[pos]='Z';
				if(name[pos]<'A') name[pos]=' ';
				break;
			case START_BTN:
				loop=0;
				break;
			case QUIT_BTN:
				name[pos]=' ';
			case MOVE_LEFT:
				pos--;
				if(pos<0) pos=0;
				break;
			case FIRE_BTN:
				if(ox==FIRE_BTN) break;
				pos++;
				if(pos>2) {
					loop=0;
				} else {
					if(name[pos]==' ') name[pos]='A';
				}
				break;			
			case MOVE_RIGHT:
				pos++;
				if(pos>2) pos=2;
				break;
		}
		
		ox=x;
				
		L1[0]=name[0];
		L2[0]=name[1];
		L3[0]=name[2];
		
		if(sys_render_begin()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_bg(bg_tex,1);
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
			render_score(0);
			center_shad_rgb(100,"Name Entry",28,1,1,1,1);
			set_font_size(18);	

			sprintf(buf,"Congratulations!  Your score is rank #%i!",rank);
			center_shad_rgb(178,buf,18,1,1,1,1);
			center_shad_rgb(200,"Please enter your initials below:",18,1,1,1,1);

			draw_txt(200,300,"_",(pos==0)?1:0.03,(pos==0)?1:0.58,1,1,28);
			draw_txt(260,300,"_",(pos==1)?1:0.03,(pos==1)?1:0.58,1,1,28);
			draw_txt(320,300,"_",(pos==2)?1:0.03,(pos==2)?1:0.58,1,1,28);
			draw_txt(206,300,"_",(pos==0)?1:0.03,(pos==0)?1:0.58,1,1,28);
			draw_txt(266,300,"_",(pos==1)?1:0.03,(pos==1)?1:0.58,1,1,28);
			draw_txt(326,300,"_",(pos==2)?1:0.03,(pos==2)?1:0.58,1,1,28);
			draw_txt(200,300,L1,(pos==0)?1:0.03,(pos==0)?1:0.58,1,1,28);
			draw_txt(260,300,L2,(pos==1)?1:0.03,(pos==1)?1:0.58,1,1,28);
			draw_txt(320,300,L3,(pos==2)?1:0.03,(pos==2)?1:0.58,1,1,28);

			center_shad_rgb(380,"Use up and down to select a letter.",16,1,1,1,1);
			center_shad_rgb(400,"Use left and right to move.",16,1,1,1,1);
			center_shad_rgb(420,"Press Start when finished.",16,1,1,1,1);
			sys_render_finish();
		}
		if(x>0) delay((x==MOVE_LEFT || x==MOVE_RIGHT)?0.20:0.15);
	}
	score_list_insert(name,score,maxcombo,time,0);
	save_scores();
	high_scores();
#endif
}