/*
 *  HighScores.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 1/6/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/genmenu.h>
#include "drawables/playField.h"
#include "drawables/squaresBg.h"
#include "drawables/squaresHUD.h"
#include "menus/DCSMenu.h"

class HighScores : public DCSMenu {
public:
	HighScores(int highlight=0);
	
protected:
		void inputEvent(const Event & evt);
};
