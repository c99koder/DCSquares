/*
 *  main.cc
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <time.h>
#include <Tiki/tiki.h>
#include <Tiki/plxcompat.h>
#include <Tiki/gl.h>
#include <Tiki/hid.h>
#include <Tiki/texture.h>

using namespace Tiki;
using namespace Tiki::GL;

#ifdef DREAMCAST
#include <oggvorbis/sndoggvorbis.h>

extern "C" {
void dcb_vmu_init();
}

extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

#endif
#ifdef GOAT
#include "libmenu.h"
#endif

#include "texture.h"
#include "squares.h"
#include "level.h"
#include "text.h"
#include "options.h"
#include "score.h"
#include "theme.h"
#include "rand.h"
#include "squarenet.h"

using namespace Tiki;
using namespace Tiki::Math;
using namespace Tiki::GL;
using namespace Tiki::GL::Plxcompat;
using namespace Tiki::Hid;

#include "TitleScreen.h"
#include "GamePlay.h"
#include "LevelStats.h"
#include "HighScores.h"
#include "NameEntry.h"
#include "MultiPlaySetup.h"
#include "HowToPlay.h"
#include "NetworkLobby.h"

extern float game_gt;
extern char highcode[];
extern int score[],combo[],maxcombo[];

volatile bool quitting = false;
void tkCallback(const Hid::Event & evt, void * data) {
	if (evt.type == Hid::Event::EvtQuit) {
		quitting = true;
	} 
#ifdef DREAMCAST
	if (evt.type == Hid::Event::EvtBtnRelease && evt.btn == Hid::Event::BtnY) {
		//vid_screen_shot("/pc/Users/sam/sshot.ppm");
	}
#endif
}

extern squarelist *netplayer;

void play_game(level_node *level) {
	GamePlay *gp;
	LevelStats *ls;
	NameEntry *ne;
	HighScores *hs;
	int rank;

	current_level=level;
#ifdef DREAMCAST
	sndoggvorbis_stop();
	sndoggvorbis_start(theme_dir("game.ogg"),1);
#endif	
	do {
		gp=new GamePlay();
		//gp->init();
		gp->FadeIn();
		gp->doMenu();
		//if(current_level->players==1 && !check_win(game_gt,0))
		//	sprintf(highcode,"GOAT-GOAT-GOAT");
		delete gp;
#ifdef NET
		netplayer=NULL;
#endif
		if(quitting) return;
		if(combo[0]>maxcombo[0]) maxcombo[0]=combo[0];
		ls=new LevelStats();
		//ls->init();
		ls->FadeIn();
		ls->doMenu();
		delete ls;
		if(current_level->players==1 && check_win(game_gt,0))
			current_level=current_level->next;
		else
			break;
		if(quitting) return;
	} while(current_level != NULL);
	rank=score_list_rank(score[0]);
	if(current_level->players==1 && rank <= 10) {
		ne=new NameEntry(rank);
		ne->FadeIn();
		ne->doMenu();
		score_list_insert((char *)ne->getName().c_str(),score[0],maxcombo[0],game_gt,current_level->level);
		delete ne;
		save_scores();
		hs=new HighScores(rank);
		hs->FadeIn();
		hs->doMenu();
		delete hs;		
	}		
#ifdef DREAMCAST
		sndoggvorbis_stop();
		sndoggvorbis_start(theme_dir("title.ogg"),1);
#endif	
	current_level=free_play;
}	

extern "C" int tiki_main(int argc, char **argv) {
	TitleScreen *ts;
	HighScores *hs;
	MultiPlaySetup *mps;
	HowToPlay *hp;
	
	// Init Tiki
	Tiki::init(argc, argv);
	//Tiki::setName("TikiSquares", NULL);
	//Tiki::GL::showCursor(false);
	Hid::callbackReg(tkCallback, NULL);

#if TIKI_PLAT==TIKI_WIN32 //init winsock
		WORD sockVersion;
	WSADATA wsaData;
	int nret;

	sockVersion = MAKEWORD(1, 1);			// We'd like Winsock version 1.1


	// We begin by initializing Winsock
	WSAStartup(sockVersion, &wsaData);
#endif

#ifdef DREAMCAST
  //arch_set_exit_path(ARCH_EXIT_MENU);
  fs_chdir("/rd");
	
	dcb_vmu_init();
	sndoggvorbis_init();
#ifdef GOAT
	goat_init();
	if(goat_50hz()) vid_set_mode(DM_640x480_PAL_IL,PM_RGB565);
#endif
#endif
	levels_init();
	score_list_init();
	text_init("VeraBd.txf",40);
	//load_options();
  //write_options();
	//goat_save_erase();
	score_list_init();
	load_theme("goat",0);
	srand(time(0));

#ifdef NET0
	
#if TIKI_PLAT == TIKI_MACOSX
	NetworkLobby *nl = new NetworkLobby("192.168.11.100","c99koder","027325");
#else
	NetworkLobby *nl = new NetworkLobby("192.168.11.100","DJ Shibby","jay123");
#endif
	nl->insertText(CHAN_GAME,"Connecting to lobby...");
	nl->FadeIn();
	nl->doMenu();

#endif
	
#ifdef DREAMCAST
	sndoggvorbis_start(theme_dir("title.ogg"),1);
#endif
	while(!quitting) {	
		ts=new TitleScreen();
		ts->FadeIn();
		ts->doMenu();

		if(quitting) break;
		
		switch(ts->getSelection()) {
			case 0:
				play_game(level_list_head);
				break;
			case 1:
				play_game(free_play);
				break;
			case 2:
				mps=new MultiPlaySetup;
				mps->FadeIn();
				mps->doMenu();
				switch(mps->getSelection()) {
					case 0:
						multi_play->win_mode=MODE_SQUARES;
						multi_play->lose_mode=MODE_TIME;
						multi_play->squares=20;
						break;
					case 1:
						multi_play->win_mode=MODE_SQUARES;
						multi_play->lose_mode=MODE_TIME;
						multi_play->squares=50;
						break;
					case 2:
						multi_play->win_mode=MODE_SCORE;
						multi_play->lose_mode=MODE_TIME;
						multi_play->score=5000;
						break;
					case 3:
						multi_play->win_mode=MODE_SCORE;
						multi_play->lose_mode=MODE_TIME;
						multi_play->score=10000;
						break;
					case 4:
						multi_play->win_mode=0;
						multi_play->lose_mode=MODE_ENEMY;
						break;
				}
				delete mps;
				play_game(multi_play);
				break;
			case 3:
				hp=new HowToPlay();
				hp->FadeIn();
				hp->doMenu();
				delete hp;
				break;
			case 4:
				hs=new HighScores();
				hs->FadeIn();
				hs->doMenu();
				delete hs;
				break;
			case 5:
				quitting=true;
				break;
		}
		delete ts;
	}
	
		glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
	
	for(float x=0; x<= 1; x+=0.025) {
		Frame::begin();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glColor3f(1-x,1-x,1-x);
		glBegin(GL_QUADS);
		glVertex3f(0,0,0.9);
		glVertex3f(640,0,0.9);
		glVertex3f(640,480,0.9);
		glVertex3f(0,480,0.9);
		glEnd();
		Frame::finish();
	}

	Tiki::shutdown();
#ifdef GOAT
	goat_exit();
#endif
	return 0;
}
