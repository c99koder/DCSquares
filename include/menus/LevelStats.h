/*
 *  LevelStats.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 1/1/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/genmenu.h>
#include "drawables/playField.h"
#include "drawables/squaresBg.h"
#include "drawables/squaresHUD.h"
#include "menus/DCSMenu.h"

class LevelStats : public DCSMenu {
public:
	LevelStats();
	void init();
	
protected:
		void inputEvent(const Event & evt);
private:
	RefPtr<squaresHUD> hud;
};
