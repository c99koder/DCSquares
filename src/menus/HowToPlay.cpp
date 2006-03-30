/*
 *  HowToPlay.cpp
 *  TikiSquares
 *
 *  Created by Sam Steele on 1/22/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tiki/tiki.h"
#include "Tiki/genmenu.h"
#include <Tiki/drawables/label.h>
#include <Tiki/anims/tintfader.h>
#include <Tiki/texture.h>

using namespace Tiki;
using namespace Tiki::GL;

#include "HowToPlay.h"
#include "squares.h"
#include "score.h"
#include "theme.h"
#include "text.h"

extern RefPtr<Font> fnt;
extern themeinfo_t themeinfo;

#define WHITE_TEXT(X, Y, TEXT) \
	l=new Label(fnt,TEXT,18,false,false); \
	l->setTranslate(Vector(X+20,Y,0)); \
	m_help[m_page]->subAdd(l);

#define COLOR_TEXT(X, Y, TEXT) \
	l=new Label(fnt,TEXT,16,false,false); \
	l->setTranslate(Vector(X+20,Y,0)); \
	l->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f)); \
	m_help[m_page]->subAdd(l);

#define TITLE(TEXT) \
	l=new Label(fnt,TEXT,24,true,false); \
	l->setTranslate(Vector(themeinfo.title_w/2 + 2,2,-0.099)); \
	l->setTint(Color(0.6,0,0,0)); \
	m_help[m_page]->subAdd(l); \
	l=new Label(fnt,TEXT,24,true,false); \
	l->setTranslate(Vector(themeinfo.title_w/2,0,0)); \
	m_help[m_page]->subAdd(l);

#define BANNA(X, Y, TEXT, TEX) \
	b=new Banna(Drawable::Trans,TEX);\
	b->setTranslate(Vector(X,Y,0));\
	b->setSize(32,32);\
	m_help[m_page]->subAdd(b);\
	COLOR_TEXT(X+10, Y+8, TEXT);

#define BANNA_TOP(X, Y, TEXT, TEX) \
	b=new Banna(Drawable::Trans,TEX);\
	b->setTranslate(Vector(X,Y,0));\
	b->setSize(32,32);\
	m_help[m_page]->subAdd(b);\
	COLOR_TEXT(X+10, Y, TEXT);

extern Texture *score_tex;
extern Texture *enemy_tex;
extern Texture *invinc_tex;
extern Texture *slow_tex; 
extern Texture *mini_tex;
extern Texture *plus_tex;
extern Texture *evil_tex;
extern Texture *speed_tex;
extern Texture *minus_tex;
extern Texture *big_tex;


HowToPlay::HowToPlay() : DCSMenu(false) {
	RefPtr<Label> l;
	RefPtr<Banna> b;
	RefPtr<Box> plr;
	init();
	set_font_size(20);
	
	//-----------------------------
	m_page=0;
	m_help[m_page] = new Layer();
	m_help[m_page]->setTranslate(Vector(themeinfo.title_x,themeinfo.title_y+30,100));
	m_help[m_page]->setTint(Color(1,1,1,1));
	m_scene->subAdd(m_help[m_page]);
	
	TITLE("How to Play");
	
	WHITE_TEXT(0,50,"Overview");
	COLOR_TEXT(20,70,"The goal of DCSquares is to collect the green\n\
squares and avoid the red squares.  There are\n\
also several special items you may encouter\n\
during the game that will affect the gameplay.");
	
	WHITE_TEXT(0,158,"General Objects");

	plr=new Box(32,32);
	plr->setTranslate(Vector(40,180,0));
	plr->setTint(Color(0,0,0));
	m_help[m_page]->subAdd(plr);

	COLOR_TEXT(50,180,"Use your mouse or analog control stick to\n\
move this player around the screen.");
	
	BANNA_TOP(40,230,"Collecting these gives you points!  The\n\
amount you recieve per square depends on how\n\
long you've been playing the current round.",score_tex);

	BANNA_TOP(40,300,"Touching one of these will end your game!\n\
Avoid them at all cost!",enemy_tex);
	
	//-----------------------------
	m_page=1;
	m_help[m_page] = new Layer();
	m_help[m_page]->setTranslate(Vector(themeinfo.title_x,themeinfo.title_y+30,100));
	m_help[m_page]->setTint(Color(0,1,1,1));
	m_scene->subAdd(m_help[m_page]);
	
	TITLE("Special Items");
	
	WHITE_TEXT(0,30,"Powerups");

	BANNA(40,50,"Briefly makes you invicible to enemy squares.",invinc_tex);
	BANNA(40,86,"Breifly decreases the speed of the squares.",slow_tex);
	BANNA(40,122,"Gives you 1000 points!",plus_tex);
	BANNA(40,158,"Shrinks the size of your player.",mini_tex);
	
	WHITE_TEXT(0,190,"Powerdowns");
	
	BANNA(40,210,"Briefly changes all squares to enemies.",evil_tex);
	BANNA(40,246,"Briefly increases the speed of the squares.",speed_tex);
	BANNA(40,282,"Takes away 1000 points.",minus_tex);
	BANNA(40,318,"Increases the size of your player.",big_tex);
	
	l=new Label(fnt,"Press start or click to continue.",16,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+themeinfo.title_h-16,100));
	m_scene->subAdd(l);
	
	m_page=0;
}

void HowToPlay::inputEvent(const Event & evt) {
	switch(evt.type) {
		case Event::EvtQuit:
			quitNow();
			break;
		case Event::EvtBtnRelease:
			if(!m_exiting && (evt.btn==Event::BtnA || evt.btn==Event::BtnStart || evt.btn==Event::MouseLeftBtn)) {
				if(m_page==1) {
					FadeOut();
					startExit();
					m_exitSpeed=1.0f/30.0f;
				} else {
					m_help[m_page++]->animAdd(new TintFader(Color(0,1,1,1),Color(-1.0f/30.0f,0,0,0)));
					m_help[m_page]->animAdd(new TintFader(Color(1,1,1,1),Color(1.0f/30.0f,0,0,0)));
				}
			}
			break;
	}
}
