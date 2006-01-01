/*
 *  GamePlay.h
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
#include "drawables/banna.h"
#include "DCSMenu.h"
#include "squares.h"

class GamePlay : public DCSMenu {
public:
	void init();
	GamePlay();
	void controlPerFrame();
	
protected:
	void inputEvent(const Event & evt);
private:
};