/*
 *  TitleScreen.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/genmenu.h>
#include "drawables/menuList.h"
#include "drawables/playField.h"
#include "drawables/squaresBg.h"
#include "drawables/banna.h"
#include "DCSMenu.h"

class TitleScreen : public DCSMenu {
public:
	TitleScreen();
	void init();

	int getSelection() { return m_selection; }
protected:
	void inputEvent(const Event & evt);
private:
	RefPtr<menuList> ml;
	
	int m_selection;
	int m_menuItems;
	int m_repeatDelay;
};