/*
 *  main.cc
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/plxcompat.h>
#include <Tiki/gl.h>
#include <Tiki/hid.h>
#ifdef DREAMCAST
#include <kos.h>
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

using namespace Tiki;
using namespace Tiki::Math;
using namespace Tiki::GL;
using namespace Tiki::GL::Plxcompat;
using namespace Tiki::Hid;

#include "TitleScreen.h"
#include "GamePlay.h"

struct mouse_pos_t {
	int x;
	int y;
	int valid;
};

volatile mouse_pos_t mp[4]; //Woah, 4 mice!

volatile bool quitting = false;
void tkCallback(const Hid::Event & evt, void * data) {
	if (evt.type == Hid::Event::EvtQuit) {
		quitting = true;
	} 
	int port = evt.port;
	if(evt.port < 0 && evt.port > 4) {
		port = 0;
	}
	if (evt.type == Event::EvtAttach) {
		printf("Device attached: %s\n",evt.dev->getName().c_str());
		if(evt.dev->getType() == Device::TypeMouse) {
			mp[port].valid=1;
			mp[port].x=320;
			mp[port].y=240;
		}
	}
	if (evt.type == Event::EvtDetach) {
		printf("Device detached: %s\n",evt.dev->getName().c_str());
		mp[port].valid=0;
	}
	if (evt.type == Event::EvtMouseMove) {
		mp[port].x=evt.x;
		mp[port].y=evt.y;
	}
}

void play_game(level_node *level) {
	GamePlay *gp=new GamePlay();

	current_level=level;
	
	do {
		gp->init();
		gp->FadeIn();
		gp->doMenu();
		current_level=current_level->next;
	} while(current_level != NULL);
	
	current_level=free_play;
}	

extern "C" int tiki_main(int argc, char **argv) {
	TitleScreen *ts;
	
	// Init Tiki
	Tiki::init(argc, argv);
	Tiki::setName("TikiSquares", NULL);
	Tiki::GL::showCursor(false);
	Hid::callbackReg(tkCallback, NULL);

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
	texture_init();
	levels_init();
	text_init("VeraBd.txf",40);
	//load_options();
  //write_options();
	score_list_init();
	load_theme("goat",0);
	srand(time(0));

	ts=new TitleScreen();

	while(!quitting) {	
		ts->init();
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
			case 3:
				quitting=true;
				break;
		}
	}
	Tiki::shutdown();
	
	return 0;
}
