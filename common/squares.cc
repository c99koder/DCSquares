/***************************************************************************
 *            squares.cc
 *
 *  Sat Sep 25 00:53:25 2004
 *  Copyright  2004  chip
 *  chip@njit.lan
 ****************************************************************************/

#include <stdlib.h>
#include <math.h>
#ifdef WIN32
#define M_PI 3.141592653589793238
#include <windows.h>
#ifdef DIRECTX
#include <dxstdafx.h>

extern CSound *powerup;
extern CSound *powerdown;
extern CSound *collect;
extern CSound *gameover;
#endif
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
#if defined(DREAMCAST) || defined(SDL)
#include "input.h"
#endif
#include "squares.h"
#include "theme.h"
#include "hud.h"
#include "rand.h"
#include "level.h"
#ifdef DREAMCAST
#include <dcplib/sg.h>
#else
#include <sg.h>
#endif

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

squarelist *squarehead=NULL;
int powerup_mode=-1;
int dcs_sfx=1;

extern float power;
extern int score[MAX_PLAYERS],score_tex,enemy_tex;

extern int combo[MAX_PLAYERS];
extern int maxcombo[MAX_PLAYERS];
extern themeinfo_t themeinfo;
extern int score_tex;
extern int enemy_tex;
extern int invinc_tex;
extern int evil_tex;
extern int shadow_tex;

int square_count=0;

float limit(float val, float min, float max) {
  if(val<min) {
		return min;
	} else if(val > max) {
		return max;
	} else {
		return val;
	}
}

extern float effect_mode;
extern float effect_timer;
extern int effect_type;

