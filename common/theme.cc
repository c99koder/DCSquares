#ifdef DIRECTX
#define UNICODE
#endif
#ifdef WIN32
#include <windows.h>
#include <atlconv.h>
#endif
#ifdef DIRECTX
#include "dxutil.h"
#include "dsutil.h"
#endif
#ifdef DREAMCAST
#include <kos.h>
#endif
#ifdef MACOS
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#ifdef OPENAL
#define AL_FORMAT_VORBIS_EXT 0x10003
#include <AL/al.h>
#include <AL/alut.h>
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
#include <stdlib.h>
#include <string.h>

#ifdef TIKI
#include <Tiki/tiki.h>
#include <Tiki/gl.h>

using namespace Tiki;
using namespace Tiki::GL;
#endif

#include "squares.h"
#include "theme.h"
#include "texture.h"
#if !defined(TIKI) && (defined(DREAMCAST) || defined(SDL))
#include "sys.h"
#endif

int loading_tex=15;
int logo_tex=0;
int game_tex=1;
int stat_tex=2;
int bg_tex=3;
int score_tex=4;
int enemy_tex=5;
int invinc_tex=6;
int slow_tex=7; 
int mini_tex=8;
int plus_tex=9;
int evil_tex=10;
int speed_tex=11;
int minus_tex=12;
int big_tex=13;
int shadow_tex=14;
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
#ifdef OPENAL
#include <sys/stat.h>
ALuint buffers[6];
ALuint sources[6];

void addSource( int buffer, int loop ) {
	alSource3f( sources[buffer] , AL_POSITION       , 0.0f , 0.0f , 0.0f );
	alSource3f( sources[buffer] , AL_VELOCITY       , 0.0f , 0.0f , 0.0f );
	alSource3f( sources[buffer] , AL_DIRECTION      , 0.0f , 0.0f , 0.0f );
	alSourcef ( sources[buffer] , AL_ROLLOFF_FACTOR  , 0.0                );
	alSourcei ( sources[buffer] , AL_SOURCE_RELATIVE , AL_TRUE            );
	alSourcei(sources[buffer] , AL_LOOPING, loop);
	alSourcei(sources[buffer] , AL_BUFFER , buffers[buffer]);  
}

void LoadOgg( char* inSoundFile, int buffer ) 
{
  void  *   ovdata  ;	
  FILE  *   fh      ;
  
  fh = fopen( inSoundFile , "rb") ;
  if( fh != NULL ) 
  {
    struct stat sbuf ;
   
    if( stat( inSoundFile , &sbuf ) != -1 ) 
    {
      ovdata = malloc( sbuf.st_size ) ;
      if( ovdata != NULL )
      {
        fread( ovdata       ,
               1            ,
               sbuf.st_size , 
               fh           ) ;
        
        alBufferData( buffers[buffer]               ,
                      AL_FORMAT_VORBIS_EXT  ,
                      ovdata                ,
                      sbuf.st_size          ,
                      1                     ) ;
        
        free( ovdata ) ;
      }				 
      fclose( fh ) ;
    }	   
  }
}
#endif

