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
#include "drawables/squaresHUD.h"
#include "level.h"
#include "squares.h"
#include "theme.h"
#include "squarenet.h"
#include "net.h"

extern RefPtr<Font> fnt;

extern int logo_tex;
extern themeinfo_t themeinfo;

extern int score[],combo[],squares[],maxcombo[],powerup_mode;
extern squarelist *player[];
extern squarelist *netplayer;
extern float game_gt;
bool gameFadingOut;
extern int dmloser;

GamePlay::GamePlay() : DCSMenu(true) {
	hud=new squaresHUD(1);
	m_scene->subAdd(hud);
	init();
}

void GamePlay::init() {
	DCSMenu::init();
	
	hud->init();
	gameFadingOut=false;
	dmloser=-1;
	
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
				if(current_level->net) {
					netplayer=player[p];
					netplayer->type=PLAYER_NET;
				}
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
				dmloser=p;
				gameFadingOut=true;
				FadeOut();
				startExit();
				m_exitSpeed=1.0f/30.0f;
			} else {
				if(current_level->win_mode & MODE_SCORE) {
					score[p]-=100;
				}
				if(current_level->win_mode & MODE_SQUARES) {
					squares[p]-=(current_level->squares/10)+1;
					if(squares[p]<0) squares[p]=0;
				}
			}
		}
		if(check_win(game_gt,p) != 0) {
			gameFadingOut=true;
			FadeOut();
			startExit();
			m_exitSpeed=1.0f/30.0f;
		}
	}
}

void GamePlay::inputEvent(const Event & evt) {
	switch(evt.type) {
		case Event::EvtQuit:
			quitNow();
			break;
		case Event::EvtMouseMove:
			if(!m_exiting) {
				player[evt.port]->x=evt.x;
				player[evt.port]->y=evt.y;
				if(current_level->net) {
					snGamePlayerMove m;
					m.x=evt.x;
					m.y=evt.y;
					game_send(CHAN_GAME,GAME_PLAYERMOVE,sizeof(m),&m);
				}
			}
			break;
		case Event::EvtBtnPress:
			switch(evt.btn) {
				case Event::BtnUp:
					player[evt.port]->yv=-6.0f;
					break;
				case Event::BtnDown:
					player[evt.port]->yv=6.0f;
					break;
				case Event::BtnLeft:
					player[evt.port]->xv=-6.0f;
					break;
				case Event::BtnRight:
					player[evt.port]->xv=6.0f;
					break;
			}
			break;
		case Event::EvtBtnRelease:
			switch(evt.btn) {
				case Event::BtnUp:
				case Event::BtnDown:
					player[evt.port]->yv=0;
					break;
				case Event::BtnLeft:
				case Event::BtnRight:
					player[evt.port]->xv=0;
					break;
			}
			break;
		case Event::EvtAxis:
			if(!m_exiting) {
				switch(evt.axis) {
					case Event::AxisX:
						player[evt.port]->xv=(evt.axisValue/8.0f);
						break;
					case Event::AxisY:
						player[evt.port]->yv=(evt.axisValue/8.0f);
						break;
				}
			}
			break;
	}
}
