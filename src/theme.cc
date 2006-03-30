
#ifdef TIKI
#include <Tiki/tiki.h>
#include <Tiki/gl.h>
#include <Tiki/texture.h>

using namespace Tiki;
using namespace Tiki::GL;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "squares.h"
#include "theme.h"

RefPtr<Texture> loading_tex;
RefPtr<Texture> logo_tex;
RefPtr<Texture> game_tex;
RefPtr<Texture> stat_tex;
RefPtr<Texture> bg_tex;
RefPtr<Texture> score_tex;
RefPtr<Texture> enemy_tex;
RefPtr<Texture> invinc_tex;
RefPtr<Texture> slow_tex; 
RefPtr<Texture> mini_tex;
RefPtr<Texture> plus_tex;
RefPtr<Texture> evil_tex;
RefPtr<Texture> speed_tex;
RefPtr<Texture> minus_tex;
RefPtr<Texture> big_tex;
RefPtr<Texture> shadow_tex;

char dcs_theme[100];
themeinfo_t themeinfo = {
	"None",
	"None",
	"None",
	"None",
	0,0,0,
	192,0,0,
	0,0,0
};

void MessageBox(char *title, char *msg);

void render_bg(Texture *tex, float fade) {
  GLfloat uv[4]={0,0,1,1};

	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();	
	glColor3f(fade,fade,fade);
	tex->select();
	glBegin(GL_QUADS);
	glTexCoord2f(0,uv[0]);
	glVertex3f(0,0,0.0);
	glTexCoord2f(1,uv[1]);
	glVertex3f(640,0,0.0);
	glTexCoord2f(1,uv[2]);
	glVertex3f(640,480,0.0);
	glTexCoord2f(0,uv[3]);
	glVertex3f(0,480,0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void render_box(int x, int y, int w, int h, Texture *tex, float fade) {
  GLfloat uv[4]={0,0,1,1};

	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();	
	glColor4f(1,1,1,fade);
	tex->select();
	glBegin(GL_QUADS);
	glTexCoord2f(0,uv[0]);
	glVertex3f(x,y,0.025);
	glTexCoord2f(1,uv[1]);
	glVertex3f(x+w,y,0.025);
	glTexCoord2f(1,uv[2]);
	glVertex3f(x+w,y+h,0.025);
	glTexCoord2f(0,uv[3]);
	glVertex3f(x,y+h,0.025);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void render_bg_game(Texture *tex, float fade) {
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();	
	glColor3f(fade,fade,fade);
	tex->select();
	glBegin(GL_QUADS);

	glTexCoord2f(0,0);
	glVertex3f(0,0,0.9);
	glTexCoord2f((float)themeinfo.game_x/640.0f,0);
	glVertex3f((float)themeinfo.game_x,0,0.9);
	glTexCoord2f((float)themeinfo.game_x/640.0f,1);
	glVertex3f((float)themeinfo.game_x,480,0.9);
	glTexCoord2f(0,1);
	glVertex3f(0,480,0.9);

	glTexCoord2f((float)themeinfo.game_x/640.0f,0);
	glVertex3f((float)themeinfo.game_x,0,0.9);
	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,0);
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,0,0.9);
	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,(float)themeinfo.game_y/480.0f);
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,(float)themeinfo.game_y,0.9);
	glTexCoord2f((float)themeinfo.game_x/640.0f,(float)themeinfo.game_y/480.0f);
	glVertex3f((float)themeinfo.game_x,(float)themeinfo.game_y,0.9);

	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,0);
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,0,0.02);
	glTexCoord2f(1,0);
	glVertex3f(640,0,0.02);
	glTexCoord2f(1,(((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f));
	glVertex3f(640,(float)themeinfo.game_y+(float)themeinfo.game_h,0.02);
	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,(((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f));
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,(float)themeinfo.game_y+(float)themeinfo.game_h,0.02);

	glTexCoord2f((float)themeinfo.game_x/640.0f,(((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f));
	glVertex3f((float)themeinfo.game_x,(float)themeinfo.game_y+(float)themeinfo.game_h,0.9);
	glTexCoord2f(1,(((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f));
	glVertex3f(640,(float)themeinfo.game_y+(float)themeinfo.game_h,0.9);
	glTexCoord2f(1,1);
	glVertex3f(640,480,0.9);
	glTexCoord2f((float)themeinfo.game_x/640.0f,1);
	glVertex3f((float)themeinfo.game_x,480,0.9);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void render_bg_title(Texture *tex, float fade) {
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();	
	glColor3f(fade,fade,fade);
	tex->select();
	glBegin(GL_QUADS);

	glTexCoord2f(0,0);
	glVertex3f(0,0,0.9);
	glTexCoord2f((float)themeinfo.title_x/640.0f,0);
	glVertex3f((float)themeinfo.title_x,0,0.9);
	glTexCoord2f((float)themeinfo.title_x/640.0f,1);
	glVertex3f((float)themeinfo.title_x,480,0.9);
	glTexCoord2f(0,1);
	glVertex3f(0,480,0.9);
	
	glTexCoord2f((float)themeinfo.title_x/640.0f,0);
	glVertex3f((float)themeinfo.title_x,0,0.9);
	glTexCoord2f(((float)themeinfo.title_x+(float)themeinfo.title_w)/640.0f,0);
	glVertex3f((float)themeinfo.title_x+(float)themeinfo.title_w,0,0.9);
	glTexCoord2f(((float)themeinfo.title_x+(float)themeinfo.title_w)/640.0f,(float)themeinfo.title_y/480.0f);
	glVertex3f((float)themeinfo.title_x+(float)themeinfo.title_w,(float)themeinfo.title_y,0.9);
	glTexCoord2f((float)themeinfo.title_x/640.0f,(float)themeinfo.title_y/480.0f);
	glVertex3f((float)themeinfo.title_x,(float)themeinfo.title_y,0.9);
	
	glTexCoord2f(((float)themeinfo.title_x+(float)themeinfo.title_w)/640.0f,0);
	glVertex3f((float)themeinfo.title_x+(float)themeinfo.title_w,0,0.9);
	glTexCoord2f(1,0);
	glVertex3f(640,0,0.9);
	glTexCoord2f(1,(((float)themeinfo.title_y+(float)themeinfo.title_h)/480.0f));
	glVertex3f(640,(float)themeinfo.title_y+(float)themeinfo.title_h,0.9);
	glTexCoord2f(((float)themeinfo.title_x+(float)themeinfo.title_w)/640.0f,(((float)themeinfo.title_y+(float)themeinfo.title_h)/480.0f));
	glVertex3f((float)themeinfo.title_x+(float)themeinfo.title_w,(float)themeinfo.title_y+(float)themeinfo.title_h,0.9);
	
	glTexCoord2f((float)themeinfo.title_x/640.0f,(((float)themeinfo.title_y+(float)themeinfo.title_h)/480.0f));
	glVertex3f((float)themeinfo.title_x,(float)themeinfo.title_y+(float)themeinfo.title_h,0.9);
	glTexCoord2f(1,(((float)themeinfo.title_y+(float)themeinfo.title_h)/480.0f));
	glVertex3f(640,(float)themeinfo.title_y+(float)themeinfo.title_h,0.9);
	glTexCoord2f(1,1);
	glVertex3f(640,480,0.9);
	glTexCoord2f((float)themeinfo.title_x/640.0f,1);
	glVertex3f((float)themeinfo.title_x,480,0.9);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void render_poly(int size, Texture *tex, float fade) {
  GLfloat uv[4]={0,0,1,1};

	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,fade);
	tex->select();
	glBegin(GL_QUADS);
	glTexCoord2f(0,uv[0]);
	glVertex3f(-size,-size,0.0);
	glTexCoord2f(1,uv[1]);
	glVertex3f(size,-size,0.0);
	glTexCoord2f(1,uv[2]);
	glVertex3f(size,size,0.0);
	glTexCoord2f(0,uv[3]);
	glVertex3f(-size,size,0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

#ifdef DREAMCAST
sfxhnd_t powerup=NULL;
sfxhnd_t powerdown=NULL;
sfxhnd_t collect=NULL;
sfxhnd_t gameover=NULL;

extern "C" {
void update_lcds();
}
#endif

char *theme_dir(char *filename) {
  static char buf[256];
#ifdef WIN32
  sprintf(buf,"themes\\%s\\%s",dcs_theme,filename);
#else
  sprintf(buf,"themes/%s/%s",dcs_theme,filename);
#endif
  return buf;
}

int load_theme(char *theme, int sfx) {
    FILE *f;
	char buf[200];
	char buf2[200];
	char *p=NULL;
	char *val;

	if(theme!=NULL) strcpy(dcs_theme,theme); else strcpy(dcs_theme,"goat");

	themeinfo.scale=2;
	themeinfo.text_r=100;
	themeinfo.text_g=100;
	themeinfo.text_b=100;
	themeinfo.game_x=0;
	themeinfo.game_y=0;
	themeinfo.game_w=640;
	themeinfo.game_h=480;
	themeinfo.title_x=0;
	themeinfo.title_y=0;
	themeinfo.title_w=640;
	themeinfo.title_h=480;
	themeinfo.logo_w=0;
	themeinfo.stat_w=-1;
	themeinfo.stat_h=-1;
	themeinfo.invinc[0]='\0';
	themeinfo.plus1000[0]='\0';
	themeinfo.slowdown[0]='\0';
	themeinfo.minisquare[0]='\0';
	themeinfo.score[0]='\0';
	themeinfo.powershadow[0]='\0';
	themeinfo.bigsquare[0]='\0';
	themeinfo.minus1000[0]='\0';
	themeinfo.speedup[0]='\0';
	themeinfo.evil[0]='\0';
	themeinfo.enemy[0]='\0';
	themeinfo.score_x[0]=320;
	themeinfo.score_y[0]=240;
	themeinfo.score_size=26;
	themeinfo.score_title[0]='\0';
	themeinfo.score_caption[0]='\0';
	strcpy(themeinfo.squares_caption," squares");
	themeinfo.squares_x[0]=320;
	themeinfo.squares_y[0]=260;
	themeinfo.squares_size=20;
	themeinfo.squares_title[0]='\0';
	strcpy(themeinfo.combo_caption," combo");
	themeinfo.combo_x[0]=320;
	themeinfo.combo_y[0]=280;
	themeinfo.combo_size=16;
	themeinfo.combo_title[0]='\0';
	themeinfo.time_x=320;
	themeinfo.time_y=300;
	themeinfo.time_size=16;
	themeinfo.player_x[0]=-800;
	themeinfo.player_y[0]=-800;
	
  f=fopen(theme_dir("theme.ini"),"r");
	if(!f) {
		if(strcmp("goat",theme)) {
			return load_theme("goat",sfx);
		} else {
#ifdef WIN32
			MessageBox(NULL,"Cannot load default theme","Error",MB_OK|MB_ICONHAND);
#endif
#ifdef MACOS
			//MessageBox("Error","Cannot load default theme");
#endif
#ifdef TIKI
			printf("Cannot load default theme\n");
			exit(-1);
#endif
			return -1;
		}
	}
	while(fgets(buf,200,f)!=NULL) {
	  p=NULL;
	  strtok(buf,"=");
	  //printf("Name: %s\n",buf);
	  val=strtok(NULL,"\n");
	  //printf("Val: %s\n",val);
	  if(!strcmp(buf,"bg_auth")) {
			p=themeinfo.bg_auth;
	  }
	  if(!strcmp(buf,"sfx_auth")) {
			p=themeinfo.sfx_auth;
	  }
		if(!strcmp(buf,"music_auth")) {
			p=themeinfo.music_auth;
		}
		if(!strcmp(buf,"squares_caption")) {
			p=themeinfo.squares_caption;
		}
		if(!strcmp(buf,"squares_title")) {
			p=themeinfo.squares_title;
		}
	  if(!strcmp(buf,"squares_x")) {
	    themeinfo.squares_x[0]=atoi(val);
	  }
	  if(!strcmp(buf,"squares_y")) {
	    themeinfo.squares_y[0]=atoi(val);
	  }
	  if(!strcmp(buf,"squares_bg_x")) {
	    themeinfo.squares_bg_x[0]=atoi(val);
	  }
	  if(!strcmp(buf,"squares_bg_y")) {
	    themeinfo.squares_bg_y[0]=atoi(val);
	  }
	  if(!strcmp(buf,"squares_size")) {
	    themeinfo.squares_size=atoi(val);
	  }
	  if(!strcmp(buf,"score_x")) {
	    themeinfo.score_x[0]=atoi(val);
	  }
	  if(!strcmp(buf,"score_y")) {
	    themeinfo.score_y[0]=atoi(val);
	  }
	  if(!strcmp(buf,"score_bg_x")) {
	    themeinfo.score_bg_x[0]=atoi(val);
	  }
	  if(!strcmp(buf,"score_bg_y")) {
	    themeinfo.score_bg_y[0]=atoi(val);
	  }
	  if(!strcmp(buf,"score_size")) {
	    themeinfo.score_size=atoi(val);
	  }
		if(!strcmp(buf,"score_caption")) {
			p=themeinfo.score_caption;
		}
		if(!strcmp(buf,"score_title")) {
			p=themeinfo.score_title;
		}
		if(!strcmp(buf,"combo_caption")) {
			p=themeinfo.combo_caption;
		}
		if(!strcmp(buf,"combo_title")) {
			p=themeinfo.combo_title;
		}
	  if(!strcmp(buf,"combo_x")) {
	    themeinfo.combo_x[0]=atoi(val);
	  }
	  if(!strcmp(buf,"combo_y")) {
	    themeinfo.combo_y[0]=atoi(val);
	  }
	  if(!strcmp(buf,"combo_bg_x")) {
	    themeinfo.combo_bg_x[0]=atoi(val);
	  }
	  if(!strcmp(buf,"combo_bg_y")) {
	    themeinfo.combo_bg_y[0]=atoi(val);
	  }
	  if(!strcmp(buf,"combo_size")) {
	    themeinfo.combo_size=atoi(val);
	  }
	  if(!strcmp(buf,"time_x")) {
	    themeinfo.time_x=atoi(val);
	  }
	  if(!strcmp(buf,"time_y")) {
	    themeinfo.time_y=atoi(val);
	  }
	  if(!strcmp(buf,"time_bg_x")) {
	    themeinfo.time_bg_x=atoi(val);
	  }
	  if(!strcmp(buf,"time_bg_y")) {
	    themeinfo.time_bg_y=atoi(val);
	  }
	  if(!strcmp(buf,"time_size")) {
	    themeinfo.time_size=atoi(val);
	  }
		if(!strcmp(buf,"time_title")) {
			p=themeinfo.time_title;
		}
		if(!strcmp(buf,"invinc")) {
			p=themeinfo.invinc;
		}
		if(!strcmp(buf,"plus1000")) {
			p=themeinfo.plus1000;
		}
		if(!strcmp(buf,"slowdown")) {
			p=themeinfo.slowdown;
		}
		if(!strcmp(buf,"minisquare")) {
			p=themeinfo.minisquare;
		}
		if(!strcmp(buf,"score")) {
			p=themeinfo.score;
		}
		if(!strcmp(buf,"powershadow")) {
			p=themeinfo.powershadow;
		}
		if(!strcmp(buf,"bigsquare")) {
			p=themeinfo.bigsquare;
		}
		if(!strcmp(buf,"minus1000")) {
			p=themeinfo.minus1000;
		}
		if(!strcmp(buf,"speedup")) {
			p=themeinfo.speedup;
		}
		if(!strcmp(buf,"evil")) {
			p=themeinfo.evil;
		}
		if(!strcmp(buf,"enemy")) {
			p=themeinfo.enemy;
		}
	  if(!strcmp(buf,"scale")) {
	    themeinfo.scale=atof(val);
			if(themeinfo.scale < 1) themeinfo.scale=1;
	  }
	  if(!strcmp(buf,"good_r")) {
	    themeinfo.good_r=atoi(val);
	  }
	  if(!strcmp(buf,"good_g")) {
	    themeinfo.good_g=atoi(val);
	  }
	  if(!strcmp(buf,"good_b")) {
	    themeinfo.good_b=atoi(val);
	  }
	  if(!strcmp(buf,"text_r")) {
	    themeinfo.text_r=atoi(val);
	  }
	  if(!strcmp(buf,"text_g")) {
	    themeinfo.text_g=atoi(val);
	  }
	  if(!strcmp(buf,"text_b")) {
	    themeinfo.text_b=atoi(val);
	  }
	  if(!strcmp(buf,"evil_r")) {
	    themeinfo.evil_r=atoi(val);
	  }
	  if(!strcmp(buf,"evil_g")) {
	    themeinfo.evil_g=atoi(val);
	  }
	  if(!strcmp(buf,"evil_b")) {
	    themeinfo.evil_b=atoi(val);
	  }
	  if(!strcmp(buf,"game_x")) {
	    themeinfo.game_x=atoi(val);
	  }
	  if(!strcmp(buf,"game_y")) {
	    themeinfo.game_y=atoi(val);
	  }
	  if(!strcmp(buf,"game_w")) {
	    themeinfo.game_w=atoi(val);
	  }		
	  if(!strcmp(buf,"game_h")) {
	    themeinfo.game_h=atoi(val);
	  }		
	  if(!strcmp(buf,"title_x")) {
	    themeinfo.title_x=atoi(val);
	  }
	  if(!strcmp(buf,"title_y")) {
	    themeinfo.title_y=atoi(val);
	  }
	  if(!strcmp(buf,"title_w")) {
	    themeinfo.title_w=atoi(val);
	  }		
	  if(!strcmp(buf,"title_h")) {
	    themeinfo.title_h=atoi(val);
	  }	
	  if(!strcmp(buf,"logo_w")) {
	    themeinfo.logo_w=atoi(val);
	  }
	  if(!strcmp(buf,"logo_h")) {
	    themeinfo.logo_h=atoi(val);
	  }
	  if(!strcmp(buf,"stat_w")) {
	    themeinfo.stat_w=atoi(val);
	  }		
	  if(!strcmp(buf,"stat_h")) {
	    themeinfo.stat_h=atoi(val);
	  }	
	  if(!strcmp(buf,"stat_size")) {
	    themeinfo.stat_size=atoi(val);
	  }	
		for(int i=0; i<MAX_PLAYERS; i++) {
			sprintf(buf2,"squares_x%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.squares_x[i]=atoi(val);
			}
			sprintf(buf2,"squares_y%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.squares_y[i]=atoi(val);
			}
			sprintf(buf2,"squares_bg_x%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.squares_bg_x[i]=atoi(val);
			}
			sprintf(buf2,"squares_bg_y%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.squares_bg_y[i]=atoi(val);
			}
			sprintf(buf2,"score_x%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.score_x[i]=atoi(val);
			}
			sprintf(buf2,"score_y%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.score_y[i]=atoi(val);
			}
			sprintf(buf2,"score_bg_x%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.score_bg_x[i]=atoi(val);
			}
			sprintf(buf2,"score_bg_y%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.score_bg_y[i]=atoi(val);
			}
			sprintf(buf2,"combo_x%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.combo_x[i]=atoi(val);
			}
			sprintf(buf2,"combo_y%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.combo_y[i]=atoi(val);
			}
			sprintf(buf2,"combo_bg_x%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.combo_bg_x[i]=atoi(val);
			}
			sprintf(buf2,"combo_bg_y%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.combo_bg_y[i]=atoi(val);
			}
			sprintf(buf2,"player_x%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.player_x[i]=atoi(val);
			}
			sprintf(buf2,"player_y%i",i);
			if(!strcmp(buf,buf2)) {
				themeinfo.player_y[i]=atoi(val);
			}
		}
	  if(p!=NULL) {
			if(val==NULL) {
				p[0]='\0';
			} else {
				strcpy(p,val);
			}
	  }
	}
	fclose(f);
#if defined(SDL) || defined(TIKI)
	loading_tex=new Texture("loading.png",false);
	for(float x=0; x<=1; x+=0.015) {
#ifdef TIKI
		Frame::begin();
#else
		if(sys_render_begin()) {
#endif
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_bg(loading_tex,x);
#ifdef TIKI
			Frame::finish();
#else
			sys_render_finish();
		}		
		delay(0.01);
#endif
	}
#endif
	game_tex=new Texture(theme_dir("game_bg.png"),0);
	bg_tex=new Texture(theme_dir("bg.png"),0);
	stat_tex=new Texture(theme_dir("stat_bg.png"),0);
	if(themeinfo.logo_w>0) logo_tex=new Texture(theme_dir("logo.png"),1);
	if(themeinfo.score[0]!='\0') score_tex=new Texture(theme_dir(themeinfo.score),1);
	if(themeinfo.invinc[0]!='\0') invinc_tex=new Texture(theme_dir(themeinfo.invinc),1);
	if(themeinfo.plus1000[0]!='\0') plus_tex=new Texture(theme_dir(themeinfo.plus1000),1);
	if(themeinfo.slowdown[0]!='\0') slow_tex=new Texture(theme_dir(themeinfo.slowdown),1);
	if(themeinfo.minisquare[0]!='\0') mini_tex=new Texture(theme_dir(themeinfo.minisquare),1);
	if(themeinfo.enemy[0]!='\0') enemy_tex=new Texture(theme_dir(themeinfo.enemy),1);
	if(themeinfo.bigsquare[0]!='\0') big_tex=new Texture(theme_dir(themeinfo.bigsquare),1);
	if(themeinfo.evil[0]!='\0') evil_tex=new Texture(theme_dir(themeinfo.evil),1);
	if(themeinfo.minus1000[0]!='\0') minus_tex=new Texture(theme_dir(themeinfo.minus1000),1);
	if(themeinfo.speedup[0]!='\0') speed_tex=new Texture(theme_dir(themeinfo.speedup),1);
	if(themeinfo.powershadow[0]!='\0') shadow_tex=new Texture(theme_dir(themeinfo.powershadow),1);
#ifdef DREAMCAST
	powerup=snd_sfx_load(theme_dir("powerup.wav"));
	powerdown=snd_sfx_load(theme_dir("powerdown.wav"));
	collect=snd_sfx_load(theme_dir("collect.wav"));
	gameover=snd_sfx_load(theme_dir("gameover.wav"));
#endif
#if defined(SDL) || defined(TIKI)
	for(float x=0; x<=1; x+=0.015) {
#ifdef TIKI
		Frame::begin();
#else
		if(sys_render_begin()) {
#endif
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			render_bg(loading_tex,1.0f-x);
#ifdef TIKI
			Frame::finish();
#else
			sys_render_finish();
		}		
#endif
	}
#endif
  return 0;
}

void unload_theme() {
#ifdef DREAMCAST
	snd_sfx_unload(powerup);
	snd_sfx_unload(powerdown);
	snd_sfx_unload(collect);
	snd_sfx_unload(gameover);
#endif
}
