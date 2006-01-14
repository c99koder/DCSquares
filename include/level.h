/*
 *  level.h
 *  DCSquares
 *
 *  Created by Sam Steele on 5/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
#define MODE_SCORE 1
#define MODE_TIME 2
#define MODE_SQUARES 4
#define MODE_ENEMY 8
	
struct level_node {
	int level;
	int win_mode;
	int lose_mode;
	int score;
	int time;
	int squares;
	bool net;
	bool power_speed;
	bool power_size;
	bool power_score;
	bool power_evil;
	float tickval;
	float min_tickval;
	float speedval;
	float max_speedval;
	float scoreval;
	float max_scoreval;
	int players;
	struct level_node *next;
};

#ifndef IN_LEVEL
extern level_node *current_level;
extern level_node *level_list_head;
extern level_node *free_play;
extern level_node *multi_play;
#endif

level_node *create_level(int win_mode, int lose_mode);
void levels_init();
int check_win(float gt, int p);
