/*
 *  level.cc
 *  DCSquares
 *
 *  Created by Sam Steele on 5/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#define IN_LEVEL
#include "squares.h"
#include "level.h"

level_node *current_level=NULL;
level_node *level_list_head=NULL;
level_node *level_list_tail=NULL;
level_node *free_play=NULL;
level_node *multi_play=NULL;
extern int score[MAX_PLAYERS],squares[MAX_PLAYERS];
int level_cnt=1;

level_node *create_level(int win_mode,int lose_mode) {
	level_node *tmp = new level_node;
	
	tmp->level=level_cnt++;
	tmp->win_mode=win_mode;
	tmp->lose_mode=lose_mode;
	tmp->score=0;
	tmp->time=0;
	tmp->squares=0;
	tmp->tickval=1.0f; //1.0
	tmp->min_tickval=0.2;
	tmp->speedval=1.2f; //1.2
	tmp->max_speedval=6;
	tmp->scoreval=100; //100
	tmp->max_scoreval=1000;
	tmp->power_size=1;
	tmp->power_score=1;
	tmp->power_evil=1;
	tmp->players=1;
	tmp->net=0;	
	tmp->next=NULL;
	
	if(level_list_tail!=NULL) 
		level_list_tail->next=tmp;
	level_list_tail=tmp;
	if(level_list_head==NULL) 
		level_list_head=tmp;
	
	return tmp;
}

void levels_init() {
	level_node *tmp;
	
	free_play=new level_node;
	free_play->level=0;
	free_play->win_mode=0;
	free_play->lose_mode=MODE_ENEMY;
	free_play->score=0;
	free_play->time=0;
	free_play->squares=0;
	free_play->tickval=1.0f; //1.0
	free_play->min_tickval=0.2;
	free_play->speedval=1.2f; //1.2
	free_play->max_speedval=6;
	free_play->scoreval=100; //100
	free_play->max_scoreval=1000;
	free_play->power_speed=1;
	free_play->power_size=1;
	free_play->power_score=1;
	free_play->power_evil=1;	
	free_play->net=0;
	free_play->players=1;
	free_play->next=NULL;
	
	multi_play=new level_node;
	multi_play->level=0;
	multi_play->win_mode=MODE_SQUARES;
	multi_play->lose_mode=MODE_TIME;
	multi_play->score=20000;
	multi_play->time=60*5;
	multi_play->squares=30;
	multi_play->tickval=1.0f; //1.0
	multi_play->min_tickval=0.6;
	multi_play->speedval=1.2f; //1.2
	multi_play->max_speedval=4;
	multi_play->scoreval=100; //100
	multi_play->max_scoreval=1000;
	multi_play->power_speed=1;
	multi_play->power_size=1;
	multi_play->power_score=1;
	multi_play->power_evil=1;	
	multi_play->net=0;
	multi_play->players=2;
	multi_play->next=NULL;
	
	//Level 1:
	//Collect 20 squares in 120 seconds
	//No powerups
	tmp=create_level(MODE_SQUARES,MODE_TIME|MODE_ENEMY);
	tmp->squares=20;
	tmp->time=120;
	tmp->power_speed=0;
	tmp->power_size=0;
	tmp->power_score=0;
	tmp->power_evil=0;

	//Level 2:
	//Collect 40 squares in 120 seconds
	//Score powerups
	tmp=create_level(MODE_SQUARES,MODE_TIME|MODE_ENEMY);
	tmp->squares=40;
	tmp->time=120;
	tmp->power_speed=0;
	tmp->power_size=0;
	tmp->power_score=1;
	tmp->power_evil=0;
	
	//Level 3:
	//Collect 60 squares in 120 seconds
	//Score and size powerups
	tmp=create_level(MODE_SQUARES,MODE_TIME|MODE_ENEMY);
	tmp->squares=60;
	tmp->time=120;
	tmp->power_speed=0;
	tmp->power_size=1;
	tmp->power_score=1;
	tmp->power_evil=0;
	
	//Level 4:
	//Earn 5000 points
	//Score, size, and speed powerups
	tmp=create_level(MODE_SCORE,MODE_ENEMY);
	tmp->score=5000;
	//tmp->time=10;
	tmp->power_speed=1;
	tmp->power_size=1;
	tmp->power_score=1;
	tmp->power_evil=0;
	
	//Level 5:
	//Survive for 30 seconds
	//All powerups
	tmp=create_level(MODE_TIME,MODE_ENEMY);
	tmp->time=30;
	tmp->speedval=4;
	tmp->tickval=0.6;
	tmp->scoreval=1000;
		
	current_level = free_play;
}

int check_win(float gt, int p) {
	if(current_level->lose_mode & MODE_TIME && (current_level->time - gt < 0)) {
		//printf("time: lose\n");
		return -1;
	}
	if(current_level->win_mode & MODE_TIME && (current_level->time - gt < 0)) {
		//printf("time: win\n");
		return 1;
	}
	if(current_level->lose_mode & MODE_SCORE && (current_level->score <= score[p])) {
		//printf("score: lose\n");
		return -1;
	}
	if(current_level->win_mode & MODE_SCORE && (current_level->score <= score[p])) {
		//printf("score: win\n");
		return 1;
	}
	if(current_level->lose_mode & MODE_SQUARES && (current_level->squares <= squares[p])) {
		//printf("squares: lose\n");
		return -1;
	}
	if(current_level->win_mode & MODE_SQUARES && (current_level->squares <= squares[p])) {
		//printf("squares: win\n");
		return 1;
	}
	return 0;
}