/*
 *  squaresHUD.cpp
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/25/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/gl.h>
#include <Tiki/drawable.h>
#include <Tiki/tikitime.h>

using namespace Tiki;
using namespace Tiki::GL;

#include "squaresHUD.h"
#include "squares.h"
#include "hud.h"

float gt;
float ogt;
float game_gt;

squaresHUD::squaresHUD(int mode) {
	m_mode = mode;
	gt=ogt=0;
}

squaresHUD::~squaresHUD() {
	game_gt=gt;
};

void squaresHUD::nextFrame(uint64 tm) {
	ogt=gt;
	gt+=tm/1000000.0f;
}

void squaresHUD::draw(ObjType list) {
	if(list==Trans) {
		glLoadIdentity();
		switch(m_mode) {
			case 0:
				render_title(gt);
				break;
			case 1:
				render_score(gt);
				break;
			case 2:
				render_win(game_gt,gt-ogt);
				break;
		}
	}
}
