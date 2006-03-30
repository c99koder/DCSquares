#ifdef __cplusplus
extern "C" {
#endif

void load_font(char *filename);
void draw_char(unsigned char a, float x, float y, float z);
void draw_string(char *a, float x, float y, float z);
float get_font_width();
float get_font_height();
void set_font_width(float width);
void set_font_height(float height);

extern float font_width;
extern float font_height;

#ifdef __cplusplus
}
#endif

