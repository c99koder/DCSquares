struct themeinfo_t {
  char theme[100];
  char bg_auth[100];
  char sfx_auth[100];
	char music_auth[100];
	float scale;
  int good_r,good_g,good_b;
  int evil_r,evil_g,evil_b;
	int text_r,text_g,text_b;
	int game_x,game_y,game_w,game_h;
	int title_x,title_y,title_w,title_h;
	int stat_w,stat_h,stat_size;
	char invinc[40];
	char plus1000[40];
	char slowdown[40];
	char minisquare[40];
	char score[40];
	char powershadow[40];
	char bigsquare[40];
	char minus1000[40];
	char speedup[40];
	char evil[40];
	char enemy[40];
	int score_x[MAX_PLAYERS], score_y[MAX_PLAYERS], score_size, score_bg_x[MAX_PLAYERS], score_bg_y[MAX_PLAYERS];
	char score_caption[40],score_title[40];
	int squares_x[MAX_PLAYERS], squares_y[MAX_PLAYERS], squares_size, squares_bg_x[MAX_PLAYERS], squares_bg_y[MAX_PLAYERS];
	char squares_caption[40],squares_title[40];	
	char combo_caption[40],combo_title[40];
	int combo_x[MAX_PLAYERS],combo_y[MAX_PLAYERS],combo_size, combo_bg_x[MAX_PLAYERS], combo_bg_y[MAX_PLAYERS];
	int time_x,time_y,time_size,time_bg_x,time_bg_y;
	char time_caption[40],time_title[40];
	int logo_w,logo_h;
};


void render_bg(int tex, float fade);
void render_bg_game(int tex, float fade);
void render_bg_title(int tex, float fade);
void render_poly(int size, int tex, float fade);
void render_box(int x, int y, int w, int h, int tex, float fade);
char *theme_dir(char *filename);
int load_theme(char *theme, int sfx);
void unload_theme();

#ifdef MACOS
#define SND_TITLE 0
#define SND_BGM 1
#define SND_COLLECT 2
#define SND_POWERUP 3
#define SND_POWERDOWN 4
#define SND_GAMEOVER 5
#endif