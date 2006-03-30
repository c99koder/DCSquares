/*
 *  banna.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/20/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BANNA_H
#define _BANNA_H

#include "Tiki/drawables/banner.h"

class Banna : public Drawable {
public:
	Banna(ObjType list, Texture *tex) : m_list(list), m_tex(tex) {};
	~Banna() {};
	
	void setSize(float w, float h);
	
	//Overloaded functions
	void draw(ObjType list);
	
private:
	Texture *m_tex;
	ObjType m_list;
	float m_w, m_h;
};

#endif
