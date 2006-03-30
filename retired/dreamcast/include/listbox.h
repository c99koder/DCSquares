#ifdef __cplusplus
extern "C" {
#endif

typedef void (*list_click_callback)(int win_id, int but_id);
typedef void (*list_change_callback)(int win_id, int but_id);

struct C99ListBox {
  int win_id,list_id,x,y,xsize,ysize,itemcnt,sel;
  char items[512][40];
  struct C99ListBox *next;
  list_click_callback onClick;
  list_change_callback onChange;
};

int c99_create_list(int win_id, int x, int y, int xsize, int ysize);
void c99_update_list(struct C99ListBox *current);
void c99_draw_list(struct C99ListBox *current);
void c99_list_set_click(int win_id, int but_id, list_click_callback onClick);
void c99_list_set_changed(int win_id, int list_id, list_change_callback cb);
void c99_list_add_item(int win_id, int list_id, char *text);
void c99_list_empty(int win_id, int list_id);
struct C99ListBox *get_list(int win_id, int list_id);

#ifdef __cplusplus
}
#endif
