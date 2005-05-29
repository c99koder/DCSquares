#ifdef __cplusplus
extern "C" {
#endif

struct C99Label {
  int win_id,but_id,x,y;
  char label[100];
  struct C99Label *next;
};

int c99_create_label(int win_id, int x, int y, char *label);
void c99_draw_label(struct C99Label *current);
void c99_label_set_text(int win_id, int but_id, char *text);

#ifdef __cplusplus
}
#endif