#ifdef __cplusplus
extern "C" {
#endif

char read_screen_ascii(int x, int y);
int read_screen_fg(int x, int y);
int read_screen_bg(int x, int y);
void print_at(int x, int y, char *text);
void clear_screen();
void color(int fg, int bg);
void locate(int col, int row);
void c99_printf(const char *fmt, ...);
void render_screen();
void init_screen();
void scroll_text(int x1, int y1, int x2, int y2, int rows);
void video_refresh();
void clear_area(int x1, int y1, int x2, int y2);
void getxy(int *x, int *y);
void c99_resize_screen(int x, int y, int xsize, int ysize);
void set_border(int color);
int get_screen_width();
int get_screen_height();
void set_do_scroll(int val);
void set_do_wrap(int val);
void do_render(int mode);
#ifdef __cplusplus
}
#endif

