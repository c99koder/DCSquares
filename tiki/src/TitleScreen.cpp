/*
 *  TitleScreen.cc
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/18/05.
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

#include "TitleScreen.h"
#include "drawables/squaresHUD.h"
#include "level.h"
#include "squares.h"
#include "theme.h"

extern RefPtr<Font> fnt;

extern int logo_tex;
extern themeinfo_t themeinfo;
extern int score[],combo[];

TitleScreen::TitleScreen() : DCSMenu(false) {
	m_scene->subAdd(new squaresHUD(0));
	m_scene->subAdd(new TikiCursor);

	ml = new menuList(fnt);
	ml->setSize(150,132);
	ml->setTranslate(Vector(320,240,20));
	ml->setTextColor(Color(1,1,1));
	ml->addItem("New Game");
	ml->addItem("Free Play");
	ml->addItem("Multiplayer");
	ml->addItem("How to Play");
	ml->addItem("Highscores");
	ml->addItem("Quit");
	m_scene->subAdd(ml);
	
	init();
	m_menuItems=6;
}

void TitleScreen::init() {
	DCSMenu::init();
	
	for(int p=0; p<MAX_PLAYERS; p++) {
		score[p]=0;
		combo[p]=0;
	}
	ml->selectItem(0);
	m_selection=0;
	m_repeatDelay=0;
}

void TitleScreen::inputEvent(const Event & evt) {
	switch(evt.type) {
		case Event::EvtQuit:
			quitNow();
			break;
		case Event::EvtKeyUp:
			m_repeatDelay=0;
			break;
		case Event::EvtKeypress:
			switch(evt.key) {
				case Event::KeySelect:
				case 32:
					FadeOut();
					startExit();
					m_exitSpeed=1.0f/30.0f;
					break;
				case Event::KeyUp:
					if(m_repeatDelay==0) {
						m_selection--;
						if(m_selection<0) {
							m_selection=0;
						} else {
							ml->selectItem(m_selection);
							m_repeatDelay=2;
						}
					} else {
						m_repeatDelay--;
					}
					break;
				case Event::KeyDown:
					if(m_repeatDelay==0) {
						m_selection++;
						if(m_selection>=m_menuItems) {
							m_selection=m_menuItems-1;
						} else {
							ml->selectItem(m_selection);
							m_repeatDelay=2;
						}
					} else {
						m_repeatDelay--;
					}
					break;
			}
			break;
	}
}
