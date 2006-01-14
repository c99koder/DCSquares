/*
 *  NameEntry.cpp
 *  TikiSquares
 *
 *  Created by Sam Steele on 1/8/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tiki/tiki.h"
#include "Tiki/genmenu.h"

using namespace Tiki;
using namespace Tiki::GL;

#include "NameEntry.h"

#include "squares.h"
#include "theme.h"
#include "text.h"
#include "score.h"
extern themeinfo_t themeinfo;

extern RefPtr<Font> fnt;

NameEntry::NameEntry(int rank) : DCSMenu(false) {
	RefPtr<Label> l;
	char tmp[100];
	init();
	
	l=new Label(fnt,"Name Entry",24,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2 + 2,themeinfo.title_y+42,99.99));
	l->setTint(Color(0.6,0,0,0));
	m_scene->subAdd(l);	
	l=new Label(fnt,"Name Entry",24,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+40,100));
	m_scene->subAdd(l);

	sprintf(tmp,"Congratulations!  Your score is rank #%i!",rank);
	l=new Label(fnt,tmp,18,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+100,100));
	l->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f));
	m_scene->subAdd(l);
	l=new Label(fnt,"Please enter your initials below.",18,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+120,100));
	l->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f));
	m_scene->subAdd(l);
	
	for(int i=0; i<3; i++) {
		m_U[i]=new Box(28,2);
		m_U[i]->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f));
		m_scene->subAdd(m_U[i]);
		m_L[i]=new Label(fnt," ",24,true,false);
		m_L[i]->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f));
		m_scene->subAdd(m_L[i]);
	}
	
	set_font_size(24);
	m_U[0]->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2 - 61,14+themeinfo.title_y+themeinfo.title_h/2,100));
	m_U[1]->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2 - 1,14+themeinfo.title_y+themeinfo.title_h/2,100));
	m_U[2]->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2 + 59,14+themeinfo.title_y+themeinfo.title_h/2,100));
	m_L[0]->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2 - 60,themeinfo.title_y+themeinfo.title_h/2,100));
	m_L[1]->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+themeinfo.title_h/2,100));
	m_L[2]->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2 + 60,themeinfo.title_y+themeinfo.title_h/2,100));
	
	m_L[0]->setText("A");
	m_L[0]->setTint(Color(1,1,1));
	m_U[0]->setTint(Color(1,1,1));		
	
	l=new Label(fnt,"Use the d-pad or mouse wheel to select a letter,",16,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+themeinfo.title_h-32,100));
	m_scene->subAdd(l);
	l=new Label(fnt,"and press start or click to continue.",16,true,false);
	l->setTranslate(Vector(themeinfo.title_x + themeinfo.title_w/2,themeinfo.title_y+themeinfo.title_h-16,100));
	m_scene->subAdd(l);
	
	m_pos=0;	
}

std::string NameEntry::getName() {
	return m_L[0]->getText() + m_L[1]->getText() + m_L[2]->getText();
}

void NameEntry::inputEvent(const Event & evt) {
	char tmp[2];
	
	if(evt.type==Event::EvtQuit) {
		quitNow();
	} else if(evt.type==Event::EvtBtnRelease || evt.type==Event::EvtKeypress) {
		if(!m_exiting) {
			if ((m_pos==2 && (evt.btn==Event::BtnA|| evt.btn==Event::MouseLeftBtn)) || evt.btn==Event::BtnStart) {
				FadeOut();
				startExit();
				m_exitSpeed=1.0f/30.0f;
			}
			if (evt.key==Event::KeyLeft) {
				m_L[m_pos]->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f));
				m_U[m_pos]->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f));					
				m_pos--;
				if(m_pos<0) m_pos=0;
				m_L[m_pos]->setTint(Color(1,1,1));
				m_U[m_pos]->setTint(Color(1,1,1));					
			}
			if (evt.key==Event::KeyRight || (m_pos<2 && (evt.btn==Event::BtnA || evt.btn==Event::MouseLeftBtn))) {
				m_L[m_pos]->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f));
				m_U[m_pos]->setTint(Color(themeinfo.text_r/255.0f,themeinfo.text_g/255.0f,themeinfo.text_b/255.0f));					
				m_pos++;
				if(m_pos>2) m_pos=2;
				if(m_L[m_pos]->getText() == " ") m_L[m_pos]->setText("A");
				m_L[m_pos]->setTint(Color(1,1,1));
				m_U[m_pos]->setTint(Color(1,1,1));					
			}
			if(evt.key==Event::KeyUp || evt.btn==Event::MouseWheelUp) {
				strcpy(tmp,m_L[m_pos]->getText().c_str());
				tmp[0]++;
				if(tmp[0]==' '+1) tmp[0]='A';
				if(tmp[0]=='Z'+1) tmp[0]=' ';
				m_L[m_pos]->setText(tmp);
			}
			if(evt.key==Event::KeyDown || evt.btn==Event::MouseWheelDown) {
				strcpy(tmp,m_L[m_pos]->getText().c_str());
				tmp[0]--;
				if(tmp[0]==' '-1) tmp[0]='Z';
				if(tmp[0]=='A'-1) tmp[0]=' ';
				m_L[m_pos]->setText(tmp);
			}
		}
	}
}
