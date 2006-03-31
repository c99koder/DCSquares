/*
 *  DCSMenu.cpp
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/20/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tiki/tiki.h"
#include "Tiki/genmenu.h"
#include "Tiki/texture.h"
#include "Tiki/anims/tintfader.h"

using namespace Tiki;
using namespace Tiki::GL;

#include "menus/DCSMenu.h"
#include "squares.h"
#include "level.h"

extern int scoreval;
extern float tickval;
extern float power;
extern float speedval;
extern float gt,ogt;
extern int effect_type;
extern int powerup_mode;
extern RefPtr<Texture> bg_tex;

DCSMenu::DCSMenu(bool game) {
	GenericMenu::GenericMenu();
	
	bg = new squaresBg(bg_tex,game);
	m_scene->subAdd(bg);
	
	pf=new playField(game);
	if(!game) pf->setAlpha(0.6);
	m_scene->subAdd(pf);
	
	box=new Box(640,480);
	box->setTranslate(Vector(320,240,1000));
	m_scene->subAdd(box);
	
	init();
}

DCSMenu::~DCSMenu() {
}

void DCSMenu::init() {
	tickval=current_level->tickval; //1.0
	speedval=current_level->speedval; //1.2
	scoreval=current_level->scoreval; //100
	power=0;
	powerup_mode=-1;
	effect_type=-1;
}

void DCSMenu::FadeIn() {
	box->fadeIn();
}

void DCSMenu::FadeOut() {
	box->fadeOut();
}

void DCSMenu::inputEvent(const Event & evt) {
	switch(evt.type) {
		case Event::EvtQuit:
			quitNow();
			break;
	}
}
