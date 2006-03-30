/*
 *  HowToPlay.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 1/22/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/genmenu.h>
#include "drawables/playField.h"
#include "drawables/squaresBg.h"
#include "drawables/squaresHUD.h"
#include "menus/DCSMenu.h"

class HowToPlay : public DCSMenu {
public:
	HowToPlay();
	
protected:
	void inputEvent(const Event & evt);
	
private:
	int m_page;
	Layer *m_help[4];
};
