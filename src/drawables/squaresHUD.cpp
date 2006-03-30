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
#include <Tiki/texture.h>

using namespace Tiki;
using namespace Tiki::GL;

#include "drawables/squaresHUD.h"
#include "squares.h"
#include "hud.h"

float game_gt;

squaresHUD::squaresHUD(int mode) {
	m_mode = mode;
	init();
}

void squaresHUD::init() {
	m_gt=m_ogt=0;
}

squaresHUD::~squaresHUD() {
};

void squaresHUD::nextFrame(uint64 tm) {
	m_ogt=m_gt;
	m_gt+=tm/1000000.0f;
	if(m_mode==1) game_gt=m_gt;
}

void squaresHUD::draw(ObjType list) {
	if(list==Trans) {
		glLoadIdentity();
		switch(m_mode) {
			case 0:
				render_title(m_gt);
				break;
			case 1:
				render_score(m_gt);
				break;
			case 2:
				render_win(game_gt,m_gt-m_ogt);
				break;
		}
	}
}
