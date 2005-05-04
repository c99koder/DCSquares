#include <SDL/SDL.h>
#include <stdlib.h>
#include "input.h"
#include "screen.h"
#include "sys.h"

int echo=1;
int keyblock=1;
float kbtm=0;
int show_cursor=1;
extern bool keys[SDLK_LAST];
int c99_mouse_sticky=0;

char last=0;

struct keyqueue_t {
  char key;
  struct keyqueue_t *prev;
  struct keyqueue_t *next;
};

struct keyqueue_t *keyqueue_head=NULL;
struct keyqueue_t *keyqueue_tail=NULL;

void push_key(char key) {
  struct keyqueue_t *tmp=(struct keyqueue_t *)malloc(sizeof(struct keyqueue_t));
	if(keys[SDLK_LSHIFT]||keys[SDLK_RSHIFT]) {
		switch(key) {
			case '`':
				key='~';
				break;
			case '0':
				key=')';
				break;
			case '1':
				key='!';
				break;
			case '2':
				key='@';
				break;
			case '3':
				key='#';
				break;
			case '4':
				key='$';
				break;
			case '5':
				key='%';
				break;
			case '6':
				key='^';
				break;
			case '7':
				key='&';
				break;
			case '8':
				key='*';
				break;
			case '9':
				key='(';
				break;
			case '-':
				key='_';
				break;
			case '=':
				key='+';
				break;
			case '[':
				key='{';
				break;
			case ']':
				key='}';
				break;
			case '\\':
				key='|';
				break;
			case ';':
				key=':';
				break;
			case '\'':
				key='\"';
				break;
			case ',':
				key='<';
				break;
			case '.':
				key='>';
				break;
			case '/':
				key='?';
				break;
		}
  }
  tmp->key=key;
  tmp->next=keyqueue_head;
  tmp->prev=NULL;
  if(keyqueue_head!=NULL) {
    keyqueue_head->prev=tmp;
  }
  if(keyqueue_tail==NULL) {
    keyqueue_tail=tmp;
  }
  keyqueue_head=tmp;

}

char pop_key() {
  char ret;
  struct keyqueue_t *current=keyqueue_tail;
  if(keyqueue_tail==NULL) return 0;
  ret=keyqueue_tail->key;
  if(keyqueue_tail->prev!=NULL) {
    keyqueue_tail->prev->next=NULL;
  }
  keyqueue_tail=keyqueue_tail->prev;

  free(keyqueue_tail);
  return ret;
}

int kbhit() {
  if(keyqueue_tail!=NULL) return 1; else return 0;
}

int getch() {
  return pop_key();
}

void set_show_cursor(int state) { show_cursor=state; }
void set_echo(int state) { echo=state; }
void set_keyblock(int state) { keyblock=state; }
void set_timeout(int tm) { 
}
void get_string(char *buf) {
}

void c99_mouse_init() {}
void read_mouse(int *x, int *y, int *lmb) {
	int x1,y1;

    *lmb=SDL_GetMouseState(&x1, &y1)&SDL_BUTTON(1);
		*x=x1;
#ifdef MACOS
		*y=480-y1;
#else
		*y=y1;
#endif
	if(*lmb==0) c99_mouse_sticky=0;
	if(*lmb==1 && c99_mouse_sticky) *lmb=0;
}

int poll_game_device(int num) {
  int m=0;
  sys_render_begin();
  if (keys[SDLK_RETURN]) {
    return START_BTN;
  }
  else if (keys[SDLK_ESCAPE]) {
    return QUIT_BTN;
  }
  else if (keys[SDLK_SPACE]) {
    return FIRE_BTN;
  }
  else if (keys['t']) {
    return BUTTON_X;
  }
  if(keys[SDLK_LSHIFT]||keys[SDLK_RSHIFT]) {
    m=9;
  } else {
    m=0;
  }
  switch(num) {
    case 0:
    if (keys[SDLK_LEFT]) {
      return MOVE_LEFT+m;
    }
    else if (keys[SDLK_RIGHT]) {
      return MOVE_RIGHT+m;
    }
    else if (keys[SDLK_UP]) {
      return MOVE_UP+m;
    }
    else if (keys[SDLK_DOWN]) {
      return MOVE_DOWN+m;
    }
                break;
        case 1:
                if(keys['A']) {
                        return MOVE_LEFT;
                }
                else if (keys['D']) {
                        return MOVE_RIGHT;
                }
                else if (keys['W']) {
                        return MOVE_UP;
                }
                else if (keys['S']) {
                        return MOVE_DOWN;
                }
                break;
        }
        return 0;
}
