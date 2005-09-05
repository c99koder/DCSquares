#ifdef __cplusplus
extern "C" {
#endif

void set_echo(int state);
void set_show_cursor(int state);
void set_keyblock(int state);
void get_string(char *buf);
void set_timeout(int tm);
int getch();
int kbhit();
void push_key(char key);
char pop_key();
void c99_mouse_init();
void read_mouse(int num, int *x, int *y, int *lmb);
int detect_mouse();
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_LEFT 3
#define MOVE_RIGHT 4
#define START_BTN 5
#define FIRE_BTN 6
#define QUIT_BTN 7
#define BUTTON_X 8
#define BUTTON_Y 9
#define FIRE_UP 10
#define FIRE_DOWN 11
#define FIRE_LEFT 12
#define FIRE_RIGHT 13

int poll_game_device(int num);

#ifdef __cplusplus
}
#endif
