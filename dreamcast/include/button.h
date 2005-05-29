#ifdef __cplusplus
extern "C" {
#endif

typedef void (*button_click_callback)(int win_id, int but_id);

struct C99Button {
  int win_id,but_id,x,y,xsize,ysize,highlight,def;
  char *label;
  struct C99Button *next;
  button_click_callback onClick;
};

int c99_create_button(int win_id, int x, int y, int xsize, int ysize, char *label);
void c99_draw_button(struct C99Button *current);
void set_button_click(int win_id, int but_id, button_click_callback onClick);
void c99_button_set_def(int win_id, int but_id);

#ifdef __cplusplus
}
#endif