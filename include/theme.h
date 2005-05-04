struct themeinfo_t {
  char theme[100];
  char bg_auth[100];
  char sfx_auth[100];
	char music_auth[100];
  int good_r,good_g,good_b;
  int evil_r,evil_g,evil_b;
	int text_r,text_g,text_b;
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