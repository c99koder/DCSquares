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

using namespace Tiki;
using namespace Tiki::GL;

#include "playField.h"
#include "squares.h"
#include "game.h"
#include "level.h"
#include "rand.h"

extern int score[],squares[],powerup_mode;
squarelist *player[MAX_PLAYERS];

playField::playField(bool game) {
	destroy_list();
	init_genrand(Time::gettime()*1000);
		
	m_game = game;
}

playField::~playField() {
};

void playField::draw(ObjType list) {
	if(list==Trans) {
		render_squares(getAlpha(),m_game);
	}
}

void playField::nextFrame(uint64 tm) {
	float gt=tm/1000000.0f;
	
	add_squares(gt);
	update_squares(gt);
}