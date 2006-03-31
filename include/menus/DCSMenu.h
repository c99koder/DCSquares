/*
 *  DCSMenu.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/20/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/genmenu.h>
#include "drawables/menuList.h"
#include "drawables/playField.h"
#include "drawables/squaresBg.h"
#include "drawables/box.h"

#ifndef _DCSMENU_H
#define _DCSMENU_H

class DCSMenu : public GenericMenu {
public:
	DCSMenu(bool game);
	~DCSMenu();
	void init();
	void FadeIn();
	void FadeOut();

protected:
	void inputEvent(const Event & evt);

private:
	RefPtr<playField> pf;
	RefPtr<squaresBg> bg;
	RefPtr<Box> box;
};

#endif
