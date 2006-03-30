/*
 *  MultiPlaySetup.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 1/14/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/genmenu.h>
#include "drawables/menuList.h"
#include "drawables/playField.h"
#include "drawables/squaresBg.h"
#include "menus/DCSMenu.h"

class MultiPlaySetup : public DCSMenu {
public:
	MultiPlaySetup();
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
