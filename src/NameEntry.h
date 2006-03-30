/*
 *  NameEntry.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 1/8/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/genmenu.h>
#include <Tiki/drawables/label.h>
#include "drawables/playField.h"
#include "drawables/squaresBg.h"
#include "DCSMenu.h"

class NameEntry : public DCSMenu {
public:
	NameEntry(int rank);
	std::string getName();
	
protected:
	void inputEvent(const Event & evt);
private:
	RefPtr<Label> m_L[3];
	RefPtr<Box> m_U[3];
	char m_pos;
};