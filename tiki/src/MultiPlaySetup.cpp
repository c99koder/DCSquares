/*
 *  MultiPlaySetup.cpp
 *  TikiSquares
 *
 *  Created by Sam Steele on 1/14/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */


#include "Tiki/tiki.h"
#include "Tiki/genmenu.h"
#include "Tiki/texture.h"
#include "Tiki/drawables/banner.h"
#include "Tiki/anims/tintfader.h"
#include "Tiki/drawables/label.h"
#include "Tiki/drawables/cursor.h"

using namespace Tiki;
using namespace Tiki::GL;

#include "MultiPlaySetup.h"
#include "drawables/squaresHUD.h"
#include "level.h"
#include "squares.h"
#include "theme.h"

extern RefPtr<Font> fnt;

extern themeinfo_t themeinfo;

MultiPlaySetup::MultiPlaySetup() : DCSMenu(false) {
	RefPtr<Label> l;
	l=new Label(fnt,"Multiplayer Setup",24,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2 + 2,themeinfo.title_y+42,99.99));
	l->setTint(Color(0.6,0,0,0));
	m_scene->subAdd(l);
	l=new Label(fnt,"Multiplayer Setup",24,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+40,100));
	m_scene->subAdd(l);

	m_scene->subAdd(new TikiCursor);
	
	ml = new menuList(fnt);
	ml->setSize(252,122);
	ml->setTranslate(Vector(320,240,20));
	ml->setTextColor(Color(1,1,1));
	ml->addItem("10 square match");
	ml->addItem("20 square match");
	ml->addItem("5000 point match");
	ml->addItem("10000 point match");
	ml->addItem("Survival");
	m_scene->subAdd(ml);

	l=new Label(fnt,"Use the d-pad or mouse to select a game type,",16,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+themeinfo.title_h-32,100));
	m_scene->subAdd(l);
	l=new Label(fnt,"and press start or click to continue.",16,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+themeinfo.title_h-16,100));
	m_scene->subAdd(l);
	
	init();
	m_menuItems=5;
}

void MultiPlaySetup::init() {
	DCSMenu::init();
	
	ml->selectItem(0);
	m_selection=0;
	m_repeatDelay=0;
}

void MultiPlaySetup::inputEvent(const Event & evt) {
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

