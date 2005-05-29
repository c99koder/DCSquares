#ifdef __cplusplus
extern "C" {
#endif

#include "button.h"
#include "listbox.h"
#include "inputbox.h"
#include "label.h"
typedef void (*window_click_callback)(int id, int x, int y);

struct C99Window {
  int id,x,y,xsize,ysize;
  char *title;
  window_click_callback onClick;
  struct C99Button *button_list;
  struct C99ListBox *listbox_list;
  struct C99InputBox *inputbox_list;
  struct C99Label *label_list;
  struct C99Window *next;
};

int c99_create_window(int x, int y, int xsize, int ysize, char *title);
void c99_draw_window(struct C99Window *current);
void set_window_click(int id, window_click_callback onClick);
struct C99Window *get_window(int id);
void delete_window(int win_id);

void render_windows();

#ifdef __cplusplus
}
#endif