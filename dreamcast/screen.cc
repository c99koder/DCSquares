#include <stdio.h>
#include <stdarg.h>
#include <string.h>
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
#include "config.h"
#include "font.h"
#include "sys.h"
#include "screen.h"

int cursor_fg,cursor_bg,cursor_x,cursor_y, border_color;
int screen_x=TEXT_X;
int screen_y=TEXT_Y;
int c99_do_wrap=1;
int c99_do_scroll=1;
int c99_do_render=0;

void set_do_wrap(int val) {
  c99_do_wrap=val;
}

void set_do_scroll(int val) {
  c99_do_scroll=val;
}

int get_screen_width() {
  return screen_x;
}

int get_screen_height() {
  return screen_y;
}

void do_render(int mode) { c99_do_render=mode; }

void set_border(int color) {
  border_color=color;
}

void c99_resize_screen(int x, int y, int xsize, int ysize) {
  clear_screen();
  screen_x=x;
  screen_y=y;
  set_font_width(xsize);
  set_font_height(ysize);
  clear_screen();
}

void video_refresh() {
  if(sys_render_begin()) { //make sure it's ok to draw (are we minimized?)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_screen();
    sys_render_finish();
  }
}

struct character {
  unsigned char ascii;
  int fg;
  int bg;
} video_ram[TEXT_X][TEXT_Y];

void init_screen() {
  cursor_x=0;
  cursor_y=0;
  cursor_fg=1;
  cursor_bg=0;
  border_color=0;
  set_font_width(8);
  set_font_height(18);
  clear_screen();
}

void cbm_color(int color) {
  float r,g,b;
  switch(color) {
    case 0:
      r = 0x00; g = 0x00; b = 0x00;
      break;
    case 1:
      r = 0x00; g = 0x00; b = 0x60;
      break;
    case 2:
      r = 0x00; g = 0x60; b = 0x00;
      break;
    case 3:
      r = 0x00; g = 0x60; b = 0x60;
      break;
    case 4:
      r = 0x60; g = 0x00; b = 0x00;
      break;
    case 5:
      r = 0x60; g = 0x00; b = 0x60;
      break;
    case 6:
      r = 0x60; g = 0x60; b = 0x00;
      break;
    case 7:
      r = 0x60; g = 0x60; b = 0x60;
      break;
    case 8:
      r = 0x40; g = 0x40; b = 0x40;
      break;
    case 9:
      r = 0x00; g = 0x00; b = 0xc0;
      break;
    case 10:
      r = 0x00; g = 0xc0; b = 0x00;
      break;
    case 11:
      r = 0x00; g = 0xc0; b = 0xc0;
      break;
    case 12:
      r = 0xc0; g = 0x00; b = 0x00;
      break;
    case 13:
      r = 0xc0; g = 0x00; b = 0xc0;
      break;
    case 14:
      r = 0xc0; g = 0xc0; b = 0x00;
      break;
    case 15:
      r = 0xc0; g = 0xc0; b = 0xc0;
      break;
    default:
      r = 0xc0; g = 0xc0; b = 0xc0;
      break;
  }
  glColor4f(r/255.0f,g/255.0f,b/255.0f,1);
}

void render_screen() {
  int x,y;
  //glLoadIdentity();
  glDisable(GL_TEXTURE_2D);
  cbm_color(border_color);
  glBegin(GL_QUADS);
  glVertex3f(0,0,-0.01);
  glVertex3f(VIDEO_X,0,-0.01);
  glVertex3f(VIDEO_X,VIDEO_Y,-0.01);
  glVertex3f(0,VIDEO_Y,-0.01);
  glEnd();
  glTranslatef((VIDEO_X-(screen_x*get_font_width()))/2,(VIDEO_Y-(screen_y*get_font_height()))/2,0);
  for(y=0;y<screen_y;y++) {
    for(x=0;x<screen_x;x++) {
      cbm_color(video_ram[x][y].bg);
      glBegin(GL_QUADS);
      glVertex3f(x*font_width,y*font_height,0.101);
      glVertex3f((x*font_width)+font_width,y*font_height,0.101);
      glVertex3f((x*font_width)+font_width,(y*font_height)+font_height,0.101);
      glVertex3f(x*font_width,(y*font_height)+font_height,0.101);
      glEnd();
    }
  }
  glEnable(GL_TEXTURE_2D);
#ifdef DREAMCAST
  glKosFinishList();
#else
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
#endif
  for(y=0;y<screen_y;y++) {
    for(x=0;x<screen_x;x++) {
      if(video_ram[x][y].ascii!=0) {
        cbm_color(video_ram[x][y].fg);
        if(video_ram[x][y].ascii!=32) draw_char(video_ram[x][y].ascii,x*get_font_width(),y*get_font_height(),0.11);
      }
    }
  }
#ifndef DREAMCAST
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
#endif
}

