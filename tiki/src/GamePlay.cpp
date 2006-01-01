/*
 *  GamePlay.cpp
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/20/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tiki/tiki.h"
#include "Tiki/genmenu.h"
#include "Tiki/texture.h"
#include "Tiki/drawables/banner.h"
#include "Tiki/anims/tintfader.h"
#include "Tiki/drawables/cursor.h"

using namespace Tiki;
using namespace Tiki::GL;

#include "GamePlay.h"
#include "squaresHUD.h"
#include "level.h"
#include "squares.h"
#include "theme.h"

extern RefPtr<Font> fnt;

extern int logo_tex;
extern themeinfo_t themeinfo;

extern int score[],combo[],squares[],maxcombo[],powerup_mode;
extern squarelist *player[];
extern float gt;

GamePlay::GamePlay() : DCSMenu(true) {
	m_scene->subAdd(new squaresHUD(1));
}

void GamePlay::init() {
	DCSMenu::init();
	
	for(int p=0; p<current_level->players; p++) {
		player[p]=create_square(((640/(current_level->players+1))*(p+1))-4,(480/2)-4,6,p);
		squares[p]=0;
		maxcombo[p]=0;
		switch(p) {
			case 0:
				player[p]->r=0;//200.0f/255.0f;			
				player[p]->g=0;//130.0f/255.0f;
				player[p]->b=0;//0.0f/255.0f;
				break;
			case 1:
				player[p]->r=130.0f/255.0f;
				player[p]->g=0.0f/255.0f;			
				player[p]->b=200.0f/255.0f;
				break;
		}
	}
}

void GamePlay::controlPerFrame() {
	squarelist *c;
	
	for(int p=0; p< current_level->players; p++) {
		c=check_collide(player[p]);
		if(c!=NULL && ((c->type == ENEMY && powerup_mode!=INVINC) || (c->type == SCORE && powerup_mode==EVIL))) {
			if(current_level->lose_mode & MODE_ENEMY) {
				FadeOut();
				startExit();
				m_exitSpeed=1.0f/30.0f;
			} else {
				if(current_level->win_mode & MODE_SCORE) {
					score[p]-=100;
				}
				if(current_level->win_mode & MODE_SQUARES) {
					squares[p]-=10;
					if(squares[p]<0) squares[p]=0;
				}
			}
		}
		if(check_win(gt,p) != 0) {
			FadeOut();
			startExit();
			m_exitSpeed=1.0f/30.0f;
		}
	}
}

void GamePlay::inputEvent(const Event & evt) {
	switch(evt.type) {
		case Event::EvtQuit:
			break;
		case Event::EvtMouseMove:
			player[evt.port]->x=evt.x;
			player[evt.port]->y=evt.y;
			break;
	}
}
