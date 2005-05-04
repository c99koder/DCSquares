#include <string.h>
#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <math.h>
#endif
#ifdef UNIX
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#endif
#ifdef DREAMCAST
#include <kos.h>
#define open fs_open
#define close fs_close
#define read fs_read
#endif
#ifdef OPENGL
#ifdef MACOS
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif
#include "texture.h"
#include "font.h"
#include "config.h"

int font_tex_id=-1;

float font_width=VIDEO_X/TEXT_X;
float font_height=VIDEO_Y/TEXT_Y;

void set_font_width(float width) {
  font_width=width;
}

void set_font_height(float height) {
  font_height=height;
}

float get_font_width() { return font_width; }
float get_font_height() { return font_height; }

void load_font(char *filename) {
  int low=0,high=255,mask,a,y,x,rx,ry;
  int in;
  int R=255,B=255,G=255,A=255;
#ifdef DREAMCAST
  uint16 *data=(uint16 *)malloc(128*128*2);
  void *pvrtex=pvr_mem_malloc(128*128*2);
#else
  unsigned char data[128*128*2];
#endif

	if(font_tex_id==-1) {
		font_tex_id=next_tex_id();
		printf("Texture ID: %i\n",font_tex_id);
	} else {
		return;
	}

  char byte;
  memset(data,0,128*128*2);
 
  in=open(filename,O_RDONLY);

  for(a=low;a<=high;a++) {
    for(y=0;y<=7;y++) {
      read(in,&byte,1);
      mask = 0x80;
      for(x=0;x<=7;x++) {
        rx=((a%16)*8)+x;
	      ry=((a/16)*8)+y;
        if(byte & mask) {
#if defined(WIN32) || defined(LINUX)
          data[ry*128+rx] = 255;
#endif
#ifdef DREAMCAST
          data[ry*128+rx] = 65535;
#endif
        } else {
          data[ry*128+rx] = 0;
        }
        mask >>= 1;
      }
    }
  }
  close(in);
  switch_tex(font_tex_id);
#ifdef DREAMCAST
  pvr_txr_load_ex((void*)data, (void *)pvrtex, 128, 128, PVR_TXRLOAD_16BPP);
  glKosTex2D(GL_ARGB4444_TWID, 128, 128, (void*)pvrtex);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 128,128, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
#endif
#ifdef DREAMCAST
  //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_FILTER,GL_FILTER_BILINEAR);
  free(data);
#endif
}

void draw_char(unsigned char a, float x, float y, float z) {
  float rx,ry;
  rx=((a%16)*8);
  ry=((a/16)*8);
  //glBindTexture(GL_TEXTURE_2D, font_tex[0]);
  switch_tex(font_tex_id);
	glBegin(GL_QUADS);
  glTexCoord2f(rx/128.0f,ry/128.0f);
  glVertex3f(x,y,z);
  glTexCoord2f((rx+8)/128.0f,ry/128.0f);
  glVertex3f(x+font_width,y,z);
  glTexCoord2f((rx+8)/128.0f,(ry+8)/128.0f);
  glVertex3f(x+font_width,y+font_height,z);
  glTexCoord2f(rx/128.0f,(ry+8)/128.0f);
  glVertex3f(x,y+font_height,z);
  glEnd();
}

void draw_string(char *a, float x, float y, float z) {
  int i;

  for(i=0;i<strlen(a);i++) {
    draw_char(a[i],x+(i*font_width),y,z);
  }
}

