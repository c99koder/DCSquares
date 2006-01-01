/*
 *  squaresHUD.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/25/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SQUARESHUD_H
#define _SQUARESHUD_H

class squaresHUD : public Drawable {
public:
	squaresHUD(int mode);
	~squaresHUD();
	
	//Overloaded functions
	void draw(ObjType list);
	void nextFrame(uint64 tm);
	
private:
	int m_mode;
};

#endif
