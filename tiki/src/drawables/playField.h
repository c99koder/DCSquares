/*
 *  playField.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PLAYFIELD_H
#define _PLAYFIELD_H

class playField : public Drawable {
public:
	playField(bool game);
	~playField();
	
	//Overloaded functions
	void draw(ObjType list);
	void nextFrame(uint64 tm);
	
private:
	bool m_game;
	float m_countdown;
};

#endif