void render_squares(float square_alpha) {
	squarelist *c=squarehead;
	float angle,x,y,dx,dy,l,i;

//#ifdef MACOS
	glViewport(themeinfo.game_x,480-(themeinfo.game_y+themeinfo.game_h),themeinfo.game_w,themeinfo.game_h);
//#else
//	glViewport(themeinfo.game_x,themeinfo.game_y,themeinfo.game_w,themeinfo.game_h);
//#endif
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	
	if(effect_type!=-1) {
		switch(effect_type) {
			case INVINC:
				glColor4f(0,1,0,0.4 * effect_timer);
				glBegin(GL_QUADS);
				glVertex3f(0,0,0.11);
				glVertex3f(640,0,0.11);
				glVertex3f(640,480,0.11);
				glVertex3f(0,480,0.11);
				glEnd();
				break;
			case SLOWMO:
				glColor4f(0,0,0,0.4 * effect_timer);
				glBegin(GL_QUADS);
				glVertex3f(0,0,0.11);
				glVertex3f(640,0,0.11);
				glVertex3f(640,480,0.11);
				glVertex3f(0,480,0.11);
				glEnd();
				break;
			case EVIL:
				glColor4f(1,0,0,0.4 * effect_timer);
				glBegin(GL_QUADS);
				glVertex3f(0,0,0.11);
				glVertex3f(640,0,0.11);
				glVertex3f(640,480,0.11);
				glVertex3f(0,480,0.11);
				glEnd();
				break;
			case SPEED:
				glColor4f(1,1,1,0.4 * effect_timer);
				glBegin(GL_QUADS);
				glVertex3f(0,0,0.11);
				glVertex3f(640,0,0.11);
				glVertex3f(640,480,0.11);
				glVertex3f(0,480,0.11);
				glEnd();
				break;
		}
	}	
	while(c!=NULL) {
		glLoadIdentity();
		for(i=(c->type>PLAYER_NET && (powerup_mode==SLOWMO || powerup_mode==SPEED))?4:0;i>=0;i--) {
			dx=0;
			dy=0;
			if(c->xv!=0) {
				dx=(i*10) * effect_timer;
				if(c->xv>0) dx*=-1;
			}
			if(c->yv!=0) {
				dy=(i*10) * effect_timer;
				if(c->yv>0) dy*=-1;
			}
			glLoadIdentity();
			glTranslatef(c->x+dx,c->y+dy,0);
			glRotatef(c->angle,0,0,1);
			
			glScalef(themeinfo.scale,themeinfo.scale,1.0);
			if(c->tex==-1) {
				if(c->type<POWERUP) {
					glBegin(GL_QUADS);
						glColor4f(limit(c->r+0.4,0,1),limit(c->g+0.4,0,1),limit(c->b+0.4,0,1),(i==0)?square_alpha:square_alpha*((float)(4-i)/8.0f));
						glVertex3f(-c->size,-c->size,0.1);
						glColor4f(limit(c->r+0.1,0,1),limit(c->g+0.1,0,1),limit(c->b+0.1,0,1),(i==0)?square_alpha:square_alpha*((float)(4-i)/8.0f));
						glVertex3f(c->size,-c->size,0.1);
						glColor4f(limit(c->r,0,1),limit(c->g,0,1),limit(c->b,0,1),(i==0)?square_alpha:square_alpha*((float)(4-i)/8.0f));
						glVertex3f(c->size,c->size,0.1);
						glColor4f(limit(c->r+0.1,0,1),limit(c->g+0.1,0,1),limit(c->b+0.1,0,1),(i==0)?square_alpha:square_alpha*((float)(4-i)/8.0f));
						glVertex3f(-c->size,c->size,0.1);
					glEnd();
				} else {
					glBegin(GL_POLYGON);
						for(int j =0;j<16;j++){
								angle = j*2*M_PI/16;
								x = cos(angle)*c->size;
								y = sin(angle)*c->size;
								dx = c->size - x;
								dy = c->size - y;
								l = (sqrt((dx*dx)+(dy*dy))/(c->size*4)) * 0.4;
								glColor4f(c->r+l, c->g+l, c->b+l,(i==0)?square_alpha:square_alpha*((float)(4-i)/8.0f));  
								glVertex3f(cos(angle)*c->size,sin(angle)*c->size,0.1);
						}
					glEnd();
				}
			} else {
				if(c->shadow_tex!=-1) {
					glLoadIdentity();
					glTranslatef(c->x+2+dx,c->y+2+dy,0.1);
					glRotatef(c->angle,0,0,1);
					glColor4f(c->r,c->g,c->b,square_alpha*((float)(8-i)/8.0f));
					glScalef(themeinfo.scale,themeinfo.scale,1.0);
					render_poly(c->size,c->shadow_tex,square_alpha*((float)(4-i)/4.0f));
				}
				glLoadIdentity();
				glTranslatef(c->x+dx,c->y+dy,0.2);
				glRotatef(c->angle,0,0,1);
				glColor4f(c->r,c->g,c->b,square_alpha);
				glScalef(themeinfo.scale,themeinfo.scale,1.0);
				render_poly(c->size,(c->tex==score_tex && powerup_mode==EVIL)?enemy_tex:c->tex,(powerup_mode==INVINC && c->tex==enemy_tex)?1.0f-(0.8f*effect_timer):(i==0)?square_alpha:square_alpha*((float)(4-i)/8.0f));
			}
		}
		c=c->next;
	}
	glScalef(1.0,1.0,1.0);
	glViewport(0,0,640,480);
	glLoadIdentity();
}

