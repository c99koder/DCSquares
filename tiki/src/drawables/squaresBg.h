/*
 *  squaresBg.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SQUARESBG_H
#define _SQUARESBG_H

class squaresBg : public Drawable {
public:
	squaresBg(Texture *tex, bool game);
	~squaresBg();
	
	//Overloaded functions
	void draw(ObjType list);
	
private:
	bool m_game;
	Texture *m_tex;
};

#endif