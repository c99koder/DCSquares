/*
 *  playField.cpp
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/gl.h>
#include <Tiki/drawable.h>
#include <Tiki/tikitime.h>
#include <Tiki/texture.h>

using namespace Tiki;
using namespace Tiki::GL;

#include "drawables/playField.h"
#include "squares.h"
#include "game.h"
#include "level.h"
#include "rand.h"

extern int score[],squares[],powerup_mode;
squarelist *player[MAX_PLAYERS];
extern bool gameFadingOut;

#ifdef DREAMCAST
extern "C" {
	void update_lcds();
}
#endif

playField::playField(bool game) {
	init_genrand(Time::gettime()*1000);
		
	m_game = game;
	if(game) m_countdown = 1.5f; else m_countdown=0;
}

playField::~playField() {
	destroy_list();
};

void playField::draw(ObjType list) {
	if(list==Trans) {
		render_squares(getAlpha(),m_game);
	}
}

void playField::nextFrame(uint64 tm) {
	float gt=tm/1000000.0f;
	if(m_countdown>0) m_countdown -= gt;
	
	if(m_countdown<=0) add_squares(gt);
	update_squares(gt);
	
#ifdef DREAMCAST
	update_lcds();
#endif
	
	if(m_game && gameFadingOut) {
		for(int p=0; p<current_level->players;p++) {
			player[p]->xv=0;
			player[p]->yv=0;
			player[p]->size+=200.0f / 30.0f;
		}
	}
}