void update_squares(float s) {
	squarelist *c=squarehead;
	squarelist *p=NULL;
	squarelist *o=NULL;
	int lmb=0;
	float speedmod=1;
	int mx=320,my=240;

	square_count=0;

  c=squarehead;
	if(powerup_mode==SLOWMO) speedmod=0.5;
	if(powerup_mode==SPEED) speedmod=2;
	
	while(c!=NULL) {
	  square_count++;
		if(c->xv!=0) c->x+=((float)c->xv*speedmod*s*100);
		if(c->yv!=0) c->y+=((float)c->yv*speedmod*s*100);
		if(c->type<=PLAYER_NET) {
			c->angle+=180*s;
			if(c->angle>360) c->angle-=360;
		}
#if defined(SDL) || defined(DREAMCAST)
		if(c->type<PLAYER_NET) {
			mx=c->x;
			my=c->y;
			read_mouse(c->type,&mx,&my,&lmb);
			switch(poll_game_device(c->type)) {
				case MOVE_UP:
					my-=8;
					break;
				case MOVE_DOWN:
					my+=8;
					break;
				case MOVE_LEFT:
					mx-=8;
					break;
				case MOVE_RIGHT:
					mx+=8;
					break;
			}
			c->x=mx;
			c->y=my;			
			if(c->x<0) c->x=0;
			if(c->x>640) c->x=640;
			if(c->y<0) c->y=0;
			if(c->y>480) c->y=480;
		}
#endif
		if(c->type>PLAYER_NET) {
			if(c->x<0-c->size || c->x>640+c->size || c->y < 0-c->size || c->y > 480+c->size) {
				c->deleted=1;
        if(c->type==SCORE) {
					for(int p=0; p<current_level->players; p++) {
						if(combo[p] > maxcombo[p]) maxcombo[p]=combo[p];
						combo[p]=0;
					}
				}
			}
		}
		c=c->next;
	}
	
	c=squarehead;
	while(c!=NULL) {
		if(c->deleted==1) {
			if(p==NULL) {
				squarehead=squarehead->next;
				o=c;
				c=squarehead;
			} else {
				p->next=c->next;
				o=c;
				c=p;
			}
			delete o;
		}
		if(c!=NULL) {
			p=c;
			c=c->next;
		}
	}
}

squarelist *check_collide(squarelist *player) {
	squarelist *c=squarehead;
	sgBox b1,b2;
	b2.setMin(player->x-(player->size*themeinfo.scale),player->y-(player->size*themeinfo.scale),-1);
	b2.setMax(player->x+(player->size*themeinfo.scale),player->y+(player->size*themeinfo.scale),1);
	
	while(c!=NULL) {
		if(c!=player) {
			b1.setMin(c->x-(c->size*themeinfo.scale),c->y-(c->size*themeinfo.scale),-1);
			b1.setMax(c->x+(c->size*themeinfo.scale),c->y+(c->size*themeinfo.scale),1);
			if(b1.intersects(&b2)) {
				if(c!=NULL&&c->type>PLAYER_NET) {
					c->deleted=1;
					if(c->type==SCORE && powerup_mode!=EVIL) {
					//c->tm=gt;
					//sprintf(tmp,"2:g,%i,%i",c->id,c->tm);
						//net_sendpacket(tmp);
						give_points(player);
#ifdef DIRECTX
						if(collect) collect->Reset();
						if(collect) collect->Play(0,0);
#endif
					}
					if(powerup_mode!=INVINC && (c->type==ENEMY || (powerup_mode==EVIL && c->type==SCORE))) {
#ifdef DREAMCAST
						snd_sfx_play(gameover,250,128);
#endif
#ifdef SDL
						Mix_PlayChannel(-1,gameover,0);
#endif
#ifdef DIRECTX
						if(gameover) gameover->Play(0,0);
#endif
					}
					if(c->type>=10 && c->type < 14) {
#ifdef DREAMCAST
						snd_sfx_play(powerup,250,128);
#endif
#ifdef SDL
						Mix_PlayChannel(-1,powerup,0);
						//if(powerup_mode==SLOWMO) Mix_PlayMusic(bgm,-1);
						Mix_VolumeMusic(MIX_MAX_VOLUME);
#endif
#ifdef DIRECTX
						if(powerup) powerup->Reset();
						if(powerup) powerup->Play(0,0);
#endif
						powerup_mode=c->type-10;
						power=5;
						if(powerup_mode==PLUS1000) score[player->type]+=1000;
						if(powerup_mode==MINISQUARE) player->size=6;
						if(powerup_mode==SLOWMO) {
#ifdef SDL
							//Mix_PlayMusic(title,-1);
							Mix_VolumeMusic(MIX_MAX_VOLUME/8);
#endif
						}
					}
					if(c->type>=14) {
	#ifdef DREAMCAST
						snd_sfx_play(powerdown,250,128);
	#endif
	#ifdef SDL
						Mix_PlayChannel(-1,powerdown,0);
						if(powerup_mode==SLOWMO && (c->type-10 != SLOWMO)) Mix_VolumeMusic(MIX_MAX_VOLUME);
	#endif
#ifdef DIRECTX
						if(powerdown) powerdown->Reset();
						if(powerdown) powerdown->Play(0,0);
#endif

						powerup_mode=c->type-10;
						if(powerup_mode==MINUS1000) score[player->type]-=1000;
						if(powerup_mode==BIGSQUARE) player->size=12;
						power=5;
					}
					return c;
				}
			}
		}
		c=c->next;
	}
	return NULL;
}

