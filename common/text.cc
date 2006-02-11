/* text.cc - text rendering support functions
 * Copyright (c) 2001-2003 Sam Steele
 *
 * This file is part of DCBlap.
 *
 * DCBlap is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * DCBlap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifdef WIN32
#include <windows.h>
#include <GL\gl.h>
#endif
#ifdef LINUX
#include <SDL/SDL.h>
#include <GL/gl.h>
#endif
#ifdef MACOS
#include <OpenGL/gl.h>

void MessageBox(char *title, char *msg);
#endif

#include "squares.h"
#include "theme.h"
#include "text.h"
#ifndef TIKI
#include "ul.h"
#include "fnt.h"
#else
#include <Tiki/tiki.h>
#include <Tiki/font.h>
#include <Tiki/texture.h>
#include <Tiki/plxcompat.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::GL::Plxcompat;

#endif

#ifndef TIKI
fntRenderer *textR;
fntTexFont *font;
#else
RefPtr<Font> fnt;
#endif
float text_z=0.95f;

void text_set_z(float z) { text_z=z; } 

extern themeinfo_t themeinfo;

void center(int x, int y,char *text, int point, float fade) {
	set_font_size(point);
	text_set_z(40);
	draw_txt((x)-(txt_width(text)/2),y,text,(float)themeinfo.text_r / 255.0f,(float)themeinfo.text_g / 255.0f,(float)themeinfo.text_b / 255.0f,fade,point);
}

void center_shad_rgb(int x, int y,char *text, int point, float fade, float r, float g, float b) {
	set_font_size(point);
	text_set_z(40.90);
	draw_txt(x-(txt_width(text)/2)+1,y+1,text,0,0,0,fade*0.6,point);
	text_set_z(40.9999);
	draw_txt(x-(txt_width(text)/2)-1,y-1,text,r,g,b,fade,point);
}

void center_shad_rgb(int y,char *text, int point, float fade, float r, float g, float b, bool game) {
	set_font_size(point);
	if(game) {
		text_set_z(40.90);
		draw_txt((themeinfo.game_x+(themeinfo.game_w/2))-(txt_width(text)/2)+1,y+1,text,0,0,0,fade*0.6,point);
		text_set_z(40.9999);
		draw_txt((themeinfo.game_x+(themeinfo.game_w/2))-(txt_width(text)/2)-1,y-1,text,r,g,b,fade,point);
	} else {
		text_set_z(40.90);
		draw_txt((themeinfo.title_x+(themeinfo.title_w/2))-(txt_width(text)/2)+1,y+1,text,0,0,0,fade*0.6,point);
		text_set_z(40.9999);
		draw_txt((themeinfo.title_x+(themeinfo.title_w/2))-(txt_width(text)/2)-1,y-1,text,r,g,b,fade,point);
	}
}

void center_shad(int y,char *text, int point, float fade, bool game) {
	set_font_size(point);
	if(game) {
		text_set_z(40.90);
		draw_txt((themeinfo.game_x+(themeinfo.game_w/2))-(txt_width(text)/2)+1,y+1,text,0,0,0,fade*0.6,point);
		text_set_z(40.9999);
		draw_txt((themeinfo.game_x+(themeinfo.game_w/2))-(txt_width(text)/2)-1,y-1,text,(float)themeinfo.text_r / 255.0f,(float)themeinfo.text_g / 255.0f,(float)themeinfo.text_b / 255.0f,fade,point);
	} else {
		text_set_z(40.90);
		draw_txt((themeinfo.title_x+(themeinfo.title_w/2))-(txt_width(text)/2)+1,y+1,text,0,0,0,fade*0.6,point);
		text_set_z(40.9999);
		draw_txt((themeinfo.title_x+(themeinfo.title_w/2))-(txt_width(text)/2)-1,y-1,text,(float)themeinfo.text_r / 255.0f,(float)themeinfo.text_g / 255.0f,(float)themeinfo.text_b / 255.0f,fade,point);
	}
}
void draw_txt(int x, int y, char *text, float r, float g, float b, float a, int point) {
  if(r>1) r=1; if(r<0) r=0;
  if(g>1) g=1; if(g<0) g=0;
  if(b>1) b=1; if(b<0) b=0;
  if(a>1) a=1; if(a<0) a=0;
#ifndef TIKI
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0,640,0,480,1,-1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glEnable(GL_BLEND);			// Turn Blending On
  glDisable(GL_DEPTH_TEST);	// Turn Depth Testing Off

  textR->setPointSize(point);
  textR->begin();
	glColor4f(r,g,b,a);
  textR->start2f(x,480-y);
  textR->puts(text);
  textR->end();
  glColor4f(1,1,1,1);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glLoadIdentity();
  glDisable(GL_BLEND);			// Turn Blending On
  glEnable(GL_DEPTH_TEST);	// Turn Depth Testing Off
#else
  int cx=x,cy=y,i;
  char tmp[2];
  fnt->setColor(Color(a,r,g,b));
  fnt->setSize(point);
  tmp[1]='\0';
  fnt->draw(Vector(cx, cy-((float)point/4.0f), text_z), text);
  for(i=0;i<strlen(text);i++) {
    tmp[0]=text[i];
    if(tmp[0]=='\n') {
      cy+=point;
      cx=x;
    } else {
      cx+=txt_width(tmp);
    }
  }
#endif
}

#ifndef TIKI
void cbk( enum ulSeverity severity, char* msg ){
	//MessageBox(NULL,msg,"Warning",MB_OK|MB_ICONEXCLAMATION);
}
#endif

int text_init(char *filename, int pointsize) {
#ifndef TIKI
  //ulSetErrorCallback(cbk);
  fntInit();
  textR=new fntRenderer();
  font=new fntTexFont(filename);
  if(strlen(ulGetError())>0) {
#ifdef WIN32
	  MessageBox(NULL,"Cannot load font","Error",MB_OK|MB_ICONHAND);
#endif
#ifdef MACOS
		MessageBox("Error","Cannot load font");
#endif
	  return -1;
  }
  textR->setFont(font);
  textR->setPointSize(pointsize);
#else
  fnt = new Font(filename);
  fnt->setSize(pointsize);
#endif
  return 0;
}

float txt_width(char *text) {
  float top,bot,left,right;
#ifndef TIKI 
  font->getBBox(text,textR->getPointSize(),0,&left,&right,&bot,&top);
#else
  fnt->getTextSize(text,right,bot);
#endif
  return right;
}

void set_font_size(int pointsize) {
#ifndef TIKI
  textR->setPointSize(pointsize);
#else
  fnt->setSize(pointsize);
#endif
}
