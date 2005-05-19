/*
 *  level.cc
 *  DCSquares
 *
 *  Created by Sam Steele on 5/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdlib.h>
#define IN_LEVEL
#include "level.h"

level_node *current_level=NULL;
level_node *level_list_head=NULL;
level_node *level_list_tail=NULL;
level_node *free_play=NULL;
extern int score,squares;

level_node *create_level(int win_mode,int lose_mode) {
	level_node *tmp = new level_node;
	
	tmp->win_mode=win_mode;
	tmp->lose_mode=lose_mode;
	tmp->score=0;
	tmp->time=0;
	tmp->squares=0;
	tmp->tickval=1.0f; //1.0
	tmp->speedval=1.2f; //1.2
	tmp->scoreval=100; //100	
	tmp->power_speed=1;
	tmp->power_size=1;
	tmp->power_score=1;
	tmp->power_evil=1;
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
	
	free_play->win_mode=0;
	free_play->lose_mode=0;
	free_play->score=0;
	free_play->time=0;
	free_play->tickval=1.0f; //1.0
	free_play->speedval=1.2f; //1.2
	free_play->scoreval=100; //100	
	free_play->next=NULL;
	
	//Level 1:
	//Collect 20 squares in 120 seconds
	//No powerups
	tmp=create_level(MODE_SQUARES,MODE_TIME);
	tmp->squares=20;
	tmp->time=120;
	tmp->power_speed=0;
	tmp->power_size=0;
	tmp->power_score=0;
	tmp->power_evil=0;

	//Level 2:
	//Collect 40 squares in 120 seconds
	//Score powerups
	tmp=create_level(MODE_SQUARES,MODE_TIME);
	tmp->squares=40;
	tmp->time=120;
	tmp->power_speed=0;
	tmp->power_size=0;
	tmp->power_score=1;
	tmp->power_evil=0;
	
	//Level 3:
	//Collect 60 squares in 120 seconds
	//Score and size powerups
	tmp=create_level(MODE_SQUARES,MODE_TIME);
	tmp->squares=60;
	tmp->time=120;
	tmp->power_speed=0;
	tmp->power_size=1;
	tmp->power_score=1;
	tmp->power_evil=0;
	
	//Level 4:
	//Earn 5000 points
	//Score, size, and speed powerups
	tmp=create_level(MODE_SCORE,0);
	tmp->score=5000;
	tmp->time=10;
	tmp->power_speed=1;
	tmp->power_size=1;
	tmp->power_score=1;
	tmp->power_evil=0;
	
	//Level 5:
	//Survive for 30 seconds
	//All powerups
	tmp=create_level(MODE_TIME,0);
	tmp->time=30;
	tmp->speedval=4;
	tmp->tickval=0.6;
	tmp->scoreval=1000;
		
	current_level = level_list_head;
}

int check_win(float gt) {
	if(current_level->lose_mode & MODE_TIME && (current_level->time - gt < 0)) {
		return -1;
	}
	if(current_level->win_mode & MODE_TIME && (current_level->time - gt < 0)) {
		return 1;
	}
	if(current_level->lose_mode & MODE_SCORE && (current_level->score <= score)) {
		return -1;
	}
	if(current_level->win_mode & MODE_SCORE && (current_level->score <= score)) {
		return 1;
	}
	if(current_level->lose_mode & MODE_SQUARES && (current_level->squares <= squares)) {
		return -1;
	}
	if(current_level->win_mode & MODE_SQUARES && (current_level->squares <= squares)) {
		return 1;
	}
	return 0;
}