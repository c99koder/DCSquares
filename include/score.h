#ifndef DREAMCAST
#define uint32 unsigned long
#endif

struct score_list_node {
	char name[32];
	uint32 score;
	uint32 combo;
	uint32 time;
	uint32 level;
	struct score_list_node *next;
};

void encode(int score, int size, char *text);
void encrypt(int seed, unsigned char *text, unsigned char *out);
char *build_code(int score, int squares, int combo, int level);
void submit_code(char *s, char *username, char *password);
int invalid_code(char *text);

void score_list_init();
void score_list_insert(char *name, uint32 score, uint32 combo, uint32 time, uint32 level);
int score_list_rank(uint32 score);
int score_list_size();
void save_scores();

extern struct score_list_node *score_list_head;