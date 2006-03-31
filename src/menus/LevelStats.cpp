/*
 *  LevelStats.cpp
 *  TikiSquares
 *
 *  Created by Sam Steele on 1/1/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tiki/tiki.h"
#include "Tiki/genmenu.h"
#include <Tiki/texture.h>

using namespace Tiki;
using namespace Tiki::GL;

#include "menus/LevelStats.h"

LevelStats::LevelStats() : DCSMenu(false) {
	hud=new squaresHUD(2);
	m_scene->subAdd(hud);
	init();
}

void LevelStats::init() {
	DCSMenu::init();
	
	hud->init();
}

void LevelStats::inputEvent(const Event & evt) {
	switch(evt.type) {
		case Event::EvtQuit:
			quitNow();
			break;
		case Event::EvtBtnRelease:
			if(!m_exiting && (evt.btn==Event::BtnA || evt.btn==Event::BtnStart || evt.btn==Event::MouseLeftBtn)) {
				FadeOut();
				startExit();
				m_exitSpeed=1.0f/30.0f;
			}
			break;
	}
}
