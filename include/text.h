void draw_txt(int x, int y, char *text, float r, float g, float b, float a, int point);
int text_init(char *filename, int pointsize);
void set_status_text(char *text, float r, float g, float b);
float txt_width(char *text);
void set_font_size(int pointsize);
void text_set_z(float z);
void center(int x, int y,char *text, int point, float fade);
void center_shad(int y,char *text, int point, float fade, bool game=false);
void center_shad_rgb(int y,char *text, int point, float fade, float r, float g, float b, bool game=false);