void c99_printf(const char *fmt, ...) {
  char buff[512];
  va_list args;
  
  va_start(args,fmt);
  vsprintf(buff,fmt,args);
  print_at(cursor_x,cursor_y,buff);
  va_end(args);
}

void print_at(int x, int y, char *text) {
  int loop;

  for (loop=0;loop<strlen(text);loop++) {
   if(text[loop]=='\n') { 
     x=0;
     y++;
    if (y>=screen_y) {
      if(c99_do_scroll) {
        scroll_text(0,0,screen_x,screen_y,2);
        y=screen_y-2;
        x=0;
      } else {
        y=0; x=0;
      }
    }
     cursor_x=x;
     cursor_y=y;
     continue;
   } 
   video_ram[x][y].ascii=text[loop];
   video_ram[x][y].fg=cursor_fg;
   video_ram[x][y].bg=cursor_bg;
    x++;
    if (x>=screen_x) { x=0; if(c99_do_wrap) y++; }
    if (y>=screen_y) {
      if(c99_do_scroll) {
        scroll_text(0,0,screen_x,screen_y,2);
        y=screen_y-2;
        x=0;
      } else {
        x=0; y=0;
      }
    }
    //we should really check the y here, too, but oh well..
    //update the cursor, just in case
    cursor_x=x;
    cursor_y=y;
  }
  if(c99_do_render && sys_render_begin()) {    
    render_screen();
    sys_render_finish();
  }
}

void locate(int col, int row) {
  cursor_x=col;
  cursor_y=row;
}

void scroll_text(int x1, int y1, int x2, int y2, int rows) {
  int x,y;

  for(y=y1;y<y2-rows;y++) {
    for(x=x1;x<x2;x++) {
      video_ram[x][y].ascii=video_ram[x][y+rows].ascii;
      video_ram[x][y].fg=video_ram[x][y+rows].fg;
      video_ram[x][y].bg=video_ram[x][y+rows].bg;
    }
  }
  for(y=y2-rows;y<y2;y++) {
    for(x=0;x<x2;x++) {
      video_ram[x][y].ascii=0;
      video_ram[x][y].fg=cursor_fg; 
      video_ram[x][y].bg=cursor_bg;
    }
  }
}

void getxy(int *x, int *y) {
  *x=cursor_x;
  *y=cursor_y;
}

char read_screen_ascii(int x, int y) {
  return video_ram[x][y].ascii;
}

int read_screen_fg(int x, int y) {
  return video_ram[x][y].fg;
}

int read_screen_bg(int x, int y) {
  return video_ram[x][y].bg;
}

void clear_area(int x1, int y1, int x2, int y2) {
	int x,y;
	for(y=y1;y<y2;y++) {
		for(x=x1;x<x2;x++) {
			video_ram[x][y].ascii=0;
			video_ram[x][y].fg=cursor_fg;
			video_ram[x][y].bg=cursor_bg;
		}
	}
}

void clear_screen() {
	int x,y;
	for(y=0;y<screen_y;y++) {
		for(x=0;x<screen_x;x++) {
			video_ram[x][y].ascii=0;
			video_ram[x][y].fg=cursor_fg;
			video_ram[x][y].bg=cursor_bg;
		}
	}
  locate(0,0);
  if(c99_do_render&&sys_render_begin()) {
    render_screen();
    sys_render_finish();
  }
}

void color(int fg, int bg) {
  cursor_fg=fg;
  cursor_bg=bg;
}

int get_fg() {
  return cursor_fg;
}

int get_bg() {
  return cursor_bg;
}

