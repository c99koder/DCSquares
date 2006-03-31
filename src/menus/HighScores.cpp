/*
 *  HighScores.cpp
 *  TikiSquares
 *
 *  Created by Sam Steele on 1/6/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tiki/tiki.h"
#include "Tiki/genmenu.h"
#include <Tiki/texture.h>
#include <Tiki/drawables/label.h>
#include <Tiki/anims/logxymover.h>

using namespace Tiki;
using namespace Tiki::GL;

#include "menus/HighScores.h"
#include "squares.h"
#include "score.h"
#include "theme.h"
#include "text.h"

extern RefPtr<Font> fnt;
extern themeinfo_t themeinfo;

char *format_time(int seconds);

HighScores::HighScores(int highlight) : DCSMenu(false) {
	RefPtr<Label> l;
	RefPtr<Layer> scoretable=new Layer();
	char pos[100],name[100],score[100],combo[100],time[100];
	struct score_list_node *current=score_list_head;
	init();

	l=new Label(fnt,"High Scores",24,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2 + 2,themeinfo.title_y+42,99.99));
	l->setTint(Color(0.6,0,0,0));
	m_scene->subAdd(l);
	l=new Label(fnt,"High Scores",24,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+40,100));
	m_scene->subAdd(l);

	l=new Label(fnt,"Name",20,true,false);
	l->setTranslate(Vector(-200,0,100));
	scoretable->subAdd(l);
	l=new Label(fnt,"Score",20,true,false);
	l->setTranslate(Vector(-60,0,100));
	scoretable->subAdd(l);
	l=new Label(fnt,"Combo",20,true,false);
	l->setTranslate(Vector(75,0,100));
	scoretable->subAdd(l);
	l=new Label(fnt,"Time",20,true,false);
	l->setTranslate(Vector(200,0,100));
	scoretable->subAdd(l);
	
	set_font_size(20);
	
	for(int i=1; i<=10; i++) {
		if(current!=NULL) {
			sprintf(name,"%s",current->name);
			sprintf(score,"%i",current->score);
			sprintf(combo,"%i",current->combo);
			sprintf(time,"%s",format_time(current->time));
			current=current->next;
		} else {
			sprintf(name,"---");
			sprintf(score,"0");
			sprintf(combo,"0");
			sprintf(time,"0:00");
		}
		sprintf(pos,"%i.",i);
		l=new Label(fnt,pos,20,false,false);
		l->animAdd(new LogXYMover(-240-txt_width(pos),i*24+12));
		l->setTranslate(Vector(-240-txt_width(pos),500,100));
		scoretable->subAdd(l);
		l=new Label(fnt,name,20,false,false);
		l->animAdd(new LogXYMover(-200-txt_width(name)/2,i*24+12));
		l->setTranslate(Vector(-200-txt_width(name)/2,500,100));
		if(i!=highlight) l->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f));
		scoretable->subAdd(l);
		l=new Label(fnt,score,20,false,false);
		l->animAdd(new LogXYMover(-60-txt_width(score)/2,i*24+12));
		l->setTranslate(Vector(-60-txt_width(score)/2,500,100));
		if(i!=highlight) l->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f));
		scoretable->subAdd(l);
		l=new Label(fnt,combo,20,false,false);
		l->animAdd(new LogXYMover(75-txt_width(combo)/2,i*24+12));
		l->setTranslate(Vector(75-txt_width(combo)/2,500,100));
		if(i!=highlight) l->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f));
		scoretable->subAdd(l);
		l=new Label(fnt,time,20,false,false);
		l->animAdd(new LogXYMover(200-txt_width(time)/2,i*24+12));
		l->setTranslate(Vector(200-txt_width(time)/2,500,100));
		if(i!=highlight) l->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f));
		scoretable->subAdd(l);
	}
	
	scoretable->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+80,0));
	m_scene->subAdd(scoretable);
	
	l=new Label(fnt,"Press start or click to continue.",16,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+themeinfo.title_h-16,100));
	m_scene->subAdd(l);
}

void HighScores::inputEvent(const Event & evt) {
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