/***************************************************************************
 *            squares.cc
 *
 *  Sat Sep 25 00:53:25 2004
 *  Copyright  2004  chip
 *  chip@njit.lan
 ****************************************************************************/

#ifdef TIKI
#include <Tiki/tiki.h>
#include <Tiki/gl.h>
#include <Tiki/vector.h>
#include <Tiki/texture.h>

using namespace Tiki;
using namespace Tiki::Math;
using namespace Tiki::GL;
#endif
#include <stdlib.h>
#include <math.h>

#include "squares.h"
#include "theme.h"
#include "hud.h"
#include "rand.h"
#include "level.h"

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
extern int score[MAX_PLAYERS];
extern int combo[MAX_PLAYERS];
extern int maxcombo[MAX_PLAYERS];
extern themeinfo_t themeinfo;
extern RefPtr<Texture> game_tex;
extern RefPtr<Texture> score_tex;
extern RefPtr<Texture> enemy_tex;
extern RefPtr<Texture> invinc_tex;
extern RefPtr<Texture> slow_tex; 
extern RefPtr<Texture> mini_tex;
extern RefPtr<Texture> plus_tex;
extern RefPtr<Texture> evil_tex;
extern RefPtr<Texture> speed_tex;
extern RefPtr<Texture> minus_tex;
extern RefPtr<Texture> big_tex;
extern RefPtr<Texture> shadow_tex;

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

