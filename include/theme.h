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
	int score_x, score_y, score_size;
	char squares_caption[40];
	int squares_x, squares_y, squares_size;
	char combo_caption[40];
	int combo_x,combo_y,combo_size;
};


void render_bg(int tex, float fade);
void render_bg_game(int tex, float fade);
void render_poly(int size, int tex, float fade);
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