void destroy_list() {
	squarelist *c=squarehead;
	squarelist *p=NULL;
	squarelist *o=NULL;
	
	while(c!=NULL) {
		if(p!=NULL) {
			p->next=c->next;
		} else {
			squarehead=c->next;
		}
		o=c;
		c=c->next;
		delete o;
	}
	
	squarehead=NULL;
}

float fade_color=0.0;
unsigned int square_pool=0;

void set_fade_color(float f) { fade_color=f; }

squarelist *get_square_by_id(int id) {
  squarelist *c=squarehead;
	
	while(c!=NULL) {
	  if(c->id==id) return c;
		c=c->next;
	}
	
	return NULL;
}

squarelist *create_square(int x, int y, int size, int type) {
	squarelist *c=new squarelist;

	c->x=x;
	c->y=y;
	c->xv=0;
	c->yv=0;
	if(type<POWERUP) {
		c->size=size;
	} else {
		c->size=size+2;
	}
	c->angle=0;
	c->type=type;
	c->tm=0;
	c->id=square_pool++;
	c->deleted=0;
	c->tex=-1;
	c->shadow_tex=-1;
	switch(type) {
		case SCORE:
			if(themeinfo.score[0]!='\0') c->tex=score_tex;
			c->r=(float)themeinfo.good_r/255.0f; c->g=(float)themeinfo.good_g/255.0f; c->b=(float)themeinfo.good_b/255.0f;
			break;
		case ENEMY:
			if(themeinfo.enemy[0]!='\0') c->tex=enemy_tex;
			c->r=(float)themeinfo.evil_r/255.0f; c->g=(float)themeinfo.evil_g/255.0f; c->b=(float)themeinfo.evil_b/255.0f;
			break;
		case POWERDOWN:
			c->r=(float)themeinfo.evil_r/255.0f; c->g=(float)themeinfo.evil_g/255.0f; c->b=(float)themeinfo.evil_b/255.0f;
			c->type=14+(genrand_int32()%4);
			if(themeinfo.evil[0]!='\0') c->tex=evil_tex+(c->type-14);
			if(themeinfo.powershadow[0]!='\0') c->shadow_tex=shadow_tex;
			c->size=10;
			break;
		case POWERUP:
			c->r=(float)themeinfo.good_r/255.0f; c->g=(float)themeinfo.good_g/255.0f; c->b=(float)themeinfo.good_b/255.0f;
			c->type=10+(genrand_int32()%4);
			if(themeinfo.invinc[0]!='\0') c->tex=invinc_tex+(c->type-10);
			if(themeinfo.powershadow[0]!='\0') c->shadow_tex=shadow_tex;
			c->size=10;
		default:
			c->r=(float)themeinfo.good_r/255.0f; c->g=(float)themeinfo.good_g/255.0f; c->b=(float)themeinfo.good_b/255.0f;
			break;
	}
	//c->tex=-1;
	c->next=NULL;
	
	if((c->type-10 == INVINC && current_level->power_evil==0) ||
		 (c->type-10 == SLOWMO && current_level->power_speed==0) ||
		 (c->type-10 == MINISQUARE && current_level->power_size==0) ||
		 (c->type-10 == PLUS1000 && current_level->power_score==0) ||
		 (c->type-10 == EVIL && current_level->power_evil==0) ||
		 (c->type-10 == SPEED && current_level->power_speed==0) ||
		 (c->type-10 == MINUS1000 && current_level->power_score==0) ||
		 (c->type-10 == BIGSQUARE && current_level->power_size==0)) {
		delete c;
		return NULL;
	}
	
	if(squarehead==NULL) {
		squarehead=c; 
	} else {
		c->next=squarehead;
		squarehead=c;
	}
	
	return c;
}