void render_bg(int tex, float fade) {
#ifdef DREAMCAST
  GLfloat uv[4]={1,1,0,0};
#else
  GLfloat uv[4]={0,0,1,1};
#endif
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();	
	glColor3f(fade,fade,fade);
	switch_tex(tex);
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

void render_box(int x, int y, int w, int h, int tex, float fade) {
#ifdef DREAMCAST
  GLfloat uv[4]={1,1,0,0};
#else
  GLfloat uv[4]={0,0,1,1};
#endif
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();	
	glColor3f(fade,fade,fade);
	switch_tex(tex);
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

void render_bg_game(int tex, float fade) {
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();	
	glColor3f(fade,fade,fade);
	switch_tex(tex);
	glBegin(GL_QUADS);
#ifndef DREAMCAST
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
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,0,0.9);
	glTexCoord2f(1,0);
	glVertex3f(640,0,0.9);
	glTexCoord2f(1,(((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f));
	glVertex3f(640,(float)themeinfo.game_y+(float)themeinfo.game_h,0.9);
	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,(((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f));
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,(float)themeinfo.game_y+(float)themeinfo.game_h,0.9);

	glTexCoord2f((float)themeinfo.game_x/640.0f,(((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f));
	glVertex3f((float)themeinfo.game_x,(float)themeinfo.game_y+(float)themeinfo.game_h,0.9);
	glTexCoord2f(1,(((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f));
	glVertex3f(640,(float)themeinfo.game_y+(float)themeinfo.game_h,0.9);
	glTexCoord2f(1,1);
	glVertex3f(640,480,0.9);
	glTexCoord2f((float)themeinfo.game_x/640.0f,1);
	glVertex3f((float)themeinfo.game_x,480,0.9);
	
	/*glTexCoord2f((float)themeinfo.game_x/640.0f,(float)themeinfo.game_y/480.0f);
	glVertex3f((float)themeinfo.game_x,(float)themeinfo.game_y,0.1);
	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,(float)themeinfo.game_y/480.0f);
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,(float)themeinfo.game_y,0.1);
	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f);
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,(float)themeinfo.game_y+(float)themeinfo.game_h,0.1);
	glTexCoord2f((float)themeinfo.game_x/640.0f,((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f);
	glVertex3f((float)themeinfo.game_x,(float)themeinfo.game_y+(float)themeinfo.game_h,0.1);*/
#else
	glTexCoord2f(0,1);
	glVertex3f(0,0,0.9);
	glTexCoord2f((float)themeinfo.game_x/640.0f,1);
	glVertex3f((float)themeinfo.game_x,0,0.9);
	glTexCoord2f((float)themeinfo.game_x/640.0f,0);
	glVertex3f((float)themeinfo.game_x,480,0.9);
	glTexCoord2f(0,0);
	glVertex3f(0,480,0.9);

	glTexCoord2f((float)themeinfo.game_x/640.0f,1);
	glVertex3f((float)themeinfo.game_x,0,0.9);
	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,1);
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,1,0.9);
	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,1-(float)themeinfo.game_y/480.0f);
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,(float)themeinfo.game_y,0.9);
	glTexCoord2f((float)themeinfo.game_x/640.0f,1-(float)themeinfo.game_y/480.0f);
	glVertex3f((float)themeinfo.game_x,(float)themeinfo.game_y,0.9);

	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,1);
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,0,0.02);
	glTexCoord2f(1,1);
	glVertex3f(640,0,0.02);
	glTexCoord2f(1,1-(((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f));
	glVertex3f(640,(float)themeinfo.game_y+(float)themeinfo.game_h,0.02);
	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,1-(((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f));
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,(float)themeinfo.game_y+(float)themeinfo.game_h,0.02);

	glTexCoord2f((float)themeinfo.game_x/640.0f,1-(((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f));
	glVertex3f((float)themeinfo.game_x,(float)themeinfo.game_y+(float)themeinfo.game_h,0.9);
	glTexCoord2f(1,1-(((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f));
	glVertex3f(640,(float)themeinfo.game_y+(float)themeinfo.game_h,0.9);
	glTexCoord2f(1,0);
	glVertex3f(640,480,0.9);
	glTexCoord2f((float)themeinfo.game_x/640.0f,0);
	glVertex3f((float)themeinfo.game_x,480,0.9);
	
	/*glTexCoord2f((float)themeinfo.game_x/640.0f,1-(float)themeinfo.game_y/480.0f);
	glVertex3f((float)themeinfo.game_x,(float)themeinfo.game_y,0.01);
	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,1-(float)themeinfo.game_y/480.0f);
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,(float)themeinfo.game_y,0.01);
	glTexCoord2f(((float)themeinfo.game_x+(float)themeinfo.game_w)/640.0f,1-((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f);
	glVertex3f((float)themeinfo.game_x+(float)themeinfo.game_w,(float)themeinfo.game_y+(float)themeinfo.game_h,0.01);
	glTexCoord2f((float)themeinfo.game_x/640.0f,1-((float)themeinfo.game_y+(float)themeinfo.game_h)/480.0f);
	glVertex3f((float)themeinfo.game_x,(float)themeinfo.game_y+(float)themeinfo.game_h,0.01);*/
#endif
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void render_bg_title(int tex, float fade) {
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();	
	glColor3f(fade,fade,fade);
	switch_tex(tex);
	glBegin(GL_QUADS);
#ifndef DREAMCAST
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
	
	glTexCoord2f((float)themeinfo.title_x/640.0f,(float)themeinfo.title_y/480.0f);
	glVertex3f((float)themeinfo.title_x,(float)themeinfo.title_y,0.1);
	glTexCoord2f(((float)themeinfo.title_x+(float)themeinfo.title_w)/640.0f,(float)themeinfo.title_y/480.0f);
	glVertex3f((float)themeinfo.title_x+(float)themeinfo.title_w,(float)themeinfo.title_y,0.1);
	glTexCoord2f(((float)themeinfo.title_x+(float)themeinfo.title_w)/640.0f,((float)themeinfo.title_y+(float)themeinfo.title_h)/480.0f);
	glVertex3f((float)themeinfo.title_x+(float)themeinfo.title_w,(float)themeinfo.title_y+(float)themeinfo.title_h,0.1);
	glTexCoord2f((float)themeinfo.title_x/640.0f,((float)themeinfo.title_y+(float)themeinfo.title_h)/480.0f);
	glVertex3f((float)themeinfo.title_x,(float)themeinfo.title_y+(float)themeinfo.title_h,0.1);
#else
	glTexCoord2f(0,1);
	glVertex3f(0,0,0.9);
	glTexCoord2f((float)themeinfo.title_x/640.0f,1);
	glVertex3f((float)themeinfo.title_x,0,0.9);
	glTexCoord2f((float)themeinfo.title_x/640.0f,0);
	glVertex3f((float)themeinfo.title_x,480,0.9);
	glTexCoord2f(0,0);
	glVertex3f(0,480,0.9);
	
	glTexCoord2f((float)themeinfo.title_x/640.0f,1);
	glVertex3f((float)themeinfo.title_x,0,0.9);
	glTexCoord2f(((float)themeinfo.title_x+(float)themeinfo.title_w)/640.0f,1);
	glVertex3f((float)themeinfo.title_x+(float)themeinfo.title_w,1,0.9);
	glTexCoord2f(((float)themeinfo.title_x+(float)themeinfo.title_w)/640.0f,1-(float)themeinfo.title_y/480.0f);
	glVertex3f((float)themeinfo.title_x+(float)themeinfo.title_w,(float)themeinfo.title_y,0.9);
	glTexCoord2f((float)themeinfo.title_x/640.0f,1-(float)themeinfo.title_y/480.0f);
	glVertex3f((float)themeinfo.title_x,(float)themeinfo.title_y,0.9);
	
	glTexCoord2f(((float)themeinfo.title_x+(float)themeinfo.title_w)/640.0f,1);
	glVertex3f((float)themeinfo.title_x+(float)themeinfo.title_w,0,0.9);
	glTexCoord2f(1,1);
	glVertex3f(640,0,0.9);
	glTexCoord2f(1,1-(((float)themeinfo.title_y+(float)themeinfo.title_h)/480.0f));
	glVertex3f(640,(float)themeinfo.title_y+(float)themeinfo.title_h,0.9);
	glTexCoord2f(((float)themeinfo.title_x+(float)themeinfo.title_w)/640.0f,1-(((float)themeinfo.title_y+(float)themeinfo.title_h)/480.0f));
	glVertex3f((float)themeinfo.title_x+(float)themeinfo.title_w,(float)themeinfo.title_y+(float)themeinfo.title_h,0.9);
	
	glTexCoord2f((float)themeinfo.title_x/640.0f,1-(((float)themeinfo.title_y+(float)themeinfo.title_h)/480.0f));
	glVertex3f((float)themeinfo.title_x,(float)themeinfo.title_y+(float)themeinfo.title_h,0.9);
	glTexCoord2f(1,1-(((float)themeinfo.title_y+(float)themeinfo.title_h)/480.0f));
	glVertex3f(640,(float)themeinfo.title_y+(float)themeinfo.title_h,0.9);
	glTexCoord2f(1,0);
	glVertex3f(640,480,0.9);
	glTexCoord2f((float)themeinfo.title_x/640.0f,0);
	glVertex3f((float)themeinfo.title_x,480,0.9);
	
	/*glTexCoord2f((float)themeinfo.title_x/640.0f,1-(float)themeinfo.title_y/480.0f);
	glVertex3f((float)themeinfo.title_x,(float)themeinfo.title_y,0.01);
	glTexCoord2f(((float)themeinfo.title_x+(float)themeinfo.title_w)/640.0f,1-(float)themeinfo.title_y/480.0f);
	glVertex3f((float)themeinfo.title_x+(float)themeinfo.title_w,(float)themeinfo.title_y,0.01);
	glTexCoord2f(((float)themeinfo.title_x+(float)themeinfo.title_w)/640.0f,1-((float)themeinfo.title_y+(float)themeinfo.title_h)/480.0f);
	glVertex3f((float)themeinfo.title_x+(float)themeinfo.title_w,(float)themeinfo.title_y+(float)themeinfo.title_h,0.01);
	glTexCoord2f((float)themeinfo.title_x/640.0f,1-((float)themeinfo.title_y+(float)themeinfo.title_h)/480.0f);
	glVertex3f((float)themeinfo.title_x,(float)themeinfo.title_y+(float)themeinfo.title_h,0.01);*/
#endif
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void render_poly(int size, int tex, float fade) {
#ifdef DREAMCAST
  GLfloat uv[4]={1,1,0,0};
#else
  GLfloat uv[4]={0,0,1,1};
#endif
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,fade);
	switch_tex(tex);
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
sfxhnd_t powerup=-1;
sfxhnd_t powerdown=-1;
sfxhnd_t collect=-1;
sfxhnd_t gameover=-1;

extern "C" {
void update_lcds();
}
#endif
#ifdef SDL
Mix_Chunk *powerup;
Mix_Chunk *powerdown;
Mix_Chunk *collect;
Mix_Chunk *gameover;
Mix_Music *bgm;
Mix_Music *title;
#endif
#ifdef DIRECTX
extern HDC hDC;
extern HWND hWnd;
extern CSoundManager *g_pSoundManager;
CSound *powerup=NULL;
CSound *powerdown=NULL;
CSound *collect=NULL;
CSound *gameover=NULL;

void audio_init();
void audio_shutdown();
void play_bgm(LPCWSTR fn);
void stop_bgm();
void c99_show_cursor(bool r);
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
#ifdef DIRECTX
	USES_CONVERSION;
#endif
#ifdef OPENAL
    // Variables to load into.
    ALenum format;
    ALsizei size;
    ALvoid* data;
    ALsizei freq;
    ALboolean loop;
#endif
    FILE *f;
	char buf[200];
	char buf2[200];
	char *p=NULL;
	char *val;

	if(theme!=NULL) strcpy(dcs_theme,theme); else strcpy(dcs_theme,"default");

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
	
  f=fopen(theme_dir("theme.ini"),"r");
	if(!f) {
		if(strcmp("default",theme)) {
			return load_theme("default",sfx);
		} else {
#ifdef WIN32
			MessageBox(NULL,A2W("Cannot load default theme"),A2W("Error"),MB_OK|MB_ICONHAND);
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
	load_texture("loading",loading_tex,0);
	for(float x=0; x<=1; x+=0.01) {
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
	load_texture(theme_dir("game_bg"),game_tex,0);
	load_texture(theme_dir("bg"),bg_tex,0);
	load_texture(theme_dir("stat_bg"),stat_tex,0);
	if(themeinfo.logo_w>0) load_texture(theme_dir("logo"),logo_tex,1);
	if(themeinfo.score[0]!='\0') load_texture(theme_dir(themeinfo.score),score_tex,1);
	if(themeinfo.invinc[0]!='\0') load_texture(theme_dir(themeinfo.invinc),invinc_tex,1);
	if(themeinfo.plus1000[0]!='\0') load_texture(theme_dir(themeinfo.plus1000),plus_tex,1);
	if(themeinfo.slowdown[0]!='\0') load_texture(theme_dir(themeinfo.slowdown),slow_tex,1);
	if(themeinfo.minisquare[0]!='\0') load_texture(theme_dir(themeinfo.minisquare),mini_tex,1);
	if(themeinfo.enemy[0]!='\0') load_texture(theme_dir(themeinfo.enemy),enemy_tex,1);
	if(themeinfo.bigsquare[0]!='\0') load_texture(theme_dir(themeinfo.bigsquare),big_tex,1);
	if(themeinfo.evil[0]!='\0') load_texture(theme_dir(themeinfo.evil),evil_tex,1);
	if(themeinfo.minus1000[0]!='\0') load_texture(theme_dir(themeinfo.minus1000),minus_tex,1);
	if(themeinfo.speedup[0]!='\0') load_texture(theme_dir(themeinfo.speedup),speed_tex,1);
	if(themeinfo.powershadow[0]!='\0') load_texture(theme_dir(themeinfo.powershadow),shadow_tex,1);
#ifdef SDL
	powerup=Mix_LoadWAV(theme_dir("powerup.wav"));
	powerdown=Mix_LoadWAV(theme_dir("powerdown.wav"));
	collect=Mix_LoadWAV(theme_dir("collect.wav"));
	gameover=Mix_LoadWAV(theme_dir("gameover.wav"));
	bgm=Mix_LoadMUS(theme_dir("bg_loop.ogg"));
	title=Mix_LoadMUS(theme_dir("bgdim_loop.ogg"));
#endif
#ifdef OPENAL
	alGenBuffers(6, buffers);
	alGenSources(6, sources);
	alutLoadWAVFile((ALbyte *)theme_dir("collect.wav"), &format, &data, &size, &freq, &loop);
  alBufferData(buffers[SND_COLLECT], format, data, size, freq);
  alutUnloadWAV(format, data, size, freq);
	addSource(SND_COLLECT,AL_FALSE);
	alutLoadWAVFile((ALbyte *)theme_dir("powerup.wav"), &format, &data, &size, &freq, &loop);
  alBufferData(buffers[SND_POWERUP], format, data, size, freq);
  alutUnloadWAV(format, data, size, freq);
	addSource(SND_POWERUP,AL_FALSE);
	alutLoadWAVFile((ALbyte *)theme_dir("powerdown.wav"), &format, &data, &size, &freq, &loop);
  alBufferData(buffers[SND_POWERDOWN], format, data, size, freq);
  alutUnloadWAV(format, data, size, freq);
	addSource(SND_POWERDOWN,AL_FALSE);
	alutLoadWAVFile((ALbyte *)theme_dir("gameover.wav"), &format, &data, &size, &freq, &loop);
  alBufferData(buffers[SND_GAMEOVER], format, data, size, freq);
  alutUnloadWAV(format, data, size, freq);
	addSource(SND_GAMEOVER,AL_FALSE);
	LoadOgg(theme_dir("bgdim_loop.ogg"),SND_TITLE);
	addSource(SND_TITLE,AL_TRUE);
	LoadOgg(theme_dir("bg_loop.ogg"),SND_BGM);
	addSource(SND_BGM,AL_TRUE);
#endif
#ifdef DREAMCAST
	powerup=snd_sfx_load(theme_dir("powerup.wav"));
	powerdown=snd_sfx_load(theme_dir("powerdown.wav"));
	collect=snd_sfx_load(theme_dir("collect.wav"));
	gameover=snd_sfx_load(theme_dir("gameover.wav"));
#endif
#ifdef DIRECTX
	if(sfx) {
  USES_CONVERSION;
  g_pSoundManager->Create( &powerup, A2W(theme_dir("powerup.wav")), 0, GUID_NULL );
  g_pSoundManager->Create( &powerdown, A2W(theme_dir("powerdown.wav")), 0, GUID_NULL );
  g_pSoundManager->Create( &collect, A2W(theme_dir("collect.wav")), 0, GUID_NULL );
  g_pSoundManager->Create( &gameover, A2W(theme_dir("gameover.wav")), 0, GUID_NULL );
	}
#endif
#if defined(SDL) || defined(TIKI)
	for(float x=0; x<=1; x+=0.01) {
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
  clear_texture_cache();
#ifdef SDL
	Mix_FreeChunk(powerup);
	Mix_FreeChunk(powerdown);
	Mix_FreeChunk(collect);
	Mix_FreeChunk(gameover);
	Mix_FreeMusic(bgm);
	Mix_FreeMusic(title);
#endif
#ifdef DREAMCAST
	snd_sfx_unload(powerup);
	snd_sfx_unload(powerdown);
	snd_sfx_unload(collect);
	snd_sfx_unload(gameover);
#endif
#ifdef OPENAL
	for(int x=0; x<6; x++) {
		alSourceStop(sources[x]);
	}
	alDeleteBuffers(6,buffers);
	alDeleteSources(6,sources);
#endif
#ifdef DIRECTX
  delete powerup;
  delete powerdown;
  delete gameover;
  delete collect;
  powerup=NULL;
  powerdown=NULL;
  gameover=NULL;
  collect=NULL;
#endif
}
