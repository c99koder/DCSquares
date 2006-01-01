/*
 *  squaresBg.cpp
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

#include "squaresBg.h"
#include "squares.h"
#include "theme.h"

squaresBg::squaresBg(int tex, bool game) {
	m_game = game;
	m_tex = tex;
}

squaresBg::~squaresBg() {
};

void squaresBg::draw(ObjType list) {
	if(list==Opaque) {
		if(m_game)
			render_bg_game(m_tex,getAlpha());
		else
			render_bg_title(m_tex,getAlpha());
	}
}
