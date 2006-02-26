/*
 *  NetworkLobby.cpp
 *  TikiSquares
 *
 *  Created by Sam Steele on 2/19/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/genmenu.h>
#include <Tiki/drawables/banner.h>
#include <Tiki/drawables/label.h>
#include <Tiki/anims/tintfader.h>
#include <Tiki/anims/logxymover.h>
#include <Tiki/texture.h>

using namespace Tiki;
using namespace Tiki::GL;

#include "drawables/box.h"

#include "NetworkLobby.h"
#include "squarenet.h"
#include "net.h"

extern Texture *bg_tex;
extern Texture *game_tex;
extern RefPtr<Font> fnt;
extern std::list<std::string> userlist;

NetworkLobby *lobby;

NetworkLobby::NetworkLobby(char *host, char *user, char *pass) {
	RefPtr<Banner> ban;
	GenericMenu::GenericMenu();
	
	bg = new Banner(Drawable::Opaque,bg_tex);
	bg->setTranslate(Vector(320,240,0));
	bg->setSize(640,480);
	m_scene->subAdd(bg);
	
	chat=new Layer();
	chat->setTranslate(Vector(18,30+384,100));
	m_scene->subAdd(chat);

	ban=new Banner(Drawable::Opaque,game_tex);
	ban->setSize(474,384);
	ban->setTranslate(Vector(20+(470/2),30+(380/2),90));
	m_scene->subAdd(ban);

	ban=new Banner(Drawable::Opaque,game_tex);
	ban->setSize(118,384);
	ban->setTranslate(Vector(502+(118/2),30+(380/2),90));
	m_scene->subAdd(ban);	
	
	users=new Layer();
	users->setTranslate(Vector(502,30,100));
	m_scene->subAdd(users);
	
	RefPtr<Label> l = new Label(fnt,"",16,false,false);
	users->subAdd(l);
	
	box=new Box(640,480);
	box->setTranslate(Vector(320,240,1000));
	m_scene->subAdd(box);
	
	ban=new Banner(Drawable::Opaque,game_tex);
	ban->setSize(474,20);
	ban->setTranslate(Vector(18+(474/2),384+30+(20/2),90));
	m_scene->subAdd(ban);	
	
	text=new Label(fnt,"",12,false,false);
	text->setTranslate(Vector(20,384+30+14,100));
	m_scene->subAdd(text);
	
	chatOffset=0;
	lobby=this;
	lobby_connect(host,user,pass);
}

void NetworkLobby::FadeIn() {
	box->fadeIn();
}

void NetworkLobby::FadeOut() {
	box->fadeOut();
}

void NetworkLobby::reloadUsers() {
	std::list<std::string>::iterator users_iter;
	RefPtr<Label> l;
	int pos=12;
	
	users->subRemoveAll();
	for(users_iter = userlist.begin(); users_iter != userlist.end(); users_iter++) {
		l=new Label(fnt,(*users_iter),14,false,false);
		l->setTranslate(Vector(0,pos,0));
		pos+=14;
		users->subAdd(l);
	}	
}

void NetworkLobby::insertText(int channel, char *text) {
	RefPtr<Label> l = new Label(fnt,text,12,false,false);
	l->setTranslate(Vector(0,chatOffset,0));
	switch(channel) {
		case CHAN_SERVER:
			l->setTint(Color(0,1,1,1));
			l->animAdd(new TintFader(Color(1,1,1,1),Color(1.0f/20.0f,0,0,0)));
			break;
		case CHAN_CHAT:
			if(text[0]=='*') {
				l->setTint(Color(0,94.0f/255.0f,224.0f/255.0f,69.0f/255.0f));
				l->animAdd(new TintFader(Color(1,1,1,1),Color(1.0f/20.0f,0,0,0)));
			} else {
				l->setTint(Color(0,214.0f/255.0f,214.0f/255.0f,214.0f/255.0f));
				l->animAdd(new TintFader(Color(1,1,1,1),Color(1.0f/20.0f,0,0,0)));
			}
			break;
		case CHAN_GAME:
			l->setTint(Color(0,0.9686,0.8786,0.0902));
			l->animAdd(new TintFader(Color(1,1,1,1),Color(1.0f/20.0f,0,0,0)));
			break;
	}
	chatOffset+=12;
	chatText.push_back(l);
	chat->subAdd(l);
	chat->animRemoveAll();
	chat->animAdd(new LogXYMover(20,30+380+6-chatOffset));
	if(chatOffset>372) {
		if(chatOffset>384) {
			chat->subRemove(chatText.front());
			chatText.pop_front();
		}
		chatText.front()->animAdd(new TintFader(Color(0,1,1,1),Color(-1.0f/10.0f,0,0,0)));
	}
}

void NetworkLobby::inputEvent(const Event & evt) {
	snChatMsg m;
	char tmp[2];

	switch(evt.type) {
		case Event::EvtQuit:
			quitNow();
			break;
		case Event::EvtKeypress:
			switch(evt.key) {
				case 13:
					strcpy(m.msg,text->getText().c_str());
					lobby_send(CHAN_CHAT,CHAT_MSG,sizeof(m),&m);
					text->setText("");
					break;
				default:
					if(text->getText().c_str()[0] == 0 || text->getText().c_str()[0] == 13) {
						tmp[0]=evt.key;
						tmp[1]='\0';
						text->setText(tmp);
					} else
						text->setText(text->getText() + (char)evt.key);
					break;
			}
			break;
	}
}

void NetworkLobby::controlPerFrame() {
	lobby_update();
}

void os_chat_insert_text(int channel, char *text) {
	lobby->insertText(channel, text);
}

void os_chat_reload_users() {
	lobby->reloadUsers();
}