void render_squares(float square_alpha, bool game) {
	squarelist *c=squarehead;
	float angle,x,y,dx,dy,l,i;

	if(game)
		glViewport(themeinfo.game_x,480-(themeinfo.game_y+themeinfo.game_h),themeinfo.game_w,themeinfo.game_h);
	else
		glViewport(themeinfo.title_x,480-(themeinfo.title_y+themeinfo.title_h),themeinfo.title_w,themeinfo.title_h);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	
	render_bg(game_tex,1);
	
	if(effect_type!=-1) {
		switch(effect_type) {
			case INVINC:
				glColor4f(0,1,0,0.4 * effect_timer);
				glBegin(GL_QUADS);
				glVertex3f(0,0,0.0021);
				glVertex3f(640,0,0.0021);
				glVertex3f(640,480,0.0021);
				glVertex3f(0,480,0.0021);
				glEnd();
				break;
			case SLOWMO:
				glColor4f(0,0,0,0.4 * effect_timer);
				glBegin(GL_QUADS);
				glVertex3f(0,0,0.0021);
				glVertex3f(640,0,0.0021);
				glVertex3f(640,480,0.0021);
				glVertex3f(0,480,0.0021);
				glEnd();
				break;
			case EVIL:
				glColor4f(1,0,0,0.4 * effect_timer);
				glBegin(GL_QUADS);
				glVertex3f(0,0,0.0021);
				glVertex3f(640,0,0.0021);
				glVertex3f(640,480,0.0021);
				glVertex3f(0,480,0.0021);
				glEnd();
				break;
			case SPEED:
				glColor4f(1,1,1,0.4 * effect_timer);
				glBegin(GL_QUADS);
				glVertex3f(0,0,0.0021);
				glVertex3f(640,0,0.0021);
				glVertex3f(640,480,0.0021);
				glVertex3f(0,480,0.0021);
				glEnd();
				break;
		}
	}	
	while(c!=NULL) {
		if(c->deleted==1) { c=c->next; continue; }
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
			if(c->tex==NULL) {
				if(c->type<POWERUP) {
					glBegin(GL_QUADS);
						glColor4f(limit(c->r+0.4,0,1),limit(c->g+0.4,0,1),limit(c->b+0.4,0,1),(i==0)?square_alpha:square_alpha*((float)(4-i)/8.0f));
						glVertex3f(-c->size,-c->size,0.011);
						glColor4f(limit(c->r,0,1),limit(c->g,0,1),limit(c->b,0,1),(i==0)?square_alpha:square_alpha*((float)(4-i)/8.0f));
						glVertex3f(c->size,-c->size,0.011);
						glColor4f(limit(c->r-0.2,0,1),limit(c->g-0.2,0,1),limit(c->b-0.2,0,1),(i==0)?square_alpha:square_alpha*((float)(4-i)/8.0f));
						glVertex3f(c->size,c->size,0.011);
						glColor4f(limit(c->r,0,1),limit(c->g,0,1),limit(c->b,0,1),(i==0)?square_alpha:square_alpha*((float)(4-i)/8.0f));
						glVertex3f(-c->size,c->size,0.011);
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
								glVertex3f(cos(angle)*c->size,sin(angle)*c->size,0.011);
						}
					glEnd();
				}
			} else {
				if(c->shadow_tex!=NULL) {
//printf("Drawing shadow\n");
					glLoadIdentity();
					glTranslatef(c->x+2+dx,c->y+2+dy,0.011);
					glRotatef(c->angle,0,0,1);
					glColor4f(c->r,c->g,c->b,square_alpha*((float)(8-i)/8.0f));
					glScalef(themeinfo.scale,themeinfo.scale,1.0);
					render_poly(c->size,c->shadow_tex,square_alpha*((float)(4-i)/4.0f));
				}
//printf("Drawing type: %i\n",c->type);
				glLoadIdentity();
				glTranslatef(c->x+dx,c->y+dy,0.012);
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
		if(c->deleted==1) { c=c->next; continue; }
	  square_count++;
		if(c->xv!=0) c->x+=((float)c->xv*speedmod*s*100);
		if(c->yv!=0) c->y+=((float)c->yv*speedmod*s*100);
		if(c->type<=PLAYER_NET) {
			c->angle+=180*s;
			if(c->angle>360) c->angle-=360;
		}
		if(c->type<PLAYER_NET) {
#ifdef LINUX
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
				case QUIT_BTN:
					//exit(0);
					break;
			}
			c->x=mx;
			c->y=my;			
#endif
			if(c->x<0) c->x=0;
			if(c->x>640) c->x=640;
			if(c->y<0) c->y=0;
			if(c->y>480) c->y=480;
		}
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
	Vector min1,max1,min2,max2;

	min2=Vector(player->x-(player->size*themeinfo.scale),player->y-(player->size*themeinfo.scale),-1);
	max2=Vector(player->x+(player->size*themeinfo.scale),player->y+(player->size*themeinfo.scale),1);

	while(c!=NULL) {
		if(c->deleted==1) { c=c->next; continue; }

		if(c!=player) {
			min1=Vector(c->x-(c->size*themeinfo.scale),c->y-(c->size*themeinfo.scale),-1);
			max1=Vector(c->x+(c->size*themeinfo.scale),c->y+(c->size*themeinfo.scale),1);
			if(min1.x <= max2.x && max1.x >= min2.x &&
			min1.y <= max2.y && max1.y >= min2.y) {
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
	squarelist *o=NULL;
	
	while(c!=NULL) {
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
	c->tex=NULL;
	c->shadow_tex=NULL;
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
			if(themeinfo.evil[0]!='\0') {
				switch(c->type-10) {
				case EVIL:
					c->tex=evil_tex;
					break;
				case SPEED:
					c->tex=speed_tex;
					break;
				case MINUS1000:
					c->tex=minus_tex;
					break;
				case BIGSQUARE:
					c->tex=big_tex;
					break;
				}
			}
			if(themeinfo.powershadow[0]!='\0') c->shadow_tex=shadow_tex;
			c->size=10;
			break;
		case POWERUP:
			c->r=(float)themeinfo.good_r/255.0f; c->g=(float)themeinfo.good_g/255.0f; c->b=(float)themeinfo.good_b/255.0f;
			c->type=10+(genrand_int32()%4);
			if(themeinfo.invinc[0]!='\0') {
				switch(c->type-10) {
				case INVINC:
					c->tex=invinc_tex;
					break;
				case SLOWMO:
					c->tex=slow_tex;
					break;
				case PLUS1000:
					c->tex=plus_tex;
					break;
				case MINISQUARE:
					c->tex=mini_tex;
					break;
				}
			}
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
