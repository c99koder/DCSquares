#ifdef __cplusplus
extern "C" {
#endif

typedef void (*inputbox_click_callback)(int win_id, int but_id);

struct C99InputBox {
  int win_id,but_id,x,y,xsize,ysize;
  char label[100];
  int pos;
  int cur;
  struct C99InputBox *next;
  inputbox_click_callback onClick;
};

int c99_create_inputbox(int win_id, int x, int y, int xsize, int ysize, char *label);
void c99_draw_inputbox(struct C99InputBox *current);
void c99_update_inputbox(struct C99InputBox *current);
void set_inputbox_click(int but_id, inputbox_click_callback onClick);
void set_inputbox_text(int win_id, int but_id, char *text);
char *get_inputbox_text(int win_id, int but_id);

#ifdef __cplusplus
}
#endif